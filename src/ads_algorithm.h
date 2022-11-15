/**
 * @file ads_algorithm.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __ADS_ALGORITHM_H__
#define __ADS_ALGORITHM_H__

#include <ads_util.h>


typedef AdsBufferMap* (*AdsProcessFunc) (AdsBufferMap* query_result);




#define RTSP_CLIENT_RTT_BANDWIDTH_TO_BITRATE rtsp_client_rtt_bandwidth_bitrate
AdsBufferMap* rtsp_client_rtt_bandwidth_bitrate (AdsBufferMap* query_result);

#define PRINT_MEIDUM print_medium
AdsBufferMap* print_medium (AdsBufferMap* query_result);
#endif
