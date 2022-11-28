cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE "Debug")
project(Ads VERSION 0.1.0)


file(GLOB UTIL_SOURCE_LIST 
	utils/*.cpp
	utils/queue/*.cpp
	utils/macros/*.cpp
	utils/logs/*.cpp
	utils/event/*.cpp
	utils/buffer/*.cpp
	utils/array/*.cpp
	utils/timeseries/*.cpp
	utils/map/*.cpp
)

file(GLOB ADAPTIVE_SOURCE_LIST 
	src/*.cpp
)

set(ADS_TARGET_FILES
	${UTIL_SOURCE_LIST}
	${ADAPTIVE_SOURCE_LIST}
)



include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/header
  ${CMAKE_CURRENT_SOURCE_DIR}/src

  ${CMAKE_CURRENT_SOURCE_DIR}/utils/queue
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/macros
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/logs
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/event
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/buffer
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/array
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/map
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/timeseries
  ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
 








add_library(ads STATIC ${ADS_TARGET_FILES})
set_property(TARGET ads PROPERTY CXX_STANDARD 11)

target_link_directories(ads PUBLIC   ${INCLUDE_LIBRARY} )    
target_compile_options(ads PUBLIC	${AVDEVICE_CFLAGS})
target_compile_options(ads PUBLIC	${AVFILTER_CFLAGS})
target_compile_options(ads PUBLIC	${AVUTIL_CFLAGS})
target_compile_options(ads PUBLIC	${AVSWCALE_CFLAGS})
target_compile_options(ads PUBLIC	${AVFORMAT_CFLAGS})
target_compile_options(ads PUBLIC	${AVRESAMPLE_CFLAGS})

set(ADDITIONAL_LIB 
	ksuser
	wsock32
	ws2_32
	d3d11 
    dxgi 
    D3DCompiler
	setupapi
	dwmapi
	bz2
	Secur32
	Bcrypt
)






target_link_libraries(ads 
	${ADDITIONAL_LIB}
)
target_compile_definitions(ads PUBLIC ${ADS_DEFINITIONS})
set_target_properties(ads PROPERTIES CXX_STANDARD 17
                               VERSION ${PROJECT_VERSION}
                               SOVERSION ${PROJECT_VERSION_MAJOR})




add_executable(testa test/main.cpp)
target_link_libraries(testa ads)