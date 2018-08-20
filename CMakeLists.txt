cmake_minimum_required(VERSION 3.9)
project(switch-bootrom-dumper C)

add_library(dump dump.c)
target_compile_options(dump PRIVATE -Wall -Wextra -std=c11)

add_executable(dump-tester dump-tester.c)
target_compile_options(dump-tester PRIVATE -Wall -Wextra -std=c11)
target_link_libraries(dump-tester dump)

install(TARGETS dump-tester RUNTIME DESTINATION .)
