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
#ifndef __ADS_DATABASE_H__
#define __ADS_DATABASE_H__
#include <ads_util.h>


typedef struct _AdsDatabase AdsDatabase;

AdsDatabase*    new_ads_database            (AdsEvent* shutdown,
                                             int max_length,
                                             nanosecond timeout);

void            database_push_record        (AdsDatabase* db,
                                             char* name,
                                             AdsBuffer* data);

AdsBufferMap* get_data_in_range            (AdsDatabase* db,
                                              time_point from,
                                              time_point to);
#endif