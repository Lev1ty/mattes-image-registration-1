CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(RigidMultimodalMultiresolutionRegistrationApp)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
SET(BUILD_SHARED_LIBS TRUE)
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
ADD_SUBDIRECTORY(CHECK)
ADD_SUBDIRECTORY(Wrapper)
ADD_SUBDIRECTORY(Reader)
ADD_SUBDIRECTORY(Writer)
ADD_SUBDIRECTORY(Visualizer)
ADD_SUBDIRECTORY(ITKtoVTK)
ADD_SUBDIRECTORY(Registrator)
ADD_SUBDIRECTORY(Observer)
ADD_EXECUTABLE(Entry Entry.cpp)
TARGET_LINK_LIBRARIES(Entry Reader Writer Visualizer ITKtoVTK Registrator)
