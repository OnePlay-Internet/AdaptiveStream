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
print_medium (AdsBufferMap* query_result)
{
    AdsBufferMap* ret = ADS_BUFFER_MAP_CLASS->init();

    {
        AdsBuffer* buf = NULL;
        AdsTimeseries* rtt_arr = NULL;
        int32 data;
        ADS_BUFFER_MAP_CLASS->ref(query_result);
        buf = ADS_BUFFER_MAP_CLASS->get(query_result,"rtt");
        if (buf == NULL) 
            goto donertt;

        if (BUFFER_CLASS->datatype(buf) != AdsDataType::ADS_DATATYPE_BUFFER_TIMESERIES) {
            LOG_ERROR("unknown");
        }
        
        rtt_arr = (AdsTimeseries*)BUFFER_REF(buf,NULL);

        float total = 0, total_period = 0;
        for(int i = 1 ; i < ADS_TIMESERIES_CLASS->length(rtt_arr) + 1;i++) {

            time_point time;
            AdsBuffer* element = ADS_TIMESERIES_CLASS->n_th(rtt_arr,i,&time);
            if (element == NULL)
                break;
            

            nanosecond* data = (nanosecond*)BUFFER_REF(element,NULL);

            int period = (time - TIME_STOP).count();

            total_period += period;
            total += ((*data).count() * period);
        }

        data = (float)total / (float)total_period;

        ADS_TIMESERIES_CLASS->unref(rtt_arr);
        BUFFER_UNREF(buf);

        ADS_BUFFER_MAP_CLASS->set(ret,BUFFER_CLASS->from_pointer(AdsDataType::ADS_DATATYPE_INT32,0,&data),"rtt");
    }
donertt:

    {
        AdsBuffer* bwbuf = NULL;
        AdsTimeseries* bandwidth_arr = NULL;

        int32 data;
        bwbuf = ADS_BUFFER_MAP_CLASS->get(query_result,"bandwidth");
        if (bwbuf == NULL) 
            goto donebw;

        if (BUFFER_CLASS->datatype(bwbuf) != AdsDataType::ADS_DATATYPE_BUFFER_TIMESERIES) {
            LOG_ERROR("unknown");
        }

        bandwidth_arr = (AdsTimeseries*)BUFFER_REF(bwbuf,NULL);

        float total = 0, total_period = 0;
        for(int i = 1 ; i < ADS_TIMESERIES_CLASS->length(bandwidth_arr) + 1;i++) {
            time_point time;
            AdsBuffer* element = ADS_TIMESERIES_CLASS->n_th(bandwidth_arr,i,&time);
            if (element == NULL)
                break;
            
            // START query logic
            nanosecond* data = (nanosecond*)BUFFER_REF(element,NULL);

            int period = (time - TIME_STOP).count();

            total_period += period;
            total += ((*data).count() * period);
            // END query logic
        }

        data = (float)total / (float)total_period;

        ADS_TIMESERIES_CLASS->unref(bandwidth_arr);
        ADS_BUFFER_MAP_CLASS->unref(query_result);

        ADS_BUFFER_MAP_CLASS->set(ret,BUFFER_CLASS->from_pointer(AdsDataType::ADS_DATATYPE_INT32,0,&data),"bandwidth");
    }
donebw:

    return ret;
}




AdsBufferMap* 
rtsp_client_rtt_bandwidth_bitrate(AdsBufferMap* query_result)
{
    AdsBufferMap* ret = ADS_BUFFER_MAP_CLASS->init();
    TIME_STOP;

    AdsBuffer* buf,*bwbuf = NULL;
    AdsTimeseries* bandwidth_arr,*rtt_arr = NULL;
    {
        int32 data;
        ADS_BUFFER_MAP_CLASS->ref(query_result);
        buf = ADS_BUFFER_MAP_CLASS->get(query_result,"rtt");
        if (buf == NULL) 
            goto donertt;

        if (BUFFER_CLASS->datatype(buf) != AdsDataType::ADS_DATATYPE_BUFFER_TIMESERIES) {
            LOG_ERROR("unknown");
        }
        
        rtt_arr = (AdsTimeseries*)BUFFER_REF(buf,NULL);

        float total = 0, total_period = 0;
        for(int i = 1 ; i < ADS_TIMESERIES_CLASS->length(rtt_arr) + 1;i++) {

            time_point time;
            AdsBuffer* element = ADS_TIMESERIES_CLASS->n_th(rtt_arr,i,&time);
            if (element == NULL)
                break;
            

            nanosecond* data = (nanosecond*)BUFFER_REF(element,NULL);

            int period = (time - TIME_STOP).count();

            total_period += period;
            total += ((*data).count() * period);
        }

        data = (float)total / (float)total_period;

        ADS_TIMESERIES_CLASS->unref(rtt_arr);
        BUFFER_UNREF(buf);

        ADS_BUFFER_MAP_CLASS->set(ret,BUFFER_CLASS->from_pointer(AdsDataType::ADS_DATATYPE_INT32,0,&data),"rtt");
    }
donertt:

    {
        int32 data;
        bwbuf = ADS_BUFFER_MAP_CLASS->get(query_result,"bandwidth");
        if (bwbuf == NULL) 
            goto donebw;

        if (BUFFER_CLASS->datatype(bwbuf) != AdsDataType::ADS_DATATYPE_BUFFER_TIMESERIES) {
            LOG_ERROR("unknown");
        }

        bandwidth_arr = (AdsTimeseries*)BUFFER_REF(bwbuf,NULL);

        float total = 0, total_period = 0;
        for(int i = 1 ; i < ADS_TIMESERIES_CLASS->length(bandwidth_arr) + 1;i++) {
            time_point time;
            AdsBuffer* element = ADS_TIMESERIES_CLASS->n_th(bandwidth_arr,i,&time);
            if (element == NULL)
                break;
            
            // START query logic
            nanosecond* data = (nanosecond*)BUFFER_REF(element,NULL);

            int period = (time - TIME_STOP).count();

            total_period += period;
            total += ((*data).count() * period);
            // END query logic
        }

        data = (float)total / (float)total_period;

        ADS_TIMESERIES_CLASS->unref(bandwidth_arr);
        ADS_BUFFER_MAP_CLASS->unref(query_result);

        ADS_BUFFER_MAP_CLASS->set(ret,BUFFER_CLASS->from_pointer(AdsDataType::ADS_DATATYPE_INT32,0,&data),"bandwidth");
    }
donebw:

    return ret;
}