/**
 * @file ads_error.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef  __ADS_LOG_H__
#define  __ADS_LOG_H__


#include <ads_util.h>

typedef enum _BufferEventType {
    NONE,
    INIT,
    REF, 
    UNREF,
    FREE,
}BufferEventType;

/**
 * @brief 
 * 
 * @param file 
 * @param line 
 * @param level 
 * @param message 
 */
void              ads_log                   (char* file,
                                             int line,
                                             char* level,
                                             char* message);

/**
 * @brief 
 * 
 * @param log 
 * @param created 
 * @param line 
 * @param file 
 * @param type 
 */
void             log_buffer                 (AdsDataType datatype,
                                             time_point created,
                                             int line,
                                             char* file,
                                             BufferEventType type);







#endif