cmake_minimum_required(VERSION 3.8)
project(destroyer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(auto_mode_destroyer src/destroyer_auto.cpp)
ament_target_dependencies(auto_mode_destroyer rclcpp geometry_msgs)

add_executable(drive_mode_destroyer src/destroyer_drive.cpp)
ament_target_dependencies(drive_mode_destroyer rclcpp geometry_msgs)

add_executable(check_move_destroyer src/destroyer_check.cpp)
ament_target_dependencies(check_move_destroyer rclcpp geometry_msgs std_msgs)

add_executable(move_destroyer src/destroyer_move.cpp)
ament_target_dependencies(move_destroyer rclcpp geometry_msgs std_msgs)

install(TARGETS
  auto_mode_destroyer
  drive_mode_destroyer
  check_move_destroyer
  move_destroyer
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
