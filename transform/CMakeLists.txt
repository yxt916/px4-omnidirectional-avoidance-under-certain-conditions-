cmake_minimum_required(VERSION 3.0.2)
project(transform)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  pcl_ros
  
)
find_package(PCL REQUIRED) 
find_package(OpenCV)

catkin_package(
  CATKIN_DEPENDS pcl_ros roscpp rospy sensor_msgs std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${OpenCV_LIBS}
)
include_directories(include ${OpenCV_INCLUDE_DIRS})
add_executable(transform src/transform.cpp)
target_link_libraries(transform ${catkin_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBS})


