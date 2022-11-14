/**
 * @file ads_buffer_map.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_util.h>
#include <pthread.h>


typedef struct _BufferLL BufferLL;

struct _BufferLL {
    char key[50];
    AdsBuffer* buff;

    BufferLL* next;
    BufferLL* prev;
};


struct _AdsBufferMap{
    pthread_mutex_t mutex;

    int ref_count;
    BufferLL* first;
};

AdsBuffer* 
ads_buffer_map_get(AdsBufferMap* arr,
                   char* key)
{
    AdsBuffer* ret = NULL;

    pthread_mutex_lock(&arr->mutex);
    BufferLL* current = arr->first;
    if (current == NULL) 
        goto done;

    do {
        if (STRING_COMPARE(current->key,key)) {
            ret = current->buff;

            if (!current->prev && !current->next) {
                arr->first = NULL;
            } else if (!current->prev && current->next) {
                current->next->prev = NULL;
                arr->first = current->next;
            } else if (!current->next) {
                current->prev->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            free(current);
            goto done;
        }

        current = current->next;
    } while (current != NULL);

done:
    pthread_mutex_unlock(&arr->mutex);
    return ret;
}

 
AdsBuffer* 
ads_buffer_map_set(AdsBufferMap* arr,
                   AdsBuffer* buff,
                   char* key)
{
    AdsBuffer* ret = NULL;

    BufferLL* appendant = (BufferLL*)malloc(sizeof(BufferLL));
    memset(appendant,0,sizeof(BufferLL));
    memcpy(appendant->key,key,strlen(key));

    appendant->buff = buff;
    BUFFER_CLASS->ref(buff,NULL);

    pthread_mutex_lock(&arr->mutex);
    BufferLL* current = arr->first;
    if (current == NULL) {
        arr->first = appendant;
        goto done;
    }

    

    while (current->next != NULL) {
        if (STRING_COMPARE(current->key,key)) {
            AdsBuffer* ret = current->buff;
            current->buff = buff;

            free(appendant);
            goto done;
        }

        current = current->next;
    }
    
    appendant->prev = current;
    current->next = appendant;

done:
    pthread_mutex_unlock(&arr->mutex);
    return ret;
}

AdsBufferMap* 
ads_buffer_map_init ()
{
    AdsBufferMap* arr = (AdsBufferMap*)malloc(sizeof(AdsBufferMap));
    arr->first = NULL;
    arr->ref_count = 1;
    arr->mutex = PTHREAD_MUTEX_INITIALIZER;
    return arr;
}

void
ads_buffer_map_ref(AdsBufferMap* arr)
{
    arr->ref_count++;
}

AdsBufferArray*
ads_buffer_map_all_key(AdsBufferMap* arr)
{
    AdsBufferArray* ret = ADS_BUFFER_ARRAY_CLASS->init();

    pthread_mutex_lock(&arr->mutex);
    BufferLL* current = arr->first;
    if (current == NULL) {
        arr->first = current;
        goto done;
    }

    

    do {
        char* data = NULL;
        AdsBuffer* buf = NULL;
        for (int i = 1; i < ADS_BUFFER_ARRAY_CLASS->length(ret); i++) {
            AdsBuffer* temp = ADS_BUFFER_ARRAY_CLASS->n_th(ret,i);
            char* val = (char*)BUFFER_REF(temp,NULL);
            if (STRING_COMPARE(current->key,val)) {
                goto next;
            }
            BUFFER_UNREF(temp);
        }
        

        data = (char*)malloc(strlen(current->key) + 1);
        memcpy(data,current->key,strlen(current->key) + 1);

        buf = BUFFER_CLASS->init(data,0,AdsDataType::ADS_DATATYPE_STRING,NULL);
        ADS_BUFFER_ARRAY_CLASS->append(ret,buf);
        BUFFER_UNREF(buf);

    next: 
        current = current->next;
    } while (current != NULL);

done:
    pthread_mutex_unlock(&arr->mutex);
    return ret;
}
 
void
ads_buffer_map_unref (AdsBufferMap* arr)
{
    pthread_mutex_lock(&arr->mutex);
    arr->ref_count--;
    if (!arr->ref_count) {
        BufferLL* current = arr->first;
        if (current == NULL) 
            goto done;
        

        while (current->next != NULL) {
            BUFFER_CLASS->unref(current->buff);
            current = current->next;
        }
    }
done:
    pthread_mutex_unlock(&arr->mutex);
}



AdsBufferMapClass*            
init_buffer_map_class()
{
    static AdsBufferMapClass klass = {0};
    RETURN_PTR_ONCE(klass);

    klass.all_key = ads_buffer_map_all_key;
    klass.get = ads_buffer_map_get;
    klass.init = ads_buffer_map_init;
    klass.ref = ads_buffer_map_ref;
    klass.set = ads_buffer_map_set;
    klass.unref = ads_buffer_map_unref;
    return &klass;
}
