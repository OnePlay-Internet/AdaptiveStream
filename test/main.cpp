/**
 * @file main.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_context.h>

#include <thread>
#include <string.h>


static void 
callback(AdsBuffer* buf)
{
    BUFFER_REF(buf,NULL);
    printf("fired\n");

    BUFFER_UNREF(buf);
}

int main(void) {
    AdsEvent* shutdown         = NEW_EVENT;
    AdsContext* ctx = new_adaptive_context(shutdown,
                         "oneplay");
    
    AdsRecordSource* src1 = add_record_source(ctx,"rtt");
    AdsRecordSource* src2 = add_record_source(ctx,"bandwidth");
    add_listener(ctx,"bitrate",callback);


    std::thread producer1 = std::thread {
        [&](){
            for (;;) {
                nanosecond ex = NANOSEC(50000);
                pointer data = (void*)(&ex);
                ads_push_record(src1,AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND,0,data);

                SLEEP_MILLISEC(100);
            }
        }
    };
    std::thread producer2 = std::thread {
        [&](){
            for (;;) {
                nanosecond ex = NANOSEC(50000);
                pointer data = (void*)(&ex);
                ads_push_record(src2,AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND,0,data);

                SLEEP_MILLISEC(150);
            }
        }
    };
    producer2.join();
    producer1.join();
}