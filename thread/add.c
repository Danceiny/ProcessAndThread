#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<syscall.h>
#include<stdlib.h>
#include<limits.h>
#include<assert.h>

#define MAX_THREAD_COUNT 99
#define MAX_BUFFER_SIZE 1024

/* global variables */
unsigned int thread_number = 5;		// N
unsigned int require_number = 20;	// M
unsigned int cur_number = 0;
long sum = 0;

const char * inputfile = "input.txt";
const char * outputfile = "output.txt"; 
const char *splitter1 = "=";
const char *splitter2 = "\n";

pthread_mutex_t Device_mutex;
void *ret;
void *add(void *arg);

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
		p1 = strtok(buf,splitter1);
		p2 = strtok(NULL,splitter2);
		tmp[i++] = (unsigned long)strtoul(p2,NULL,10);
	}

	thread_number = (unsigned int)(tmp[0]<MAX_THREAD_COUNT ? tmp[0] : MAX_THREAD_COUNT);
	if(tmp[1] > UINT_MAX)
	{
		printf("the require number is too big, I'll set it as UINT_MAX \n");
		tmp[1] = (unsigned long)(UINT_MAX);
	}
	require_number = tmp[1] & UINT_MAX;

	// Close file
	fclose(fp); 

	// Init thread
	pthread_mutex_init(&Device_mutex,NULL);
	pthread_t pthread[MAX_THREAD_COUNT];

	// Create thread
	for(i = 0 ;i < thread_number; i++)
	{
		printf("create the %uth thread~~~\n", i+1);
		pthread_create(&pthread[i],NULL,add,NULL);
	}		
	for(i = 0; i < thread_number; i++){
		pthread_join(pthread[i],&ret);
	}

    return 0;
}
void *add(void *arg)
{
	while(cur_number <= require_number)
    {
		pthread_mutex_lock(&Device_mutex);
		sum = sum + cur_number;
		printf("the thread id = %u , the current num is %u,\tthe current sum is %ld\n" , syscall(SYS_gettid),cur_number,sum);		
		cur_number++;
		pthread_mutex_unlock(&Device_mutex);
		usleep(100);	
    }
    if(cur_number > require_number)
    {	
    	char buf[MAX_BUFFER_SIZE];
    	sprintf(buf,"%ld",sum);
    	FILE *fp;
		if((fp=fopen(outputfile,"w+"))==NULL)
		{
			printf("Sorry, I can not open the file! Please verify it~~~");
			return -1;
		}
		fwrite(buf,strlen(buf),1,fp);
		fflush(fp);
		printf("write to file...\n");
		fclose(fp);
    }
}
