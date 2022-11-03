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

typedef struct _Record {
    std::chrono::nanoseconds sink_cycle;

    std::chrono::nanoseconds capture_cycle;

    std::chrono::high_resolution_clock::time_point timestamp;

    uint64 sink_queue_size;
}Record;


typedef enum _AdaptiveEventCode {
    EVENT_NONE = 0,

    UPDATE_CAPTURE_DELAY_INTERVAL = 2,
    DISABLE_CAPTURE_DELAY_INTERVAL = 4,

    SINK_CYCLE_REPORT = 8,
    CAPTURE_CYCLE_REPORT = 16,
    CAPTURE_TIMEOUT = 32,

    AVCODEC_BITRATE_CHANGE = 64,
    AVCODEC_FRAMERATE_CHANGE = 128,
}AdaptiveEventCode;

typedef struct _AdaptiveEvent {
    AdaptiveEventCode code;
    std::chrono::nanoseconds time_data;
    int num_data;
}AdaptiveEvent;



void newAdaptiveControl (AdsEvent* shutdown,
                         AdsQueue* sink_queue,
                         AdsQueue* capture_event_in,
                         AdsQueue* capture_event_out,
                         AdsQueue* sink_event_in,
                         AdsQueue* sink_event_out);

#endif