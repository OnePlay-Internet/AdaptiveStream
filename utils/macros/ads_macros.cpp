/**
 * @file ads_macro.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_macros.h>
#include <string.h>


void ads_do_nothing(void*){}

bool 
string_compare(char* a, 
               char* b)
{
    if (!a || !b)
        return false;
    
    int i = 0;
    if (strlen(a) != strlen(b))
        return false;

    while (*(a+i) && *(a + i) == *(b + i))
    {
        i++;
    }

    if (strlen(a) == i)
        return true;  
    else
        return false;
}

char*
ads_datatype_name(AdsDataType type){
    switch (type)
    {
    case ADS_DATATYPE_INT32:
        return "INT32";
    case ADS_DATATYPE_STRING:
        return "STRING";
    case ADS_DATATYPE_BOOLEAN:
        return "BOOLEAN";
    case ADS_DATATYPE_BYTEARRAY:
        return "BYTEARRAY";
    case ADS_DATATYPE_TIMERANGE_NANOSECOND:
        return "TIMERANGE_NANOSECOND";
    case ADS_DATATYPE_TIMEPOINT:
        return "TIMEPOINT";
    case ADS_DATATYPE_STRUCT:
        return "STRUCT";
    case ADS_DATATYPE_ERROR:
        return "ERROR";
    case ADS_DATATYPE_BUFFER:
        return "BUFFER";
    case ADS_DATATYPE_BUFFER_ARRAY:
        return "BUFFER_ARRAY";
    case ADS_DATATYPE_BUFFER_MAP:
        return "BUFFER_MAP";
    default:
        return "UNKNOWN";
    }

}

time_point
timestop()
{
    static time_point ret;
    RETURN_ONCE(ret);
    ret = NOW;
}