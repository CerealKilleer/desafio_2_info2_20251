add_library(lib_alojamiento STATIC alojamiento.cpp)
target_include_directories(lib_alojamiento PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_huesped STATIC huesped.cpp)
target_include_directories(lib_huesped PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_anfitrion STATIC anfitrion.cpp)
target_include_directories(lib_anfitrion PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_fecha STATIC fecha.cpp)
target_include_directories(lib_fecha PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_app STATIC app.cpp)
target_include_directories(lib_app PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(lib_reserva STATIC reserva.cpp)
target_include_directories(lib_reserva PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(lib_reserva PRIVATE 
                        lib_fecha)

target_link_libraries(lib_app PRIVATE
                        lib_alojamiento
                        lib_huesped
                        lib_anfitrion
                        lib_fecha
                        lib_reserva)