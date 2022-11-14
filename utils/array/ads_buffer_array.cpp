/**
 * @file ads_buffer_array.cpp
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

    BufferLL* next;
    BufferLL* prev;
};


struct _AdsBufferArray{
    int ref_count;
    int length;


    BufferLL* first;
};

AdsBuffer* 
ads_buffer_array_n_th (AdsBufferArray* arr,
                       int n)
{
    BufferLL* current = arr->first;
    if (current == NULL) {
        return NULL;
    }

    int count = 1;

    do {
        if (count == n)
            return current->buff;
        

        current = current->next;
        count++;
    }while (current != NULL);
}

 
void
ads_buffer_array_append(AdsBufferArray* arr,
                        AdsBuffer* buff)
{
    BufferLL* appendant = (BufferLL*)malloc(sizeof(BufferLL));
    appendant->next = NULL;
    appendant->buff = buff;
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

AdsBufferArray* 
ads_buffer_array_init ()
{
    AdsBufferArray* arr = (AdsBufferArray*)malloc(sizeof(AdsBufferArray));
    arr->first = NULL;
    arr->length = 0;
    arr->ref_count = 1;
    return arr;
}

void
ads_buffer_array_ref(AdsBufferArray* arr)
{
    arr->ref_count++;
}

int
ads_buffer_array_length(AdsBufferArray* arr)
{
    return arr->length;
}
 
void
ads_buffer_array_unref (AdsBufferArray* arr)
{
    arr->ref_count--;
    if (!arr->ref_count) {
        for (int i = 1; i < arr->length; i++) {
            AdsBuffer* buf = ADS_BUFFER_ARRAY_CLASS->n_th(arr,i);
            BUFFER_CLASS->unref(buf);
            free(arr);
        }
    }
}

AdsBufferArray* 
ads_buffer_array_take (AdsBufferArray* arr,
                       int first,
                       int last)
{
    if (first >= arr->length || last > arr->length) {
        return NULL;
    }
    
    AdsBufferArray* ret = ADS_BUFFER_ARRAY_CLASS->init();
    for (int i = first; i < last; i++) {
        ADS_BUFFER_ARRAY_CLASS->append(ret ,ADS_BUFFER_ARRAY_CLASS->n_th(arr,i));
    }

    return ret;
}


AdsBufferArrayClass*            
init_buffer_array_class()
{
    static AdsBufferArrayClass klass = {0};
    RETURN_PTR_ONCE(klass);
    klass.append = ads_buffer_array_append;
    klass.init = ads_buffer_array_init;
    klass.n_th = ads_buffer_array_n_th;
    klass.ref = ads_buffer_array_ref;
    klass.unref = ads_buffer_array_unref;
    klass.length = ads_buffer_array_length;

    return &klass;
}
