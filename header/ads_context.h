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
#ifndef __ADS_CONTEXT_H__
#define __ADS_CONTEXT_H__

#include <ads_util.h>

typedef struct _AdsRecordSource AdsRecordSource;

typedef struct _AdsContext AdsContext;

typedef void (*AdsCallback) (AdsBuffer* buf);


/**
 * @brief 
 * 
 * @param shutdown 
 * @param application 
 * @return AdaptiveContext* 
 */
AdsContext*         new_adaptive_context            (AdsEvent* shutdown,
                                                    char* application,
                                                    AdsProcessFunc function);

AdsRecordSource*    add_listener                    (AdsContext* context,
                                                     char* name,
                                                     AdsCallback func);

AdsRecordSource*    add_record_source               (AdsContext* context,
                                                     char* name);

void                ads_push_record                 (AdsRecordSource* lis,
                                                     AdsDataType type,
                                                     int size,
                                                     void* data);
#endif