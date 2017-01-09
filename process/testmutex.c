#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>




int main(void){//2个进程，一个进程完成每次加1,另一个进程完成每次加2,2个进程协作完成累加,使用共享内存方式在进程间通信

int *x;
int rt;
int shm_id;
char *addnum="myadd";
char *ptr;

pthread_mutex_t mutex;//互斥对象
pthread_mutexattr_t mutexattr;//互斥对象属性


   pthread_mutexattr_init(&mutexattr);//初始化互斥对象属性
   pthread_mutexattr_setpshared(&mutexattr,PTHREAD_PROCESS_SHARED);//设置互斥对象为PTHREAD_PROCESS_SHARED共享，即可以在多个进程的线程访问,PTHREAD_PROCESS_PRIVATE为同一进程的线程共享
   rt=fork();//复制父进程，并创建子进程 

   if (rt==0){//子进程完成x+1
       shm_id=shm_open(addnum,O_RDWR,0664);//为0时，在Linux下正常
       ptr=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
       x=(int *)ptr;  
  
      for (int i=0;i<10;i++){
           pthread_mutex_lock(&mutex);      
       (*x)++;
       printf("x++\t:\t%d\n",*x);
           pthread_mutex_unlock(&mutex); 
       sleep(1);                 
      }
   }   
   else{//父进程完成x+2
       shm_id=shm_open(addnum,O_RDWR|O_CREAT,0644);
        ftruncate(shm_id,sizeof(int));
        ptr=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
        x=(int *)ptr; 
          
      for (int i=0;i<10;i++){
           pthread_mutex_lock(&mutex);       
       (*x)+=2;
       printf("x+=2\t:\t%d\n",*x);
           pthread_mutex_unlock(&mutex);  
           sleep(1); 
      }      
          
   } 
   shm_unlink(addnum);//删除共享名称
   munmap(ptr,sizeof(int));//删除共享内存
   return(0);
}
