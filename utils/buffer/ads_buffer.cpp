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
    std::chrono::high_resolution_clock::time_point created;
#ifdef BUFFER_TRACE

     BufferLog log;
#endif
};



AdsBuffer*
object_duplicate (AdsBuffer* obj)
{
    AdsBuffer* object = (AdsBuffer*)malloc(sizeof(AdsBuffer));
    memset(object,0,sizeof(AdsBuffer));
    memcpy(object,obj,sizeof(AdsBuffer));

    object->data = malloc(obj->size);
    memcpy(object->data,obj->data,obj->size);

    object->ref_count = 1;
    object->created = std::chrono::high_resolution_clock::now();
    return object;
}

#ifndef BUFFER_TRACE
 AdsBuffer* 
object_init (pointer data, 
                uint size,
                BufferFreeFunc free_func)
{
     AdsBuffer* object = ( AdsBuffer*)malloc(sizeof( AdsBuffer));
    memset(object,0,sizeof( AdsBuffer));

    object->data = data;
    object->free_func = free_func;
    object->size = size,
    object->ref_count = 1;
    return object;
}
pointer 
object_ref ( AdsBuffer* obj,
            int* size)
{

    if(FILTER_ERROR(obj))
        return;

    obj->ref_count++;
    if (size)
        *size = obj->size;
    
    return obj->data;
}

void    
object_unref ( AdsBuffer* obj)
{
    if(FILTER_ERROR(obj))
        return;

    obj->ref_count--;
    if (!obj->ref_count)
    {
        obj->free_func(obj->data);
        free(obj);
    }
}
#else
AdsBuffer* 
object_init (pointer data, 
                uint size,
                BufferFreeFunc free_func,
                char* file,
                int line,
                char* type)
{
    AdsBuffer* object = (AdsBuffer*)malloc(sizeof(AdsBuffer));
    memset(object,0,sizeof(AdsBuffer));
    memcpy(object->log.dataType,type,strlen(type));
    object->created = std::chrono::high_resolution_clock::now();

    object->data = data;
    object->free_func = free_func;
    object->size = size,
    object->ref_count = 1;


     log_buffer(&object->log,object->created,line,file, BufferEventType::INIT);

    return object;
}
pointer 
object_ref (AdsBuffer* obj,
            int* size,
            char* file,
            int line)
{
     log_buffer(&obj->log,obj->created,line,file, BufferEventType::REF);

    obj->ref_count++;
    if (size)
        *size = obj->size;
    
    return obj->data;
}

void    
object_unref (AdsBuffer* obj,
                char* file,
                int line)
{
     log_buffer(&obj->log,obj->created,line,file, BufferEventType::UNREF);
    
    obj->ref_count--;
    if (!obj->ref_count) {
         log_buffer(&obj->log,obj->created,line,file, BufferEventType::FREE);
        obj->free_func(obj->data);
        free(obj);
    }
}

#endif

AdsBuffer*
buffer_merge(AdsBuffer* buffer,
                AdsBuffer* inserter)
{
    uint new_size = buffer->size+inserter->size;
    pointer new_ptr = malloc(new_size);
    memcpy(new_ptr,buffer,buffer->size);
    memcpy((char*)new_ptr+buffer->size,inserter,inserter->size);
    return BUFFER_INIT(new_ptr,new_size,free);
}

uint
object_size (AdsBuffer* obj)
{
    return obj->size;
}

int64
object_create(AdsBuffer* obj)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(obj->created.time_since_epoch()).count();
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
    klass.created       = object_create;
    return &klass;
}