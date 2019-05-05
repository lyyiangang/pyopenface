cmake_minimum_required (VERSION 3.2)
project(OpenFaceWrapper VERSION 2.0.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../OpenFace/cmake/modules/")
set(PYTHON_INLCUDE "/usr/include/python3.5")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -msse -msse2 -msse3")
set(LIB_NAME "_pyopenface")
set(SWIG_PY_WRAP_CPP "pyopenface_wrap.cxx")
#dlib static library also need add -fPIC flag
add_definitions(-std=c++14 -fPIC ) #  -O3 -g 

find_package(OpenFace REQUIRED)
if ( ${OpenFace_FOUND}  )
    MESSAGE("OpenFace information:")
    MESSAGE("OpenFace inlcude directory:${OpenFace_INCLUDE_DIRS}")
    MESSAGE("Openface library direcotry:${OpenFace_LIBRARIES}")
else()
    MESSAGE(FATAL_ERROR "OpenFace not found in the system.")
endif()

find_package( Boost 1.5.9 REQUIRED COMPONENTS filesystem system )
if(${Boost_FOUND})
    MESSAGE("Boost information:") 
    MESSAGE("  Boost_VERSION: ${Boost_VERSION}")
    MESSAGE("  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}") 
    MESSAGE("  Boost_LIBRARIES: ${Boost_LIBRARIES}") 
    MESSAGE("  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}") 
else()
    MESSAGE(FATAL_ERROR "Boost not found in the system.")
endif()
MESSAGE("dlib_INCLUDE_DIRS: ${dlib_INCLUDE_DIRS}")

include_directories(${OpenFace_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${PYTHON_INLCUDE})
link_directories(${Boost_LIBRARY_DIRS})

# generate siwg wrap code before build(execuate in command "cmake ..")
execute_process(COMMAND swig -python -c++ -o ${SWIG_PY_WRAP_CPP} pyopenface.i
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE CMD_ERROR
                OUTPUT_FILE CMD_OUTPUT)
MESSAGE( STATUS "swig CMD_ERROR:" ${CMD_ERROR})
MESSAGE( STATUS "swig CMD_OUTPUT:" ${CMD_OUTPUT})

add_library(${LIB_NAME} SHARED ${CMAKE_SOURCE_DIR}/${SWIG_PY_WRAP_CPP} 
                                ${CMAKE_SOURCE_DIR}/WOpenFace.cpp)
set_target_properties(${LIB_NAME} PROPERTIES PREFIX "") # no add 'lib' before library name, should add it after add_library command
target_link_libraries(${LIB_NAME} ${OpenCV_LIBS} ${OpenBLAS_LIB} ${OpenFace_LIBRARIES} LandmarkDetector Utilities dlib::dlib ${Boost_LIBRARIES}) #libLandmarkDetector.a

add_executable(test_wrapper test_wrapper.cpp WOpenFace.cpp)
target_link_libraries(test_wrapper ${OpenCV_LIBS} ${OpenBLAS_LIB} LandmarkDetector Utilities dlib::dlib ${Boost_LIBRARIES}) #libLandmarkDetector.a


install(FILES 
        "${CMAKE_SOURCE_DIR}/build/${LIB_NAME}.so"
        "${CMAKE_SOURCE_DIR}/pyopenface.py"
        DESTINATION "${CMAKE_SOURCE_DIR}/dist/")

install(DIRECTORY 
        "${CMAKE_SOURCE_DIR}/../OpenFace/build/bin/AU_predictors"
        "${CMAKE_SOURCE_DIR}/../OpenFace/build/bin/classifiers"
        "${CMAKE_SOURCE_DIR}/../OpenFace/build/bin/model"
        DESTINATION "${CMAKE_SOURCE_DIR}/dist/")