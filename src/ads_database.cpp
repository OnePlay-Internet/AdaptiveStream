/**
 * @file ads_algorithm.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#include <ads_database.h>
#include <ads_util.h>

#include <pthread.h>

typedef struct _AdsDatabaseRecord AdsDatabaseRecord;

struct _AdsDatabaseRecord {
    /**
     * @brief 
     * key string
     */
    char key[50];
    AdsBuffer* data;

    time_point timestamp;
    AdsDatabaseRecord* next;
    AdsDatabaseRecord* prev;
};

struct _AdsDatabase {

    pthread_mutex_t mutex;
    AdsDatabaseRecord* first; 

    int length;

    int max_length;
    nanosecond timeout;

    AdsEvent* shutdown;
    std::thread clean_thread;
};


void
DatabaseCleanThread(AdsDatabase* database)
{
    while (!IS_INVOKED(database->shutdown)) {
        SLEEP_MILLISEC(1000);

        // LOG_DEBUG("cleaning database, current length: %d",database->length);

        time_point now = NOW;

        AdsDatabaseRecord* rc = database->first;
        if (rc == NULL) 
            continue;

        
        while (rc->next != NULL) {
            nanosecond lifetime = now - rc->timestamp;

            if (lifetime > database->timeout) {

                pthread_mutex_lock(&database->mutex);
                if (!rc->prev && !rc->next) {
                    database->first = NULL;
                } else if (!rc->prev && rc->next) {
                    rc->next->prev = NULL;
                    database->first = rc->next;
                } else {
                    rc->prev->next = rc->next;
                    rc->next->prev = rc->prev;
                }

                database->length--;
                pthread_mutex_unlock(&database->mutex);

                AdsDatabaseRecord* old = rc;
                rc = rc->next;
                BUFFER_UNREF(old->data);
                free(old);
            } else {
                rc = rc->next;
            }
        }

    }
}


AdsDatabase*
new_ads_database(AdsEvent* shutdown,
                 int max_length,
                 nanosecond timeout) 
{
    AdsDatabase* database = (AdsDatabase*) malloc(sizeof(AdsDatabase));
    memset(database,0,sizeof(AdsDatabase));
    database->max_length = max_length;
    database->timeout = timeout;
    database->shutdown = shutdown;
    database->mutex = PTHREAD_MUTEX_INITIALIZER;

    database->clean_thread = std::thread {DatabaseCleanThread,database};
    return database;
}



void
database_remove_last(AdsDatabase* db)
{
    pthread_mutex_lock(&db->mutex);
    db->first->next->prev = NULL;

    AdsDatabaseRecord* record = db->first;
    db->first = db->first->next;
    free((void*)record);

    db->length--;
    pthread_mutex_unlock(&db->mutex);
}

AdsDatabaseRecord*
get_nth_record(AdsDatabase* db,
               int n)
{
    if (n > db->length || db->first == NULL) {
        return NULL;
    }
    

    int count = 1;

    pthread_mutex_lock(&db->mutex);
    AdsDatabaseRecord* rc = db->first;
    while (count != n) {
        if (rc->next != NULL) {
            rc = NULL;
            goto done;
        }
        
        count++;
        rc = rc->next;
    }

done:
    pthread_mutex_unlock(&db->mutex);
    return rc;
}

void
database_push_record(AdsDatabase* db,
                     char* name,
                     AdsBuffer* data)
{
    AdsDatabaseRecord* record = (AdsDatabaseRecord*)malloc(sizeof(AdsDatabaseRecord));
    memset(record,0,sizeof(AdsDatabaseRecord));
    memcpy(record->key,name,strlen(name));

    if (db->length >= db->max_length)
        database_remove_last(db);

    BUFFER_REF(data,NULL);
    record->data = data;
    record->timestamp = NOW;
    record->next = NULL;

    pthread_mutex_lock(&db->mutex);
    AdsDatabaseRecord* rc = db->first;
    if (rc == NULL) {
        db->first = record;
        goto done;
    }
    
    while (rc->next != NULL) {
        rc = rc->next;
    }

    record->prev = rc;
    rc->next = record;
done:
    pthread_mutex_unlock(&db->mutex);
    db->length++;
    return;
}






AdsBufferMap*
get_data_in_range(AdsDatabase* db,
             time_point from,
             time_point to)
{
    AdsBufferMap* ret = NULL;

    pthread_mutex_lock(&db->mutex);
    AdsDatabaseRecord* rc = db->first;
    if (rc == NULL) 
        goto done;
    

    ret = ADS_BUFFER_MAP_CLASS->init();
    {
        int count = 0;
        while (rc->next != NULL) {
            AdsBuffer* arrbuf = NULL;
            if (!(from < rc->timestamp && rc->timestamp < to)) 
                goto next;

            arrbuf = ADS_BUFFER_MAP_CLASS->get(ret,rc->key);
            if (arrbuf) {
                AdsTimeseries* arr = (AdsTimeseries*)BUFFER_REF(arrbuf,NULL);
                ADS_TIMESERIES_CLASS->append(arr,rc->data,rc->timestamp);

                ADS_BUFFER_MAP_CLASS->set(ret,arrbuf,rc->key);
                BUFFER_UNREF(arrbuf);
            } else {
                AdsTimeseries* arr = ADS_TIMESERIES_CLASS->init();
                ADS_TIMESERIES_CLASS->append(arr,rc->data,rc->timestamp);

                AdsBuffer* insert = BUFFER_INIT(arr,0,AdsDataType::ADS_DATATYPE_BUFFER_TIMESERIES,NULL);
                ADS_BUFFER_MAP_CLASS->set(ret,insert,rc->key);
                BUFFER_UNREF(insert);
            }
            
        next: 
            rc = rc->next;
            count++;
        }

    }
    
done:
    pthread_mutex_unlock(&db->mutex);
    return ret;
}