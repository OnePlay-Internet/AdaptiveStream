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

#define ADS_QUEUE_CLASS        ADS_QUEUE_CLASS_init()

typedef struct _QueueArray AdsQueue;

typedef struct _AdsQueueClass{
    bool (*push) (AdsQueue* queue,  AdsBuffer* data, bool record);

    bool (*peek) (AdsQueue* queue);

    pointer       (*pop) (AdsQueue* queue, 
                             AdsBuffer** buf,
                            int* size,
                            bool record);
    AdsQueue* (*init) (int size);

    uint64 (*size) (AdsQueue* queue);

    void (*stop) (AdsQueue* queue);
} AdsQueueClass;

AdsQueueClass*        ADS_QUEUE_CLASS_init      ();



#endif