/**
 * @file ads_event.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_event.h>
#include <ads_macros.h>
#include <thread>


AdsEvent*    
new_event()
{
    return ADS_QUEUE_CLASS->init(INFINITE);
}

void
destroy_event(AdsEvent* event) 
{
    return ADS_QUEUE_CLASS->stop(event);
}

void            
raise_event(AdsEvent* broadcaster)
{
    bool val = true;
    AdsBuffer* obj = BUFFER_CLASS->from_pointer(AdsDataType::ADS_DATATYPE_BOOLEAN,0,&val);
    ADS_QUEUE_CLASS->push(broadcaster,obj);
    BUFFER_CLASS->unref(obj);
}

bool            
wait_event(AdsEvent* broadcaster)
{
    while (!ADS_QUEUE_CLASS->peek(broadcaster)) { 
        std::this_thread::sleep_for(SEC(1)); 
    }
    return true;
}

bool            
is_invoked(AdsEvent* broadcaster)
{
    return ADS_QUEUE_CLASS->peek(broadcaster);
}