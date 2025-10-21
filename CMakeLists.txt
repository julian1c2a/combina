cmake_minimum_required(VERSION 4.0)
project(Combinatoria)

# Selecciona tu compilador. Descomenta la línea correspondiente.
# Compilador g++
set(CMAKE_CXX_COMPILER "c:/msys64/ucrt64/bin/g++.exe")

# Compilador clang++
# set(CMAKE_CXX_COMPILER "c:/msys64/ucrt64/bin/clang++.exe")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

# Añade los archivos fuente al ejecutable
add_executable(Combinatoria main.cpp lib/int128_helpers.cpp)

# Añade el directorio 'inc' a las rutas de inclusión para el target 'Combinatoria'
target_include_directories(Combinatoria PUBLIC inc)
