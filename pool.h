#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <list>
using namespace std;

typedef void *(*threadmethod) (void *arg);

struct assignment
{
	threadmethod process;
	void *arg;
};

class Pool
{
public:
	pthread_mutex_t queue_lock;
	pthread_cond_t queue_ready;
	list<assignment> task;
	int MAX;
	int num;
	int shut;
	pthread_t *threadid;
	Pool(int max);
	void add(threadmethod process, void *arg);
	~Pool();
};

void* RUN(void *arg)
{
	Pool* pool = (Pool*)arg;
	while (1)
	{
		pthread_mutex_lock(&(pool->queue_lock));
		while (pool->num == 0 && !pool->shut)
			pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
		if (pool->shut)
		{
			pthread_mutex_unlock(&(pool->queue_lock));
			pthread_exit(NULL);
		}
		if (!pool->task.empty())
		{
			assignment assign = pool->task.front();
			pool->task.pop_front();
			pool->num--;
			pthread_mutex_unlock(&(pool->queue_lock));
			(*(assign.process)) (assign.arg);
		}
	}
	pthread_exit(NULL);
}

Pool::Pool(int max)
{
	pthread_mutex_init(&(this->queue_lock), NULL);
	pthread_cond_init(&(this->queue_ready), NULL);
	MAX = max;
	num = 0;
	shut = 0;
	threadid = (pthread_t *)malloc(MAX * sizeof(pthread_t));
	for (int i = 0; i < max; i++)
		pthread_create(&(this->threadid[i]), NULL, RUN, (void*)this);
}

void Pool::add(threadmethod process, void *arg)
{
	assignment A;
	A.process = process;
	A.arg = arg;
	pthread_mutex_lock(&(this->queue_lock));
	task.push_back(A);
	num++;
	pthread_mutex_unlock(&(this->queue_lock));
	pthread_cond_signal(&(this->queue_ready));
}

Pool::~Pool()
{
	if (this->shut)
		return;
	this->shut = 1;
	pthread_cond_broadcast(&(this->queue_ready));
	int i;
	for (i = 0; i < MAX; i++)
		pthread_join(this->threadid[i], NULL);
	free(this->threadid);
	pthread_mutex_destroy(&(this->queue_lock));
	pthread_cond_destroy(&(this->queue_ready));
}