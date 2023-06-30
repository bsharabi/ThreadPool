#include "threadPool.h"

int initThreadPool(ThreadPool *pool, int initialSize)
{
    pool->poolThread = (pthread_t *)malloc(sizeof(pthread_t) * initialSize);
    if (pool->poolThread == NULL)
    {
        perror("Error: Unable to allocate memory for the thread pool");
        return 1;
    }
    pool->poolSize = initialSize;
    return 0;
}

void destroyThreadPool(ThreadPool *pool)
{
    free(pool->poolThread);
}

int addThreadToPool(ThreadPool *pool)
{
    size_t newSize = pool->poolSize + 1;
    pthread_t *newPoolThread = (pthread_t *)realloc(pool->poolThread, sizeof(pthread_t) * newSize);
    if (newPoolThread == NULL)
    {
        perror("Error: Unable to allocate memory for the new thread");
        return 1;
    }
    pool->poolThread = newPoolThread;
    pool->poolThread[pool->poolSize] = newSize;
    pool->poolSize = newSize;

    return 0;
}

void execTask(p_task task, char *argv)
{

    if (strcmp(argv, "-e") == 0)
        encrypt(task->buffer, key);
    else if (strcmp(argv, "-d") == 0)
        decrypt(task->buffer, key);
    else
        printf("Error");
}
void destroyTask()
{
    while (headTask != NULL)
    {
        p_task task = headTask;
        headTask = headTask->next;
        printf("%s", task->buffer);
        free(task->buffer);
        free(task);
    }
}

void addTask(char *buf)
{
    p_task task = (p_task)malloc(sizeof(p_task));
    if (task == NULL)
    {
        perror("Eroor");
        return;
    }
    task->buffer = (char *)malloc(sizeof(char) * DEST_SIZE);
    if (task->buffer == NULL)
    {
        free(task);
        return;
    }
    strcpy(task->buffer, buf);

    if (headTask == NULL)
        tailTask = headTask = task;
    else
    {
        tailTask->next = task;
        tailTask = task;
    }
    task->threadId = pool.poolSize;
}

void *startJob(void *args)
{
    p_task task;
    pthread_mutex_lock(&pool.mutex_lock);
    if (currentTask != NULL)
    {
        task = currentTask;
        currentTask = currentTask->next;
        pthread_mutex_unlock(&pool.mutex_lock);
        pthread_cond_broadcast(&pool.mutex_cond);
        execTask(task, args);
    }
    pthread_mutex_unlock(&pool.mutex_lock);
    pthread_cond_broadcast(&pool.mutex_cond);
    return NULL;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("usage: key < file \n");
        printf("!! data more than 1024 char will be ignored !!\n");
        return 0;
    }
    if (initThreadPool(&pool, 0))
        return 1;

    if (pthread_mutex_init(&pool.mutex_lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }
    if (pthread_cond_init(&pool.mutex_cond, NULL) != 0)
    {
        printf("\n cond init has failed\n");
        return 1;
    }

    key = atoi(argv[1]);

    char ch;
    int counter = 0;
    char data[DEST_SIZE];

    while ((ch = getchar()) != EOF)
    {
        data[counter++] = ch;
        if (counter == 1024)
        {
            data[counter] = '\0';
            addTask(data);
            if (addThreadToPool(&pool))
                return 1;
            counter = 0;
        }
    }

    if (counter > 0)
    {
        data[counter] = '\0';
        addTask(data);
        if (addThreadToPool(&pool))
            return 1;
    }
    currentTask = headTask;
    for (int i = 0; i < pool.poolSize; i++)
    {
        if (pthread_create(&pool.poolThread[i], NULL, startJob, argv[2]) != 0)
        {
            perror("Failed to create the thread");
        }
    }
    for (int i = 0; i < pool.poolSize; i++)
    {
        if (pthread_join(pool.poolThread[i], NULL) != 0)
        {
            fprintf(stderr, "Failed to join thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    destroyTask();
    pthread_mutex_destroy(&pool.mutex_lock);
    pthread_cond_destroy(&pool.mutex_cond);
    destroyThreadPool(&pool);
    return 0;
}