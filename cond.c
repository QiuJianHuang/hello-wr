#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

int sum = 0;
int pthread_start_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_count = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_count_tt = PTHREAD_COND_INITIALIZER;


void *pthread1(void *arg)
{  
    int index = 0;

	pthread_mutex_lock(&mutex);
	pthread_start_count ++;
	printf("thread1 ++ \n");
	pthread_cond_signal(&cond_count_tt);
	printf("thread1 waiting cond_count \n");
	pthread_cond_wait(&cond_count, &mutex);
	pthread_mutex_unlock(&mutex);
	for(; index <= 550; index++)
	{
        pthread_mutex_lock(&mutex);
	    sum++;
	    printf("pthread1 sum:%d \n", sum);
		if(sum == 100)
		{
		    printf("pthread1 signal \n");
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cond);
		}else
		{
	        pthread_mutex_unlock(&mutex);
		}
	}

	pthread_exit(NULL);
}

void *pthread2(void *arg)
{
	int index = 0;

	pthread_mutex_lock(&mutex);
	pthread_start_count ++;
	printf("thread2 ++ \n");
	pthread_cond_signal(&cond_count_tt);
	printf("thread1 waiting cond_count \n");
	pthread_cond_wait(&cond_count, &mutex);
	pthread_mutex_unlock(&mutex);
	for(; index <= 550; index++)
	{
        pthread_mutex_lock(&mutex);
	    sum++;
	    printf("pthread2 sum:%d \n", sum);
		if(sum == 100)
		{
		    printf("pthread2 signal \n");
			pthread_cond_signal(&cond);
		}else
		{
			pthread_mutex_unlock(&mutex);
		}
	    
	}

	pthread_exit(NULL);
	
}

void *pthread3(void *arg)
{
    printf("pthread3 run \n");
    pthread_mutex_lock(&mutex);
	while(pthread_start_count < 2)
	{	
	    printf("thread3 wait cond_count_tt \n");
	    pthread_cond_wait(&cond_count_tt, &mutex);
	}
	printf("thread3 broadcast \n");
	pthread_cond_broadcast(&cond_count);
	while(sum < 100)
	{
		pthread_cond_wait(&cond, &mutex);
	}
	printf("count over, sum:%d \n", sum);
    pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
}
int main()
{
	int res;
	pthread_t pth1;
	pthread_t pth2;
	pthread_t pth3;

	res = pthread_create(&pth1, NULL, pthread1, NULL);
	if(res != 0)
	{
		printf("pthread1 failed ! \n");
        return -1;
	}

	res = pthread_create(&pth2, NULL, pthread2, NULL);
	if(res != 0)
	{
		printf("pthread2 failed ! \n");
        return -1;
	}

    res = pthread_create(&pth3, NULL, pthread3, NULL);
	if(res != 0)
	{
		printf("pthread3 failed ! \n");
        return -1;
	}

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	pthread_join(pth3, NULL);

	return 0;
	
}
