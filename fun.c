#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdint.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#define MAX 100

sem_t dbAccess;
sem_t readCountAccess;

int readCount=0, num = 0;
int arr[MAX] = {0};

void *Reader(void *arg);
void *Writer(void *arg);

int main(int argc, char* argv[])
{
  int i=0, num_of_readers = 0, num_of_writers = 0;
  srand(time(NULL));

  //inititalizing semaphores
  sem_init(&readCountAccess,0,1);
  sem_init(&dbAccess,0,1);

  pthread_t readers_tid[1000], writer_tid[1000];
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

  printf("Array Contents\n");
  for(i=0; i<MAX; i++)
    printf("%d", arr[i]);
  printf("\n");

  return 0;
}

void * Writer(void *arg)
{
  sleep(1);

  int temp=(intptr_t) arg;

  printf("Writer %d is trying to enter into db to modify data:\n",temp);

  sem_wait(&dbAccess);

  printf("Writer %d Working\n",temp);
  num = rand()%MAX;
  arr[num]++;
  printf("After Write Value of index(%d) = %d\n",num, arr[num]); 
  printf("Writer %d Done\n", temp);
  sem_post(&dbAccess);
  
  return 0;
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
      printf("Reader %d Reading\n",temp);
      num = rand()%MAX;
      printf("Read Value of index(%d) = %d\n",num, arr[num]); 
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
  return 0;
}

