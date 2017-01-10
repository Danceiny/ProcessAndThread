#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>
#include<limits.h>
#include<assert.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_PROCESS_COUNT 99

const char * inputfile = "input.txt";
const char * outputfile = "output.txt";
const char * splitter1 = "=";
const char * splitter2 = "\n";

unsigned int process_number = 5;
unsigned int require_number = 20;
unsigned int cur_number = 0;
unsigned int loop = 0;


struct msg_st
{
	long type;	// must be long int type
	unsigned int cur;
	long sum;
};
int main()
{
	// Open file
	FILE *fp;

	if((fp=fopen(inputfile,"r"))==NULL)
	{
		printf("Sorry, I can not open the file! Please verify it~~~");
		return -1;
	}

	// Read file and Extract required parameters
	char buf[MAX_BUFFER_SIZE];
	unsigned long tmp[2];
	char *p1, *p2;
	int i=0;
	while(fgets(buf,MAX_BUFFER_SIZE,fp)){
		if(i>=2)break;	//if the file is over 2 lines, stop reading the file after 2 lines got.
		p1 = strtok(buf,splitter1);
		p2 = strtok(NULL,splitter2);
		printf("the number read is %s.\n", p2);
		tmp[i++] = (unsigned long)strtoul(p2,NULL,10);
	}
	process_number = (unsigned int)(tmp[0]<MAX_PROCESS_COUNT ? tmp[0] : MAX_PROCESS_COUNT);
	if(tmp[1] > UINT_MAX)
	{
		printf("the require number is too big, I'll set it as UINT_MAX.\n");
		tmp[1] = (unsigned long)(UINT_MAX);
	}
	require_number = tmp[1] & UINT_MAX;	// narrow typecast 


	printf("the process number is %u,\tthe require number is %u.\n",process_number,require_number); 
	assert(process_number > 0 && process_number <= MAX_PROCESS_COUNT);
	assert(require_number >= 0 && require_number <= UINT_MAX);

	// Close file
	fclose(fp);

	struct msg_st data;
	data.type = 1;	// must greater than 0
	data.cur = 0;
	data.sum = 0;
	int msgid = -1;
	msgid = msgget((key_t)1239,0666|IPC_CREAT|IPC_NOWAIT);
	//msgid = msgget((key_t)1235,0666|IPC_CREAT); //also work correctly
	msgsnd(msgid,&data,sizeof(struct msg_st)-sizeof(long),0);	
	pid_t fpid;

	printf("ddd3d\n");
	while(loop < process_number)
	{
	    fpid=fork();
		loop++;
		if(0 == fpid)	// child process
		{
		   	while(1)
		    { 	
				//printf("the current process id is %d...",getpid());
				if(-1 == msgrcv(msgid,&data,sizeof(struct msg_st)-sizeof(long),0,0));	// failed
				
				data.cur++;
	
				if(data.cur > require_number)
				{
					data.cur = require_number;
					msgsnd(msgid,&data,sizeof(struct msg_st)-sizeof(long),0);					
					return 0;	// cannot with exit
				}

				if(data.cur == require_number)
				{
					data.sum += data.cur;	

					msgsnd(msgid,&data,sizeof(struct msg_st)-sizeof(long),0);
					printf("the current number is %u,\tthe current sum is %ld\n",data.cur,data.sum);				
					printf("the calculation is over...\n");
					
					// write the sum to a text file
					char buf[MAX_BUFFER_SIZE];
					sprintf(buf,"%ld",data.sum);
					FILE *fp;
					if((fp=fopen(outputfile,"w+")) == NULL)
					{
						printf("Sorry failed to open the file %s\n",outputfile);
						return -1;
					}
					fwrite(buf,strlen(buf),1,fp);
					fflush(fp);
					fclose(fp);
					return 0;	// cannot with exit
				}

				data.sum += data.cur;
				printf("the current number is %u,\tthe current sum is %ld\n",data.cur,data.sum);
				msgsnd(msgid,&data,sizeof(struct msg_st)-sizeof(long),0);
		   }
		}
		sleep(1);
	}

	msgrcv(msgid,&data,sizeof(struct msg_st)-sizeof(long),0,0);
	return 0;
}
