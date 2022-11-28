/**
 * @file ads_datatype.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef  __ADS_DATATYPE_H__
#define  __ADS_DATATYPE_H__



typedef unsigned char uint8;
typedef unsigned int  uint;
typedef int           int32;
typedef unsigned int  uint32;
typedef unsigned long long uint64;
typedef long long     int64;
typedef void*         pointer;


#include <chrono>
#include <thread>
using namespace std::literals::chrono_literals;

typedef std::chrono::high_resolution_clock::time_point time_point;
typedef std::chrono::nanoseconds nanosecond;
typedef std::chrono::seconds second;
typedef std::chrono::milliseconds millisecond;
typedef std::chrono::microseconds microsecond;
typedef std::chrono::minutes minute;

#define SLEEP_MILLISEC(x)   std::this_thread::sleep_for(x * 1ms)
#define NANOSEC(x)   x * 1ns
#define MILLISEC(x)   x * 1ms
#define SEC(x)   x * 1s
#define NOW std::chrono::high_resolution_clock::now()


typedef enum _AdsDataType {
    ADS_DATATYPE_FLOAT,
    ADS_DATATYPE_INT32,
    ADS_DATATYPE_STRING,
    ADS_DATATYPE_BOOLEAN,
    ADS_DATATYPE_BYTEARRAY,
    ADS_DATATYPE_TIMERANGE_NANOSECOND,
    ADS_DATATYPE_TIMEPOINT,
    ADS_DATATYPE_STRUCT,
    ADS_DATATYPE_ERROR,
    ADS_DATATYPE_BUFFER,
    ADS_DATATYPE_BUFFER_ARRAY,
    ADS_DATATYPE_BUFFER_TIMESERIES,
    ADS_DATATYPE_BUFFER_MAP,
}AdsDataType;




#endif