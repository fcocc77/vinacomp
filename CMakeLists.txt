cmake_minimum_required(VERSION 3.8.2)

project(vinacomp VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_FLAGS "-fPIC")
include_directories(${CMAKE_SOURCE_DIR})


include_directories(
    "/usr/include/qt5"
    "/usr/include/qt5/QtWidgets"
    "/usr/include/qt5/QtCore"
    "/usr/include/qt5/QtGui"
    "/usr/include/qt5/QtMultimedia"
    "/usr/include/qt5/QtNetwork"
)

include_directories("/usr/include/python3.6m")

include_directories("libs/QCodeEditor/includes")

include_directories("source/gui/app")
include_directories("source/gui/node_graph")
include_directories("source/gui/common")
include_directories("source/gui/curve_editor")
include_directories("source/gui/knob")
include_directories("source/gui/layout")
include_directories("source/gui/nodes")
include_directories("source/gui/open_gl_view")
include_directories("source/gui/properties")
include_directories("source/gui/script_editor")
include_directories("source/gui/viewer")

include_directories("source/python_api")

include_directories("source/engine/app")
include_directories("source/engine/nodes")
include_directories("source/engine/project")
include_directories("source/engine/render")


include_directories("utils/c++")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DGUI)
add_definitions(${Qt5Widgets_DEFINITIONS})

file(GLOB_RECURSE GUICC "source/gui/*/*.cc")
file(GLOB_RECURSE render "source/engine/render/*.cc")
file(GLOB_RECURSE project "source/engine/project/*.cc")
file(GLOB_RECURSE nodes "source/engine/nodes/*.cc")
file(GLOB_RECURSE APICC "source/python_api/*.cc")
file(GLOB_RECURSE UTILCC "utils/c++/*.cc")
file(GLOB_RECURSE code_editor "libs/QCodeEditor/src/*")

# ejecutables
add_executable(
    vinacomp
    ${GUICC}
    ${render}
    ${project}
    ${nodes}
    ${APICC}
    ${UTILCC}
    ${code_editor}
)

find_package(PythonLibs REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Multimedia)
find_package(Qt5Network)
find_package(OpenGL REQUIRED)
find_package( OpenCV REQUIRED )

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries(
    vinacomp 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Multimedia
    Qt5::Network
    OpenGL
    ${OpenCV_LIBS}
    ${PYTHON_LIBRARIES}
)


