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

using namespace std::literals;

AdsEvent*    
new_event()
{
    return QUEUE_ARRAY_CLASS->init(INFINITE);
}

void
destroy_event(AdsEvent* event) 
{
    return QUEUE_ARRAY_CLASS->stop(event);
}

void            
raise_event(AdsEvent* broadcaster)
{
    AdsBuffer* obj = BUFFER_INIT((pointer)true,sizeof(bool),DO_NOTHING);
    QUEUE_ARRAY_CLASS->push(broadcaster,obj,false);
    BUFFER_UNREF(obj);
}

bool            
wait_event(AdsEvent* broadcaster)
{
    while (!QUEUE_ARRAY_CLASS->peek(broadcaster)) { 
        std::this_thread::sleep_for(1s); 
    }
    return true;
}

bool            
is_invoked(AdsEvent* broadcaster)
{
    return QUEUE_ARRAY_CLASS->peek(broadcaster);
}