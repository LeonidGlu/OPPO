cmake_minimum_required(VERSION 3.10)
project(FilmLab1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(film_classes STATIC
    Film.cpp
    GameFilm.cpp
    CartoonFilm.cpp
    SeriesFilm.cpp
    FilmContainer.cpp
)

add_executable(film_lab OPPPO_lab1.cpp)
target_link_libraries(film_lab film_classes)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
