/**
 * @file queue.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef  __ADS_BUFFER_H__
#define  __ADS_BUFFER_H__

#include <ads_datatype.h>
#include <string>
#include <typeinfo>
#include <cstring>

// #define BUFFER_TRACE
#define BUFFER_CLASS          object_class_init() 
#ifdef BUFFER_TRACE

#define BUFFER_REF(obj,size)                         object_class_init()->ref(obj,size,__FILE__,__LINE__)

#define BUFFER_UNREF(obj)                            object_class_init()->unref(obj,__FILE__,__LINE__)

#define BUFFER_INIT(obj,size,free)                   object_class_init()->init(obj,size,free,__FILE__,__LINE__,(char*)typeid(obj).name())
#else
#define BUFFER_REF(obj,size)                         object_class_init()->ref(obj,size)

#define BUFFER_UNREF(obj)                            object_class_init()->unref(obj)

#define BUFFER_INIT(obj,size,type,free)              object_class_init()->init(obj,size,type,free)
#endif





/**
 * @brief 
 * x: object name
 * y: object data size
 * z: object data pointer
 */
#define BUFFER_MALLOC(x,y,type,z) pointer z = (pointer)malloc( y );  \
                             memset(z,0,y); \
                              AdsBuffer* x = BUFFER_INIT(z,y,type,free) 


/**
 * @brief 
 * x: object name
 * y: object data size
 * z: object data source
 */
#define BUFFER_DUPLICATE(x,y,z,ptr) pointer ptr = (pointer)malloc( y );  \
                                    memcpy(ptr,z,y); \
                                     AdsBuffer* x = BUFFER_INIT(ptr,y,free) \




typedef void (*BufferFreeFunc) (pointer data);

typedef struct _AdsBuffer AdsBuffer;



typedef struct _BufferClass {
    pointer (*ref)      (AdsBuffer* obj,
                            int* size
#ifdef BUFFER_TRACE
                            ,char* file
                            ,int line
#endif
                            );

    void    (*unref)    (AdsBuffer* obj
#ifdef BUFFER_TRACE
                            ,char* file
                            ,int line
#endif
                            );

    AdsBuffer* (*init)     (pointer data,
                            uint size,
                            AdsDataType datatype,
                            BufferFreeFunc func
#ifdef BUFFER_TRACE
                            ,char* file
                            ,int line
                            ,char* type
#endif
                            );

    AdsBuffer* (*duplicate)(AdsBuffer* obj);

    uint    (*size)     (AdsBuffer* obj);

    int64   (*created)(AdsBuffer* obj);

    AdsDataType (*datatype)(AdsBuffer* obj);

    AdsBuffer* (*from_pointer)  (AdsDataType type,
                                 int size,
                                 pointer ptr);
} BufferClass;



BufferClass*        object_class_init       ();



#endif