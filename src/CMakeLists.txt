add_library(lib_alojamiento STATIC alojamiento.cpp)
target_include_directories(lib_alojamiento PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_app STATIC app.cpp)
target_include_directories(lib_app PRIVATE ${PROJECT_SOURCE_DIR}/include)