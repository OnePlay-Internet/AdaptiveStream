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

#include <thread>
#include <cstring>
using namespace std::literals;


#define DISABLE_EVALUATION true
#define EVALUATION_INTERVAL 3s
#define LISTEN_INTERVAL 10ms
#define BUFFER_LIMIT 3ms

typedef struct _Record {
    std::chrono::nanoseconds sink_cycle;

    std::chrono::nanoseconds capture_cycle;

    std::chrono::high_resolution_clock::time_point timestamp;

    uint64 sink_queue_size;
}Record;

typedef struct _AdaptiveContext {
    AdsQueue* capture_record;
    AdsQueue* capture_event;
    AdsQueue* sink_record;
    AdsQueue* client_record;

    /**
     * @brief 
     * 
     */
    AdsEvent* shutdown;
    /**
     * @brief 
     * store metric as record
     */
    Record records[1000];
    int record_count;

    /**
     * @brief 
     * 
     */
    std::chrono::high_resolution_clock::time_point prev;

    /**
     * @brief 
     * RAW processed data
     */
    std::chrono::nanoseconds capture_delay;
}AdaptiveContext;

void
push_record(AdaptiveContext* context, 
            Record* record)
{
    for (int i = 1; i < RECORD_SIZE; i++)
    {
        context->records[i] = context->records[i-1];
    }
    memcpy((pointer)&context->records[0],(pointer)record,sizeof(Record));
}

void
median_10_record(AdaptiveContext* context, 
                 Record* record)
{
    float median_sink_cycle, median_capture_cycle, total_time, diff, total_buffer_time = 0;

    for (int i = 0; i < RECORD_SIZE; i++) {
        diff = (context->records[i].timestamp - context->prev).count(); //miliseconds
        if (diff < 0)
            continue;
        
        median_sink_cycle    += context->records[i].sink_cycle.count() * diff;
        median_capture_cycle += context->records[i].capture_cycle.count() * diff;


        total_time += diff;
    }

    record->capture_cycle = ((int)(median_capture_cycle / total_time)) * 1ns;
    record->sink_cycle =    ((int)(median_sink_cycle / total_time)) * 1ns;

}

void 
adaptiveThreadListen(AdaptiveContext* context)
{
    while (!IS_INVOKED(context->shutdown))
    {
        AdsBuffer* buf = NULL;
        Record rec; memset(&rec,0,sizeof(Record));
        bool has_sink,has_capture = false;

        std::this_thread::sleep_for(LISTEN_INTERVAL);
        if (QUEUE_ARRAY_CLASS->peek(context->sink_record)) {
            AdaptiveEvent* event = (AdaptiveEvent*) 
                QUEUE_ARRAY_CLASS->pop(context->sink_record,&buf,NULL,true);
            switch (event->code) {
            case AdaptiveRecordCode::SINK_THREAD_CYCLE:
                rec.sink_cycle = event->time_data;
                has_sink = true;
                break;
            default:
                LOG_ERROR("unknown event");
                break;
            }

            BUFFER_UNREF(buf);
        }
        
        if (QUEUE_ARRAY_CLASS->peek(context->capture_record)) {
            AdaptiveEvent* event = (AdaptiveEvent*) 
                QUEUE_ARRAY_CLASS->pop(context->capture_record,&buf,NULL,true);
            switch (event->code) {
            case AdaptiveRecordCode::CAPTURE_THREAD_CYCLE:
                rec.capture_cycle = event->time_data;
                has_capture = true;
                break;
            default:
                LOG_ERROR("unknown event");
                break;
            }

            BUFFER_UNREF(buf);
        }

        if (QUEUE_ARRAY_CLASS->peek(context->client_record)) {

        }

        if (has_sink || has_capture)
        {
            rec.timestamp = std::chrono::high_resolution_clock::now();

            if(!has_sink) 
                rec.sink_cycle = context->records[0].sink_cycle;
            if(!has_capture) 
                rec.capture_cycle = context->records[0].capture_cycle;

            push_record(context,&rec);
            context->record_count++;
        }
    }
}

void 
adaptiveThreadProcess(AdaptiveContext* context)
{
    while (!IS_INVOKED(context->shutdown))
    {
        if (context->record_count < RECORD_SIZE) {
            std::this_thread::sleep_for(EVALUATION_INTERVAL); 
            continue;
        }
        
        AdaptiveEventCode code = AdaptiveEventCode::EVENT_NONE;

        Record record;
        median_10_record(context,&record);

        if (DISABLE_EVALUATION) {
            char log[100] = {0};
            int64 cap_mili =      std::chrono::duration_cast<std::chrono::milliseconds>(record.capture_cycle).count() %1000 %1000;
            int64 cap_micro =     std::chrono::duration_cast<std::chrono::microseconds>(record.capture_cycle).count() % 1000;
            int64 sink_mili =     std::chrono::duration_cast<std::chrono::milliseconds>(record.sink_cycle).count() % 1000 % 1000;
            int64 sink_micro =    std::chrono::duration_cast<std::chrono::microseconds>(record.sink_cycle).count() % 1000;
            std::chrono::nanoseconds total_buffer_time = context->records[0].sink_queue_size * record.capture_cycle;
            int64 buffer_time =     std::chrono::duration_cast<std::chrono::milliseconds>(total_buffer_time).count() % 1000 % 1000;
            snprintf(log,100,"capture cycle %dms,%dus | sink sycle %dms,%dus | total buffer time %dms",cap_mili,cap_micro,sink_mili,sink_micro,buffer_time);
            LOG_INFO(log);
        }

        context->prev = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(EVALUATION_INTERVAL);
    }
}

void 
newAdaptiveControlThread (AdsEvent* shutdown,
                        AdsQueue* capture_record,
                        AdsQueue* capture_event,
                        AdsQueue* sink_record,
                        AdsQueue* client_record)
{
    AdaptiveContext context;
    memset(&context,0,sizeof(AdaptiveContext));
    context.prev = std::chrono::high_resolution_clock::now();
    context.capture_record = capture_record;
    context.capture_event = capture_event;
    context.sink_record = sink_record;
    context.client_record = client_record;

    context.record_count = 0;
    context.shutdown = shutdown;

    std::thread listen { adaptiveThreadListen, &context };
    listen.detach();
    std::thread process { adaptiveThreadProcess, &context };
    process.detach();
    WAIT_EVENT(context.shutdown);
    std::this_thread::sleep_for(1s);
}


void 
newAdaptiveControl (AdsEvent* shutdown,
                    AdsQueue* capture_record,
                    AdsQueue* capture_event,
                    AdsQueue* sink_record,
                    AdsQueue* client_record
                    )
{
    std::thread thread { newAdaptiveControlThread, 
                    shutdown,
                    capture_record,
                    capture_event,
                    sink_record,
                    client_record };
    thread.detach();
}
