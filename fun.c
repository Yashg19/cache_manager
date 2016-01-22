#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdint.h>

sem_t dbAccess;
sem_t readCountAccess;

int readCount=0;
int val = 100;

void *Reader(void *arg);
void *Writer(void *arg);

int main(int argc, char* argv[])
{
  int i=0, num_of_readers = 0, num_of_writers = 0;

  //inititalizing semaphores
  sem_init(&readCountAccess,0,1);
  sem_init(&dbAccess,0,1);

  pthread_t readers_tid[100], writer_tid[100];
  num_of_readers = atoi(argv[1]);
  num_of_writers = atoi(argv[2]);
  
  for(i = 0; i < num_of_readers; i++)
    {
      pthread_create(&readers_tid[i], NULL , Reader, (void *) (intptr_t) i);
    }

  for(i = 0;i < num_of_writers; i++)
    {
      pthread_create(&writer_tid[i], NULL, Writer, (void *) (intptr_t) i);
    }

  for(i = 0; i < num_of_writers; i++)
    {
      pthread_join(writer_tid[i],NULL);
    }

  for(i = 0; i < num_of_readers; i++)
    {
      pthread_join(readers_tid[i], NULL);
    }

  sem_destroy(&dbAccess);
  sem_destroy(&readCountAccess);
  return 0;
}

void * Writer(void *arg)
{
  
  sleep(1);
  int temp=(intptr_t) arg;
  printf("Writer %d is trying to enter into db to modify data:\n",temp);
  sem_wait(&dbAccess);
  printf("Writer %d Working\n",temp);
  val++;
  printf("Writer %d Done\n");
  sem_post(&dbAccess);
}

void *Reader(void *arg)
{
  sleep(1);
  int temp=(intptr_t) arg;
  printf("Reader %d is trying to read data from db:\n",temp);
  sem_wait(&readCountAccess);
  readCount++;
  if(readCount==1)
    {
      sem_wait(&dbAccess);
      printf("Reader %d Reading Value = %d\n",temp, val);
    }
  sem_post(&readCountAccess);
  sem_wait(&readCountAccess);
  readCount--;
  if(readCount==0)
    {
      printf("Reader %d Done\n",temp);
      sem_post(&dbAccess);
    }
  sem_post(&readCountAccess);
}

