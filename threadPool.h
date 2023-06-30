#ifndef _THREADPOOL_
#define _THREADPOOL_
#include "codec.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define DEST_SIZE 1025

typedef struct
{
    pthread_t *poolThread;
    size_t poolSize;
    pthread_mutex_t mutex_lock;
    pthread_cond_t mutex_cond;
} ThreadPool;

typedef struct Task
{
    char *buffer;
    int threadId;
    struct Task *next;
} *p_task;

int key;

p_task headTask = NULL;
p_task tailTask = NULL;
p_task currentTask = NULL;
ThreadPool pool;

int initThreadPool(ThreadPool *pool, int initialSize);

void destroyThreadPool(ThreadPool *pool);

int addThreadToPool(ThreadPool *pool);

void printAll();

void execTask(p_task task, char *argv);

void addTask(char *buf);

void *startJob(void *args);
#endif
