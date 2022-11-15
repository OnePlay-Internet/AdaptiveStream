/**
 * @file ads_event.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef  __ADS_EVENT_H__
#define  __ADS_EVENT_H__

#include <ads_util.h>

#define NEW_EVENT        new_event()
#define RAISE_EVENT(x)   raise_event(x)
#define DESTROY_EVENT(x) destroy_event(x)
#define WAIT_EVENT(x)    wait_event(x)
#define IS_INVOKED(x)    is_invoked(x)

typedef AdsQueue       AdsEvent;

AdsEvent*       new_event       ();

void            destroy_event   (AdsEvent* broadcaster);

void            raise_event     (AdsEvent* broadcaster);

bool            wait_event      (AdsEvent* broadcaster);

bool            is_invoked      (AdsEvent* broadcaster);


#endif