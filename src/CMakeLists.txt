
aux_source_directory(. DIR_SRC_SRCS)

include_directories(../lib/config)
include_directories(../lib/lcd)
include_directories(../lib/font)
include_directories(../lib/fatfs)
include_directories(../lib/sdcard)

add_library(src ${DIR_SRC_SRCS})
target_link_libraries(src PUBLIC config lcd font fatfs)
