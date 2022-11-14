/**
 * @file screencoder_adaptive.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-09-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_listener.h>
#include <ads_context.h>

#include <ads_database.h>

#include <ads_util.h>

#include <thread>
#define LISTEN_INTERVAL 100ms

struct _AdsRecordSource {
    char name[50];

    AdsQueue* queue;
    std::thread thread;

    AdsEvent* shutdown;
    AdsDatabase* db;
};

void adaptiveThreadListen(AdsRecordSource* lis);

AdsRecordSource*
new_ads_listener(char* name,
                 AdsEvent* shutdown,
                 AdsDatabase* db)
{
    AdsRecordSource* listener = (AdsRecordSource*)malloc(sizeof(AdsRecordSource));
    memset(listener,0,sizeof(AdsRecordSource));

    memcpy(listener->name,name,strlen(name));
    listener->shutdown = shutdown;
    listener->db = db;
    listener->queue = ADS_QUEUE_CLASS->init(INFINITE);


    listener->thread = std::thread { adaptiveThreadListen,listener };
    return listener;
}


void
adaptiveThreadListen(AdsRecordSource* lis)
{
    int size;
    AdsBuffer* buf = NULL;
    while (!IS_INVOKED(lis->shutdown))
    {
        std::this_thread::sleep_for(LISTEN_INTERVAL);
        if (ADS_QUEUE_CLASS->peek(lis->queue)) {
            ADS_QUEUE_CLASS->pop(lis->queue,&buf,&size);
            database_push_record(lis->db,lis->name,buf);
            BUFFER_UNREF(buf);
        }
    }
}


void
ads_push_record(AdsRecordSource* lis,
                         AdsDataType type,
                         int size,
                         void* data)
{
    AdsBuffer* buf = BUFFER_CLASS->from_pointer(type,size,data);
    ADS_QUEUE_CLASS->push(lis->queue,buf);
}