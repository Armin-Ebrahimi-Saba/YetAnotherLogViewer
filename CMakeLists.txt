# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
project ("YetAnotherLogViewer")

set(DETAILED_FAILURE_MSG ON)
set(TRACEDESIGNTIME ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.5.1\\msvc2019_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.2 REQUIRED COMPONENTS Qml Core Widgets Quick)
set(DMAKE_TOOLCHAIN_FILE "C:\\src\\vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
# camel concurrent queue
find_package(concurrentqueue)
# Boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF) 
set(Boost_DEBUG ON)
set(DETAILED_FAILURE_MSG ON)
set(Boost_INCLUDE_DIR "C:\\Boost\\boost_1_82_0\\include\\boost-1_82")
find_package(Boost 1.82 REQUIRED)
if(Boost_FOUND)
    message("include directory: ${Boost_INCLUDE_DIRS}")
    message("library directory: ${Boost_LIBRARY_DIRS}")
endif()
qt_standard_project_setup()
add_subdirectory ("YetAnotherLogViewer")
set_target_properties(YetAnotherLogViewer PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)