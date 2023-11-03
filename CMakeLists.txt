cmake_minimum_required(VERSION 3.21.0)
project(version_info VERSION 0.0.1 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
string(TIMESTAMP TIME "%Y-%m-%d %H:%M")
configure_file(version_info.cpp.in version_info.cpp @ONLY)
add_library(version_info  SHARED version_info.cpp)
# add public header
set_target_properties(version_info PROPERTIES PUBLIC_HEADER version_info.h)
target_include_directories(version_info PRIVATE .)
add_library(print SHARED print.cpp)
set_target_properties(print PROPERTIES PUBLIC_HEADER print.h)
target_link_libraries(version_info PUBLIC print)
# target_link_libraries(print version_info) #  circular dependency

add_executable(main main.cpp)
target_link_libraries(main version_info)

# set main build to dir Exc
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Exc)

# This need rebuild target for recopy dll.
function(copy_dll target)
add_custom_command(TARGET ${target} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy -t $<TARGET_FILE_DIR:${target}> $<TARGET_RUNTIME_DLLS:${target}>
  COMMAND_EXPAND_LISTS
  )
endfunction()

copy_dll(main)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# set cpack destination
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/package")
# set install dir name
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

set(CPACK_PACKAGE_EXECUTABLES "main" "main")


include(CPack)

install(TARGETS main  DESTINATION bin) # exclude ms-win

install(TARGETS version_info print
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/version_info)
