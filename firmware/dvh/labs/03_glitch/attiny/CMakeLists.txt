cmake_minimum_required(VERSION 3.22)

set(CMAKE_PROJECT_NAME attiny)
project(${CMAKE_PROJECT_NAME})

set(MCU attiny85)
set(F_CPU 125000UL)

add_executable(${CMAKE_PROJECT_NAME} main.c)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -mmcu=${MCU} -DF_CPU=${F_CPU} -Os -g)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mmcu=${MCU})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
  COMMAND xxd -i ${CMAKE_PROJECT_NAME}.bin > ${CMAKE_SOURCE_DIR}/../include/${CMAKE_PROJECT_NAME}.h
  COMMENT "Converting ELF to binary and generating payload header..."
  VERBATIM
)
