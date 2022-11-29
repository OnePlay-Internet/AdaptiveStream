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
callback1(AdsBuffer* buf)
{
    int* data = (int*)BUFFER_REF(buf,NULL);
    if(*data)
        printf("fired bandwidth %d\n",*data);
    else
        printf("fired bandwidth %d\n",*data);

    BUFFER_UNREF(buf);
}
static void 
callback2(AdsBuffer* buf)
{
    int* data = (int*)BUFFER_REF(buf,NULL);
    if(*data)
        printf("fired rtt %d\n",*data);
    else
        printf("fired rtt %d\n",*data);

    BUFFER_UNREF(buf);
}

int main(void) {
    AdsEvent* shutdown         = NEW_EVENT;
    AdsContext* ctx = new_adaptive_context(shutdown,
                         "print",NULL);
    
    AdsRecordSource* src1 = add_record_source(ctx,"rtt");
    AdsRecordSource* src2 = add_record_source(ctx,"bandwidth");

    add_listener_to_ctx(ctx,"bandwidth",callback1,NULL);
    add_listener_to_ctx(ctx,"rtt",callback2,NULL);


    auto producertt = [&](){
        int i = 50;
        for (;;) {
            nanosecond ex = NANOSEC(i);
            pointer data = (void*)(&ex);
            AdsRecordSource* src = get_record_source(ctx,"rtt");
            ads_push_record(src,AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND,0,data);

            SLEEP_MILLISEC(1000);
            i+=5;
        }
    };
    auto producebandwidth = [&](){
        int i = 1000;
        for (;;) {
            nanosecond ex = NANOSEC(i);
            pointer data = (void*)(&ex);
            AdsRecordSource* src = get_record_source(ctx,"bandwidth");
            ads_push_record(src,AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND,0,data);

            SLEEP_MILLISEC(1500);
            i+=50;
        }
    };

    std::thread producer1 = std::thread { producertt };
    std::thread producer2 = std::thread { producebandwidth };
    std::thread producer3 = std::thread { producertt };

    producer3.join();
    producer2.join();
    producer1.join();
}