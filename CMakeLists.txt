cmake_minimum_required(VERSION 3.9)
project(switch-bootrom-dumper C CXX)

add_subdirectory(ELFIO)

add_library(dump dump.c z85.c)
target_compile_options(dump PRIVATE -Wall -Wextra -std=gnu11)

add_executable(dump-tester dump-tester.c)
target_compile_options(dump-tester PRIVATE -Wall -Wextra -std=gnu11)
target_link_libraries(dump-tester dump)

add_executable(text-dump-to-elf text-dump-to-elf.cpp)
target_link_libraries(text-dump-to-elf elfio)

install(TARGETS text-dump-to-elf RUNTIME DESTINATION .)
