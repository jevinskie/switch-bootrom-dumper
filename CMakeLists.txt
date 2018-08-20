cmake_minimum_required(VERSION 3.9)
project(switch-bootrom-dumper C)

add_library(srec-dump srec-dump.c)
target_compile_options(srec-dump PRIVATE -Wall -Wextra -std=c11)

add_executable(srec-dump-tester srec-dump-tester.c)
target_compile_options(srec-dump-tester PRIVATE -Wall -Wextra -std=c11)
target_link_libraries(srec-dump-tester srec-dump)

install(TARGETS srec-dump-tester RUNTIME DESTINATION .)
