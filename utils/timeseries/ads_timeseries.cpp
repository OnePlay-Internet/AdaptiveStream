/**
 * @file ads_timeseries_array.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_util.h>


typedef struct _BufferLL BufferLL;

struct _BufferLL {
    AdsBuffer* buff;

    time_point timestamp;

    BufferLL* next;
    BufferLL* prev;
};


struct _AdsTimeseries{
    int ref_count;
    int length;


    BufferLL* first;
};

AdsBuffer* 
ads_timeseries_array_n_th (AdsTimeseries* arr,
                           int n,
                           time_point* time)
{
    BufferLL* current = arr->first;
    if (current == NULL) {
        return NULL;
    }

    int count = 1;
    do {
        if (count == n) {
            if (time)
                *time = current->timestamp;
            
            return current->buff;
        }
        

        current = current->next;
        count++;
    }while (current != NULL);
}

 
void
ads_timeseries_array_append(AdsTimeseries* arr,
                            AdsBuffer* buff,
                            time_point time)
{
    BufferLL* appendant = (BufferLL*)malloc(sizeof(BufferLL));
    appendant->next = NULL;
    appendant->prev = NULL;
    appendant->buff = buff;
    appendant->timestamp = time;
    BUFFER_CLASS->ref(buff,NULL);

    BufferLL* current = arr->first;
    if (current == NULL) {
        arr->first = appendant;
        arr->length = 1;
        return;
    }

    while (current->next != NULL) {
        current = current->next;
    }
    
    appendant->prev = current;
    current->next = appendant;
    arr->length++;
}

AdsTimeseries* 
ads_timeseries_array_init ()
{
    AdsTimeseries* arr = (AdsTimeseries*)malloc(sizeof(AdsTimeseries));
    arr->first = NULL;
    arr->length = 0;
    arr->ref_count = 1;
    return arr;
}

int
ads_timeseries_array_length(AdsTimeseries* arr)
{
    return arr->length;
}
 
void
ads_timeseries_array_unref (AdsTimeseries* arr)
{
    arr->ref_count--;
    if (!arr->ref_count) {
        for (int i = 1; i < arr->length; i++) {
            AdsBuffer* buf = ads_timeseries_array_n_th(arr,i,NULL);
            BUFFER_CLASS->unref(buf);
        }
        free(arr);
    }
}



AdsTimeseriesClass*            
init_timeseries_array_class()
{
    static AdsTimeseriesClass klass = {0};
    RETURN_PTR_ONCE(klass);
    klass.append =  ads_timeseries_array_append;
    klass.init =    ads_timeseries_array_init;
    klass.n_th =    ads_timeseries_array_n_th;
    klass.unref =   ads_timeseries_array_unref;
    klass.length =  ads_timeseries_array_length;

    return &klass;
}
