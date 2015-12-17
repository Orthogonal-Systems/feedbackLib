cmake_minimum_required(VERSION 2.8)
project (feedback_examples)

include(${ARDUINO_SETTINGS_FILE})
 
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
 
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

### add example executable directories
add_avr_executable(feedback_test feedback_test/feedback_test.cpp)

### add the dependant libraries
### acm7812_test
avr_target_link_libraries(feedback_test AMC7812)
avr_target_link_libraries(feedback_test ARDUINO)
