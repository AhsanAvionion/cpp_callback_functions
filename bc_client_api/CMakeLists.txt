cmake_minimum_required(VERSION 3.14)

project(bc_client_api LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the shared library
add_library(bc_client_api SHARED
    bc_client_api.cpp
    bc_client_api.h
)

# Optional: define a macro for export (if needed for Windows)
# target_compile_definitions(bc_client_api PRIVATE BC_CLIENT_LIB_LIBRARY)
