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
#define EVALUATION_INTERVAL 10s
#define LISTENER_MAX_COUNT 10


struct Listener {
    int id;
    char event_name[50];
    void* data;

    AdsCallback callback;
};

struct _AdsCronJob {
    AdsDatabase* database;
    AdsProcessFunc process;

    AdsEvent* shutdown;

    std::thread thread;

    int listener_count;
    struct Listener listener[LISTENER_MAX_COUNT];
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
    job->listener_count = 1;

    job->thread = std::thread { cronJobThread , job };
    return job;
}



void
call_listener(AdsCronJob* job,
              char* event,
              AdsBuffer* buf)
{
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (STRING_COMPARE(event,job->listener[i].event_name)) {
            job->listener[i].callback(buf,job->listener[i].data);
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
        std::this_thread::sleep_for(EVALUATION_INTERVAL);
        now = NOW;

        AdsBufferMap* map = get_data_in_range(job->database,prev,now);
        if (!map) 
            continue;
        
        

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
            call_listener(job,event_name,event_data);
            

            BUFFER_UNREF(buf);
        }
        

        ADS_BUFFER_ARRAY_CLASS->unref(event_names);

        prev = now;
        ADS_BUFFER_MAP_CLASS->unref(map);
    }
}

int
register_listener(AdsCronJob* job,
                  AdsCallback callback,
                  char* event,
                  void* user_data)
{
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (job->listener[i].id == 0) {

            memcpy(job->listener[i].event_name,event,strlen(event));
            job->listener[i].id = job->listener_count;
            job->listener[i].callback = callback;
            job->listener[i].data = user_data;


            job->listener_count++;
            return job->listener[i].id;
        }
    }
}


void
remove_listener(AdsCronJob* job,
                int id)
{
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (job->listener[i].id == id) {
            memset(&job->listener[i],0,sizeof(Listener));
            return;
        }
    }
}