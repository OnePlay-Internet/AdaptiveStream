/**
 * @file ads_buffer_map.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __ADS_BUFFER_MAP_H__
#define __ADS_BUFFER_MAP_H__
#include <ads_util.h>

#define ADS_BUFFER_MAP_CLASS        init_buffer_map_class()

typedef struct _AdsBufferMap AdsBufferMap;


typedef struct _AdsBufferMapClass {
    AdsBuffer* (*get) (AdsBufferMap* arr,
                       char* key);

    AdsBuffer* (*set) (AdsBufferMap* arr,
                       AdsBuffer* buffer,
                       char* key);

    AdsBufferMap* (*init) ();

    AdsBufferArray* (*all_key) (AdsBufferMap*);

    void (*ref) (AdsBufferMap* arr);

    void (*unref) (AdsBufferMap* arr);
}AdsBufferMapClass;


AdsBufferMapClass*            init_buffer_map_class     ();

#endif
