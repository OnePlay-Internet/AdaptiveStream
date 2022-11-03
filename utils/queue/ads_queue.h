/**
 * @file ads_queue.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __ADS_QUEUE_H__
#define __ADS_QUEUE_H__


#include <ads_buffer.h>

#define QUEUE_ARRAY_CLASS        queue_array_class_init()

typedef struct _QueueArray AdsQueue;

typedef struct _QueueArrayClass{
    bool (*push) (AdsQueue* queue,  Buffer* data, bool record);

    bool (*peek) (AdsQueue* queue);

    pointer       (*pop) (AdsQueue* queue, 
                             Buffer** buf,
                            int* size,
                            bool record);
    AdsQueue* (*init) (int size);

    uint64 (*size) (AdsQueue* queue);

    void (*stop) (AdsQueue* queue);
} QueueArrayClass;

QueueArrayClass*        queue_array_class_init      ();



#endif