/**
 * @file ads_buffer_array.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __ADS_BUFFER_ARRAY_H__
#define __ADS_BUFFER_ARRAY_H__
#include <ads_util.h>

#define ADS_BUFFER_ARRAY_CLASS        init_buffer_array_class()

typedef struct _AdsBufferArray AdsBufferArray;


typedef struct _AdsBufferArrayClass {
    AdsBuffer* (*n_th) (AdsBufferArray* arr,
                        int n);

    AdsBufferArray* (*init) ();

    int (*length) (AdsBufferArray* arr);

    void (*ref) (AdsBufferArray* arr);

    void (*append) (AdsBufferArray* arr,
                 AdsBuffer* buff);

    void (*unref) (AdsBufferArray* arr);

    AdsBufferArray* (*take) (int first,
                             int last);
}AdsBufferArrayClass;


AdsBufferArrayClass*            init_buffer_array_class     ();

#endif
