/**
 * @file object.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ads_buffer.h>
#include <ads_datatype.h>
#include <ads_macros.h>
#include <cstdlib>
#include <string.h>
#include <mutex>





typedef struct _AdsBuffer{
    uint ref_count;

    /**
     * @brief 
     * should not be used directly
     */
    BufferFreeFunc free_func;

    /**
     * @brief 
     * should not be used directly
     */
    uint size;

    /**
     * @brief 
     * should not be used directly
     */
    pointer data;

    /**
     * @brief 
     * 
     */
    time_point created;



    AdsDataType datatype;
    bool allow_duplicate;

#ifdef BUFFER_TRACE
    /**
     * @brief 
     * 
     */
    BufferLog log;
#endif
};



AdsBuffer*
object_duplicate (AdsBuffer* obj)
{
    if (!obj->allow_duplicate)
        return NULL;
    
    AdsBuffer* object = (AdsBuffer*)malloc(sizeof(AdsBuffer));
    memset(object,0,sizeof(AdsBuffer));
    memcpy(object,obj,sizeof(AdsBuffer));

    object->data = malloc(obj->size);
    memcpy(object->data,obj->data,obj->size);

    object->ref_count = 1;
    object->created = NOW;
    return object;
}


AdsBuffer* 
object_init (pointer data, 
             uint size,
             AdsDataType datatype,
             BufferFreeFunc free_func
#ifdef BUFFER_TRACE
             ,char* file,
             int line,
             char* type
#endif
             )
{
    AdsBuffer* object = (AdsBuffer*)malloc(sizeof(AdsBuffer));
    memset(object,0,sizeof(AdsBuffer));
    object->created = NOW;

    object->data = data;
    object->free_func = free_func;
    object->ref_count = 1;
    object->datatype = datatype;
    object->allow_duplicate = true;


    switch (datatype)
    {
    case AdsDataType::ADS_DATATYPE_BOOLEAN :
        free_func = free;
        size = sizeof(bool);
        break;
    case AdsDataType::ADS_DATATYPE_INT32 :
        free_func = free;
        size = sizeof(int32);
        break;
    case AdsDataType::ADS_DATATYPE_TIMEPOINT :
        free_func = free;
        size = sizeof(time_point);
        break;
    case AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND :
        free_func = free;
        size = sizeof(nanosecond);
        break;
    case AdsDataType::ADS_DATATYPE_STRING :
        free_func = free;
        size = strlen((char*)data) + 1;
        break;

    case AdsDataType::ADS_DATATYPE_STRUCT :
    case AdsDataType::ADS_DATATYPE_BYTEARRAY :
        free_func = free;
        break;

    case AdsDataType::ADS_DATATYPE_BUFFER_ARRAY:
        free_func = (BufferFreeFunc)ADS_BUFFER_ARRAY_CLASS->unref;
    case AdsDataType::ADS_DATATYPE_BUFFER_MAP:
        free_func = (BufferFreeFunc)ADS_BUFFER_MAP_CLASS->unref;
    case AdsDataType::ADS_DATATYPE_BUFFER:
        free_func = (BufferFreeFunc)BUFFER_CLASS->unref;
        object->allow_duplicate = false;
        size = 0;
        break;

    default:
        LOG_ERROR("unknown datatype");
        return NULL;
    }


#ifdef BUFFER_TRACE
    memcpy(object->log.dataType,type,strlen(type));
    log_buffer(obj->datatype,object->created,line,file, BufferEventType::INIT);
#endif

    object->size = size;
    return object;
}
pointer 
object_ref (AdsBuffer* obj,
            int* size
#ifdef BUFFER_TRACE
            ,char* file,
            int line
#endif
            )
{
#ifdef BUFFER_TRACE
    log_buffer(obj->datatype,obj->created,line,file, BufferEventType::REF);
#endif

    obj->ref_count++;
    if (size)
        *size = obj->size;
    
    return obj->data;
}

void    
object_unref (AdsBuffer* obj
#ifdef BUFFER_TRACE
              ,char* file,
              int line
#endif
              )
{

#ifdef BUFFER_TRACE
    log_buffer(obj->datatype,obj->created,line,file, BufferEventType::UNREF);
#endif
    
    obj->ref_count--;
    if (!obj->ref_count) {
#ifdef BUFFER_TRACE
        log_buffer(obj->datatype,obj->created,line,file, BufferEventType::FREE);
#endif
        obj->free_func(obj->data);
        free(obj);
    }
}



uint
object_size (AdsBuffer* obj)
{
    return obj->size;
}

int64
object_create(AdsBuffer* obj)
{
    return std::chrono::duration_cast<nanosecond>(obj->created.time_since_epoch()).count();
}


AdsDataType
object_get_datatype(AdsBuffer* obj) {
    return obj->datatype;
}




AdsBuffer* 
object_from_pointer(AdsDataType type,
                    int optional_size,
                    pointer ptr)
{
    int size;
    switch (type)
    {
    case AdsDataType::ADS_DATATYPE_BOOLEAN :
        size = sizeof(bool);
        break;
    case AdsDataType::ADS_DATATYPE_INT32 :
        size = sizeof(int32);
        break;
    case AdsDataType::ADS_DATATYPE_TIMEPOINT :
        size = sizeof(time_point);
        break;
    case AdsDataType::ADS_DATATYPE_TIMERANGE_NANOSECOND :
        size = sizeof(nanosecond);
        break;
    case AdsDataType::ADS_DATATYPE_STRING :
        size = strlen((char*)ptr) + 1;
        break;

    case AdsDataType::ADS_DATATYPE_STRUCT :
    case AdsDataType::ADS_DATATYPE_BYTEARRAY :
        size = optional_size;
        break;

    case AdsDataType::ADS_DATATYPE_BUFFER_ARRAY:
    case AdsDataType::ADS_DATATYPE_BUFFER_MAP:
    case AdsDataType::ADS_DATATYPE_BUFFER:
        LOG_ERROR("buffer from pointer is banned");
        return NULL;

    default:
        LOG_ERROR("unknown datatype");
        return NULL;
    }

    if (size <= 0) {
        LOG_ERROR("size must be larger than 0");
        return NULL;
    }
    

    pointer buff_ptr = malloc(size);
    memset(buff_ptr,0,size);
    memcpy(buff_ptr,ptr,size);
    return object_init(buff_ptr,size,type,free);
}


BufferClass*
object_class_init()
{
    static BufferClass klass = {0};
    RETURN_PTR_ONCE(klass);

    klass.init          = object_init;
    klass.unref         = object_unref;
    klass.ref           = object_ref;
    klass.duplicate     = object_duplicate;
    klass.size          = object_size;
    klass.datatype      = object_get_datatype;
    klass.from_pointer  = object_from_pointer;
    return &klass;
}