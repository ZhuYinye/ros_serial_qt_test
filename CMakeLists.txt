cmake_minimum_required(VERSION 3.0.2)

project(serial_test)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g ")

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  message_generation
)

find_package(Qt5  5.14.2 EXACT REQUIRED COMPONENTS  Widgets  )
if(Qt5_FOUND)
	message("yes found")
endif(Qt5_FOUND)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
	src/serial_data_parse.cpp
	src/serial_node.cpp
	src/serial_qt.cpp
)

set(FORMS
	src/serial_qt.ui
)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  serial.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  
  # Or other packages containing msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  #INCLUDE_DIRS include
  #LIBRARIES serial_test
  #CATKIN_DEPENDS
  #DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(serial_node ${SOURCES} ${FORMS})
target_link_libraries(serial_node
	 ${catkin_LIBRARIES}
	 Qt5::Widgets
 )
add_dependencies(serial_node serial_test_generate_messages)
