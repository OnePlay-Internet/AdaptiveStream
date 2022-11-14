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
#include <ads_cronjob.h>
#include <ads_algorithm.h>
#include <ads_util.h>
#include <ads_database.h>

#include <thread>
#define EVALUATION_INTERVAL 3s


struct Listener {
    int id;
    char event_name[50];

    AdsCallback callback;
};

struct _AdsCronJob {
    AdsDatabase* database;
    AdsProcessFunc process;

    AdsEvent* shutdown;

    std::thread thread;

    int listener_count;

    struct Listener listener[10];
};

void cronJobThread(AdsCronJob* job);

AdsCronJob*
new_cron_job(AdsDatabase* db,
             AdsProcessFunc func,
             AdsEvent* shutdown)
{
    AdsCronJob* job = (AdsCronJob*)malloc(sizeof(AdsCronJob));
    memset(job,0,sizeof(AdsCronJob));

    job->database = db;
    job->process = func;
    job->shutdown = shutdown;

    job->thread = std::thread { cronJobThread , job };
    return job;
}



void
call_listener(AdsCronJob* job,
              char* event,
              AdsBuffer* buf)
{
    for (int i = 0; i < job->listener_count; i++) {
        if (STRING_COMPARE(event,job->listener[i].event_name)) {
            job->listener[i].callback(buf);
            return;
        }
    }
}

void 
cronJobThread(AdsCronJob* job)
{
    char* keys[10];

    time_point prev,now;
    while (!IS_INVOKED(job->shutdown)) {
        now = NOW;

        AdsBufferMap* map = get_data_in_range(job->database,prev,now);
        if (!map) {
            continue;
        }
        

        /**
         * @brief 
         * process
         */
        AdsBufferMap* events = job->process(map);

        AdsBufferArray* event_names = ADS_BUFFER_MAP_CLASS->all_key(events);
        for (int i = 1; i < ADS_BUFFER_ARRAY_CLASS->length(event_names) + 1; i++) {
            AdsBuffer* buf = ADS_BUFFER_ARRAY_CLASS->n_th(event_names,i);
            char* event_name = (char*)BUFFER_REF(buf,NULL);

            AdsBuffer* event_data = ADS_BUFFER_MAP_CLASS->get(events,event_name);
            call_listener(job,event_name,buf);
            

            BUFFER_UNREF(buf);
        }
        

        ADS_BUFFER_ARRAY_CLASS->unref(event_names);

        prev = now;
        ADS_BUFFER_MAP_CLASS->unref(map);
        std::this_thread::sleep_for(EVALUATION_INTERVAL);
    }
}

void
register_listener(AdsCronJob* job,
                  AdsCallback callback,
                  char* event)
{
    job->listener[job->listener_count].id = job->listener_count;
    memcpy(job->listener[job->listener_count].event_name,event,strlen(event));
    job->listener[job->listener_count].callback = callback,

    job->listener_count++;
}