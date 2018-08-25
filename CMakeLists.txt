cmake_minimum_required(VERSION 3.9)
project(switch-bootrom-dumper C CXX)

add_subdirectory(ELFIO)
add_subdirectory(fmt)

add_library(dump dump.c z85.c)
target_compile_options(dump PRIVATE -Wall -Wextra -std=gnu11)

add_executable(dump-tester dump-tester.c)
target_compile_options(dump-tester PRIVATE -Wall -Wextra -std=gnu11)
target_link_libraries(dump-tester dump)

add_executable(text-dump-to-elf text-dump-to-elf.cpp)
target_link_libraries(text-dump-to-elf elfio fmt dump)
set_target_properties(text-dump-to-elf PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

add_executable(dump-miniloaders dump-miniloaders.cpp)
target_link_libraries(dump-miniloaders elfio fmt)
set_target_properties(dump-miniloaders PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

install(TARGETS dump-tester text-dump-to-elf dump-miniloaders RUNTIME DESTINATION .)
