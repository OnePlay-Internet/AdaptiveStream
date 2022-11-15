/**
 * @file queue.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_util.h>
#define BASE_SIZE 1024


#include <thread>
#include <stdio.h>
#include <string.h>


#include <pthread.h>




typedef struct _BufferLL BufferLL;
struct _BufferLL {
    AdsBuffer* buff;

    BufferLL* next;
};

struct _QueueArray {
    /**
     * @brief 
     * 
     */
    BufferLL* first;

    uint64 size;
    uint64 max_size;

    pthread_mutex_t mutex;
};




/**
 * @brief 
 * 
 * @param queue 
 * @param data 
 * @return true 
 * @return false 
 */
bool            
queue_array_push(AdsQueue* queue, 
                 AdsBuffer* obj)
{
    BufferLL* last = (BufferLL*)malloc(sizeof(BufferLL));
    memset(last,0,sizeof(BufferLL));

    while (queue->size >= queue->max_size)
        std::this_thread::sleep_for(ATOMIC_SLEEP);

#ifdef BUFFER_TRACE
#ifndef MINGW
        BUFFER_CLASS->ref(obj,NULL,"\\ads_queue.cpp",__LINE__);
#else
        BUFFER_CLASS->ref(obj,NULL,"/ads_queue.cpp",__LINE__);
#endif
#else
    BUFFER_CLASS->ref(obj,NULL);
#endif

    // lock this
    last->buff  = obj;
    last->next = NULL;

    {
        BufferLL* container;
        pthread_mutex_lock(&queue->mutex);
        if(!queue->first) {
            queue->first = last;
        } else {
            container = queue->first;
            while (container->next) { container = container->next; }
            container->next = last;
        }
        pthread_mutex_unlock(&queue->mutex);
    }

    queue->size++;
    return true;
}

bool
queue_array_peek(AdsQueue* queue)
{
    return queue->size > 0 ? true : false;
}

uint64
queue_array_size(AdsQueue* queue)
{
    return queue->size;
}



pointer
queue_array_pop(AdsQueue* queue, 
                  AdsBuffer** buf,
                int* size)
{
    BufferLL* container;
    AdsBuffer *ret;

    // lock this
    while(!queue->first || !queue->size) 
        std::this_thread::sleep_for(ATOMIC_SLEEP);
    

    {
        pthread_mutex_lock(&queue->mutex);
        container = queue->first;
        ret = queue->first->buff;
        queue->first = queue->first->next;
        pthread_mutex_unlock(&queue->mutex);
    }


    free(container);
    *buf = ret;
    pointer data;
#ifdef BUFFER_TRACE
#ifndef MINGW
    char* file = "\\ads_queue.cpp";
#else
    char* file = "/ads_queue.cpp";
#endif
    data =  BUFFER_CLASS->ref(ret,size,file,__LINE__);
    BUFFER_CLASS->unref(ret,file,__LINE__);
#else 
    data =  BUFFER_CLASS->ref(ret,size);
    BUFFER_CLASS->unref(ret);
#endif

    queue->size--;
    return data;
}


AdsQueue*     
queue_array_init(int max_size)
{
    AdsQueue* array = (AdsQueue*)malloc(sizeof(AdsQueue));
    array->first = NULL;
    array->size = 0;
    array->max_size = max_size;
    array->mutex = PTHREAD_MUTEX_INITIALIZER;
    return array;
}


void            
queue_array_finalize(AdsQueue* queue)
{
    while(queue_array_peek(queue))
    {
        AdsBuffer* buf;
        queue_array_pop(queue,&buf,NULL);
#ifdef BUFFER_TRACE
#ifndef MINGW
    char* file = "\\ads_queue.cpp";
#else
    char* file = "/ads_queue.cpp";
#endif
    BUFFER_CLASS->unref(buf,file,__LINE__);
#else
    BUFFER_CLASS->unref(buf);
#endif
    }

    free(queue);
}

/**
 * 
 * @brief 
 * 
 */
AdsQueueClass*
ads_queue_class_init()
{
    static AdsQueueClass klass = {0};
    RETURN_PTR_ONCE(klass);

    klass.init = queue_array_init;
    klass.size = queue_array_size;
    klass.peek = queue_array_peek;
    klass.pop  = queue_array_pop;
    klass.push = queue_array_push;
    klass.stop = queue_array_finalize;
    return &klass;
}