# CMakeList.txt : CMake project for YetAnotherLogViewer, include source and define
# project specific logic here.

set (RESOURCES
    Logger.cpp
    YetAnotherLogViewer.cpp
    Parser.cpp
    LoggerModel.cpp
    FileUtil.cpp
    NetworkUtil.cpp
)

set (INCLUDES
    Logger.hpp
    YetAnotherLogViewer.hpp
    Parser.hpp
    LoggerModel.hpp
    FileUtil.hpp
    NetworkUtil.hpp
)

qt_add_executable(YetAnotherLogViewer
    ${RESOURCES}
    ${INCLUDES}
    resources.qrc
)

qt_add_qml_module(YetAnotherLogViewer
    URI YetAnotherLogViewer
    VERSION 1.0
    QML_FILES Main.qml
)

find_path(CONCURRENTQUEUE_INCLUDE_DIR concurrentqueue.h "C:\\src\\vcpkg\\vcpkg\\installed\\x86-windows\\include\\concurrentqueue")
target_include_directories(YetAnotherLogViewer 
PUBLIC ${CONCURRENTQUEUE_INCLUDE_DIR}
PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(YetAnotherLogViewer PRIVATE 
Qt6::Widgets 
Qt6::Core 
Qt6::Quick 
${Boost_LIBRARIES})

