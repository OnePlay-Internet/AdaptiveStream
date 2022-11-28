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
#include <ads_context.h>

#include <ads_database.h>
#include <ads_listener.h>
#include <ads_cronjob.h>

#include <ads_util.h>

#include <thread>
#include <cstring>


#define DISABLE_EVALUATION true
#define BUFFER_LIMIT 3ms






struct _AdsContext {
    /**
     * @brief 
     * 
     */
    AdsEvent* shutdown;
    /**
     * @brief 
     * store metric as record
     */
    AdsDatabase* database;

    AdsCronJob* job;


    int listener_count;
    AdsRecordSource* listeners[10];
};



AdsContext* 
new_adaptive_context(AdsEvent* shutdown,
                     char* application,
                     AdsProcessFunc algorithm)
{
    if (algorithm == NULL) {
        if (STRING_COMPARE(application,"rtsp")) {
            algorithm = RTSP_CLIENT_RTT_BANDWIDTH_TO_BITRATE;
        } else if (STRING_COMPARE(application,"print")) {
            algorithm = PRINT_MEIDUM;
        } else {
            return NULL;
        }
    }
    

    AdsContext* context = (AdsContext*)malloc(sizeof(AdsContext));
    memset(context,0,sizeof(AdsContext));

    context->shutdown = shutdown;
    context->database = new_ads_database(shutdown,10000,SEC(600));
    context->job = new_cron_job(context->database,algorithm,shutdown);
    
    return context;
}



AdsRecordSource*
add_record_source(AdsContext* context,
             char* name)
{
    context->listeners[context->listener_count] = new_ads_listener(name,context->shutdown,context->database);
    context->listener_count++;
    return context->listeners[context->listener_count - 1];
}


AdsRecordSource*
add_listener(AdsContext* context,
             char* name,
             AdsCallback func)
{
    register_listener(context->job,func,name);
}