/**
 * @file ads_log.cpp
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief 
 * @version 1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ads_log.h>
#include <ads_util.h>

#include <cstdio>
#include <cstring>

#include <thread>
#include <fstream>
#include <iostream>
#include <chrono>

#define LOG_QUEUE get_log_queue()


using namespace std::literals;



typedef struct _Err {
    int line;
    char* file;
    char level[100];
    char message[100];
    char time[100];
}Err;

static char*
find_from_back(char* str, char* word)
{
    int count = strlen(str) - 1;
    while (*(str+count) != *word && count > 0) {
        count--;
    }
    return str + count + 1;
}

static bool
find_substr(char* str, char* word)
{
    int count = 0;
    while (*(str+count)) {
        
        int y = 0;
        while (y+count < strlen(str) && *(str+count + y) == *(word+y)) {
            if (strlen(word) == y + 1) {
                return true;
            }
            y++;
        }
        count++;
    }

    return false;
}

void
render_log(AdsQueue* array) 
{
    std::ofstream outfile;
    outfile.open("./log.txt", std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    outfile.open("./log.txt", std::ios_base::app); // append instead of overwrite
    while(true)
    {
        while(!ADS_QUEUE_CLASS->peek(array)) {
            std::this_thread::sleep_for(50ms);
        }


            AdsBuffer* buf;
        Err* err = (Err*)ADS_QUEUE_CLASS->pop(array,&buf,NULL,false);

        char file_log[36] = {" "};
        file_log[35] = 0;
        snprintf(file_log,35,"FILE: %s:%d",
#ifndef MINGW
        find_from_back(err->file,"\\"),
#else
        find_from_back(err->file,"/"),
#endif
        err->line);

        for (int i = 0; i < 35; i++) {
            if (file_log[i] == 0) {
                memcpy(file_log + i ," ",1);
            }
        }

        char level_log[31] = {" "};
        level_log[30] = 0;
        snprintf(level_log,30,"LEVEL: %s",err->level);

        for (int i = 0; i < 30; i++) {
            if (level_log[i] == 0) {
                memcpy(level_log + i ," ",1);
            }
        }

        char time_log[61] = {" "};
        time_log[60] = 0;
        snprintf(time_log,60,"TIMESTAMP: %s",err->time);

        for (int i = 0; i < 60; i++) {
            if (time_log[i] == 0) {
                memcpy(time_log+ i ," ",1);
            }
        }
            

        outfile << level_log << "||"<< file_log << "||"<< time_log << " || MESSAGE: "<< err->message << std::endl; 

        if(find_substr(err->level,"trace"))
            goto done;

        std::cout << level_log << "||"<< file_log << "||"<< time_log << " || MESSAGE: "<< err->message << std::endl; 
    done:
        BUFFER_UNREF(buf);
    }
}

AdsQueue*
get_log_queue()
{
    static AdsQueue* ret;
    RETURN_ONCE(ret);
    ret = ADS_QUEUE_CLASS->init(INFINITE);
    std::thread render {render_log,ret};
    render.detach();
    return ret;
}


void
get_string_fmt(std::chrono::nanoseconds time,
                char** string)
{
    int64 min =      std::chrono::duration_cast<std::chrono::minutes>(time).count() %1000 %1000 %1000 %1000 %60;
    int64 sec =      std::chrono::duration_cast<std::chrono::seconds>(time).count() %1000 %1000 % 1000 % 1000;
    int64 mili =     std::chrono::duration_cast<std::chrono::milliseconds>(time).count() %1000 % 1000 % 1000;
    int64 micro =    std::chrono::duration_cast<std::chrono::microseconds>(time).count() %1000 % 1000;
    int64 nano =     std::chrono::duration_cast<std::chrono::nanoseconds>(time).count() %1000;
    snprintf(*string,100,"(min:%d|sec:%d|mili:%d|micro:%d|nano:%d)",min,sec,mili,micro,nano);
}

std::chrono::nanoseconds 
get_start_time()
{
    static std::chrono::high_resolution_clock::time_point fix;
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    RETURN_ONCE(now - fix);
    fix = std::chrono::high_resolution_clock::now();
    return now - fix;
}

void ads_log(char* file,
            int line,
            char* level,
            char* message)
{
    if(find_substr(file,"/util/") || find_substr(file, "\\util\\"))
        return;

    char timestr[100] = {0};
    char* temp = timestr;
    auto time = get_start_time();
    get_string_fmt(time,&temp);

    BUFFER_MALLOC(buf,sizeof(Err),ptr);
    memcpy(((Err*)ptr)->message,message,strlen(message));
    memcpy(((Err*)ptr)->level,level,strlen(level));
    memcpy(((Err*)ptr)->time,timestr,strlen(timestr));
    ((Err*)ptr)->line = line;
    ((Err*)ptr)->file = file;
    ADS_QUEUE_CLASS->push(LOG_QUEUE,buf,false);
    BUFFER_UNREF(buf);
    return;
}

char*
map_event(BufferEventType type)
{
    switch (type)
    {
    case BufferEventType::INIT:
        return "INIT";
    case BufferEventType::REF:
        return "REF";
    case BufferEventType::UNREF:
        return "UNREF";
    case BufferEventType::FREE:
        return "FREE";
    default:
        return "unknown";
    }
}



void log_buffer(BufferLog* lg,
                std::chrono::high_resolution_clock::time_point created,
                int line,
                char* file,
                BufferEventType type)
{
    if (TRACE_RECORD) {
        int64 createTime = std::chrono::duration_cast<std::chrono::milliseconds>(created.time_since_epoch()).count() % 10000;
        char str[100] = {0};
        snprintf(str, 100, "buffer id %d contain %s : %s",createTime,lg->dataType,map_event(type));
        ads_log(file,line,"trace",str);
    }
    
}
