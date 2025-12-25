cmake_minimum_required(VERSION 3.10)
project(FilmTests)

# Находим GoogleTest
find_package(GTest REQUIRED)
include(GoogleTest)

# Файлы тестов
set(TEST_SOURCES
    test_main.cpp
    test_films.cpp
    test_container.cpp
    test_commands.cpp
)

# Создаем исполняемый файл тестов
add_executable(film_tests ${TEST_SOURCES})

# Подключаем заголовки
target_include_directories(film_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}  # Корневая директория с Film.h и другими
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Линкуем с библиотекой классов и gtest
target_link_libraries(film_tests
    GTest::gtest
    GTest::gtest_main
    film_classes  # Наша библиотека классов
)

# Добавляем тесты в систему тестирования
gtest_discover_tests(film_tests)
