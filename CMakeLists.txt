# =============================================================================
#  CMakeLists.txt - Cod Rutier (Simulare de trafic rutier)
#  Alternativa la Makefile pentru build (cerinta proiectului accepta oricare).
#
#  Utilizare:
#    mkdir build && cd build
#    cmake ..
#    make
#    ./cod_rutier
# =============================================================================

cmake_minimum_required(VERSION 3.10)
project(CodRutier CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Toate sursele din src/ (jocul propriu-zis).
file(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(cod_rutier ${GAME_SOURCES})

# Testele (optional): se compileaza cu "cmake -DBUILD_TESTS=ON .."
option(BUILD_TESTS "Compileaza testele unitare" OFF)
if(BUILD_TESTS)
    enable_testing()
    file(GLOB GAME_SOURCES_NOMAIN "${CMAKE_SOURCE_DIR}/src/*.cpp")
    list(REMOVE_ITEM GAME_SOURCES_NOMAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")
    file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src} ${GAME_SOURCES_NOMAIN})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
