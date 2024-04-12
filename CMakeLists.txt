cmake_minimum_required(VERSION 3.5)
project(uds_kobuki)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(matplotlib_cpp REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  msg/ControlCmd.msg
)


add_executable(pub_cmd src/pub.cpp)
add_executable(sub_odom src/sub.cpp)
add_executable(plot_scan src/subLidar.cpp)
add_executable(master src/master.cpp)
add_executable(prcs src/process.cpp)
add_executable(automatic src/automatic.cpp)
ament_target_dependencies(pub_cmd rclcpp geometry_msgs)
ament_target_dependencies(sub_odom rclcpp nav_msgs)
ament_target_dependencies(plot_scan rclcpp sensor_msgs)
ament_target_dependencies(master rclcpp std_msgs)
ament_target_dependencies(prcs rclcpp geometry_msgs std_msgs)
ament_target_dependencies(automatic rclcpp sensor_msgs)
target_link_libraries(plot_scan matplotlib_cpp::matplotlib_cpp)
target_link_libraries(automatic matplotlib_cpp::matplotlib_cpp)

install(TARGETS
  pub_cmd 
  sub_odom
  plot_scan
  master
  prcs
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
