//Deependra Thakur
//1001753178


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX 36
#define NUM_THREAD 4


int final=0;
int n1,n2; 
char *s1,*s2;
pthread_mutex_t m;

FILE *fp;

int readf(char* filename)
{
    if((fp=fopen(filename, "r"))==NULL)
    {
        printf("ERROR: can’t open %s!\n", filename);
        return 0;
    }
    
    s1=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory!\n") ;
        return -1;
    }
    
    s2=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory\n") ;
        return -1;
    }
    
    /*read s1 s2 from the file*/
    
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1); /*length of s1*/
    n2=strlen(s2)-1; /*length of s2*/
    
    if( s1==NULL || s2==NULL || n1 < n2 ) /*when error exit*/
    {
        return -1;
    }
}

void* num_substring ( void*arg )
{
    int i,j,k;
    int total=0;
    int count ;
    int thread_id=*(int*)arg;
    int start =(thread_id)*(n1/NUM_THREAD);
    int end=(thread_id+1)*(n1/NUM_THREAD);

    for (i=start; i <=end; i++)
    {
        count =0;
        for(j = i ,k = 0; k < n2; j++,k++)
        { /*search for the next string of size of n2*/
            if (*(s1+j)!=*(s2+k))
            {
                break ;
            }
            else
            {
                count++;
            }
            if (count==n2)
                //pthread_mutex_lock(&lock);
                total++; /*find a substring in this step*/
                // pthread_mutex_unlock(&lock);
         }
    }
    pthread_mutex_lock(&m);
    final+=total; 
    pthread_mutex_unlock(&m);
    return NULL ;
}
    
int main(int argc, char *argv[])
{
    int count=0 ;
    pthread_t tid[NUM_THREAD];
    int id[NUM_THREAD];



    if( argc < 2 )
    {
      printf("Error: You must pass in the datafile as a commandline parameter\n");
    }

    readf ( argv[1] ) ;

    struct timeval start, end;
    float mtime; 
    int secs, usecs;    

    gettimeofday(&start, NULL);

    //count = num_substring () ;
    for(int i=0;i<NUM_THREAD;i++){
        id[i]=i;
        if(pthread_create(&tid[i],NULL,num_substring,(void*)&id[i])){
            perror("error creating thread");
        }
    }
    for(int i=0;i<NUM_THREAD;i++){
        if(pthread_join(tid[i],NULL)){
            perror("Error joining thread");
        }
    }

    gettimeofday(&end, NULL);

    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

    printf ("The number of substrings is : %d\n",final) ;
    printf ("Elapsed time is : %f milliseconds\n", mtime );

    if( s1 )
    {
      free( s1 );
    }

    if( s2 )
    {
      free( s2 );
    }

    return 0 ; 
}