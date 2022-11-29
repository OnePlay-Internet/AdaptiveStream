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
#include <ads_util.h>

#include <ads_database.h>
#include <ads_algorithm.h>


#include <thread>

typedef struct _AdsCronJob AdsCronJob;



AdsCronJob* new_cron_job    (AdsDatabase* db,
                             AdsProcessFunc func,
                             AdsEvent* shutdown);

int register_listener      (AdsCronJob* job,
                             AdsCallback callback,
                             char* event,
                             void* data);

void remove_listener        (AdsCronJob* job,
                            int id);