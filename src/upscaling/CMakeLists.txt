cmake_minimum_required( VERSION 3.16 )
add_module( "lib" "upscaling" "vpc" ${FOLDER_CONTEXT} TRUE TRUE )

start_sources()

add_sources( SOURCE_GROUP "Public"
        "upscaling.cpp"
        "upscaling.h"
)

end_sources()
