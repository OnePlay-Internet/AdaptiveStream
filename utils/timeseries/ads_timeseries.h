/**
 * @file ads_timeseries_array.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __ADS_TIMESERIES_ARRAY_H__
#define __ADS_TIMESERIES_ARRAY_H__
#include <ads_util.h>

#define ADS_TIMESERIES_CLASS        init_timeseries_array_class()

typedef struct _AdsTimeseries AdsTimeseries;


typedef struct _AdsTimeseriesClass {
    AdsBuffer* (*n_th) (AdsTimeseries* arr,
                        int n,
                        time_point* time);

    AdsTimeseries* (*init) ();

    int (*length) (AdsTimeseries* arr);

    void (*append) (AdsTimeseries* arr,
                    AdsBuffer* buff,
                    time_point time);

    void (*unref) (AdsTimeseries* arr);
}AdsTimeseriesClass;


AdsTimeseriesClass*            init_timeseries_array_class     ();

#endif
