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
#ifndef __ADS_LISTENER_H__
#define __ADS_LISTENER_H__

#include <ads_util.h>
#include <ads_context.h>
#include <ads_database.h>





/**
 * @brief 
 * 
 * @param name 
 * @param shutdown 
 * @param db 
 * @return AdsListener* 
 */
AdsRecordSource* new_ads_listener       (char* name,
                                        AdsEvent* shutdown,
                                        AdsDatabase* db);

char* ads_record_source_get_name        (AdsRecordSource* source);

#endif