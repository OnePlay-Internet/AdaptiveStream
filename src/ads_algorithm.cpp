/**
 * @file ads_algorithm.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ads_algorithm.h>
#include <ads_util.h>

AdsBufferMap* 
rtsp_client_rtt_bandwidth_bitrate (AdsBufferMap* query_result)
{
    AdsBufferMap* ret = ADS_BUFFER_MAP_CLASS->init();

    int* data = NULL;
    AdsBuffer* buf,*bwbuf = NULL;
    AdsBufferArray* bandwidth_arr,*rtt_arr = NULL;
    {
        ADS_BUFFER_MAP_CLASS->ref(query_result);
        buf = ADS_BUFFER_MAP_CLASS->get(query_result,"rtt");
        if (buf == NULL) 
            goto done;

        if (BUFFER_CLASS->datatype(buf) != AdsDataType::ADS_DATATYPE_BUFFER_ARRAY) {
            LOG_ERROR("unknown");
        }
        
        rtt_arr = (AdsBufferArray*)BUFFER_REF(buf,NULL);
        ADS_BUFFER_ARRAY_CLASS->ref(rtt_arr);


        nanosecond total;
        for(int i = 1 ; i < ADS_BUFFER_ARRAY_CLASS->length(rtt_arr) + 1;i++) {
            AdsBuffer* element = ADS_BUFFER_ARRAY_CLASS->n_th(rtt_arr,i);
            if (element == NULL)
                break;
            

            nanosecond* data = (nanosecond*)BUFFER_REF(element,NULL);
            total += *data;
        }


        ADS_BUFFER_ARRAY_CLASS->unref(rtt_arr);
        BUFFER_UNREF(buf);
    }


    {
        bwbuf = ADS_BUFFER_MAP_CLASS->get(query_result,"bandwidth");
        if (bwbuf == NULL) 
            goto done;

        if (BUFFER_CLASS->datatype(bwbuf) != AdsDataType::ADS_DATATYPE_BUFFER_ARRAY) {
            LOG_ERROR("unknown");
        }

        bandwidth_arr = (AdsBufferArray*)BUFFER_REF(bwbuf,NULL);
        ADS_BUFFER_ARRAY_CLASS->ref(bandwidth_arr);


        ADS_BUFFER_ARRAY_CLASS->unref(bandwidth_arr);
        ADS_BUFFER_MAP_CLASS->unref(query_result);
    }

    data = (int*)malloc(sizeof(int));
    *data = 10;
    ADS_BUFFER_MAP_CLASS->set(ret,BUFFER_INIT(data,0,AdsDataType::ADS_DATATYPE_INT32,NULL),"bitrate");
done:
    return ret;
}