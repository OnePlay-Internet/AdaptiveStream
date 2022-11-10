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


typedef enum _AdaptiveRecordCode {
    CLIENT_BANDWIDTH,
    CLIENT_RTT,

    SINK_THREAD_CYCLE,
    CAPTURE_THREAD_CYCLE,
}AdaptiveRecordCode;

typedef enum _AdaptiveEventCode {
    BITRATE_CHANGE ,
    FRAMERATE_CHANGE ,
}AdaptiveEventCode;

typedef struct _AdaptiveRecord {
    AdaptiveRecordCode code;
    std::chrono::nanoseconds time_data[10];
    int num_data[10];
}AdaptiveRecord;

typedef struct _AdaptiveEvent {
    AdaptiveEventCode code;
    std::chrono::nanoseconds time_data[10];
    int num_data[10];
}AdaptiveEvent;


void newAdaptiveControl (AdsEvent* shutdown,
                         AdsQueue* capture_record,
                         AdsQueue* capture_event,
                         AdsQueue* sink_record,
                         AdsQueue* client_record
                         );

#endif