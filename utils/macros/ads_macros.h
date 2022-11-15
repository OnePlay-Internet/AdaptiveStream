/**
 * @file screencoder_macro.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __SUNSHINE_MACRO_H__
#define __SUNSHINE_MACRO_H__
#include <ads_util.h>

#define DEFAULT_BITRATE ( 1000 * 1000 * 30 )

#ifndef INFINITE
#define INFINITE 10000
#endif

#define RECORD_SIZE 200

#define PACKET_QUEUE_SIZE 2

#define ATOMIC_SLEEP 1ms

#define TRACE_RECORD false

#define LOG_ERROR(content)   ads_log(__FILE__,__LINE__,"error",content)

#define LOG_WARNING(content)   ads_log(__FILE__,__LINE__,"warning",content)

#define LOG_DEBUG(content)   ads_log(__FILE__,__LINE__,"debug",content)

#define LOG_INFO(content)   ads_log(__FILE__,__LINE__,"info",content)

#define RETURN_PTR_ONCE(x)  { static bool init = false; if(init) { return &x; } else { init = true; } }

#define RETURN_ONCE(x)  { static bool init = false; if(init) { return x; } else { init = true; } }

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define ADS_DATATYPE_NAME(x) ads_datatype_name(x)
char* ads_datatype_name(AdsDataType type);

#define DO_NOTHING ads_do_nothing
void ads_do_nothing(void*);

#define STRING_COMPARE(a,b) string_compare(a,b)
bool string_compare(char* a, char* b);

#define TIME_STOP timestop()
time_point timestop();

#endif