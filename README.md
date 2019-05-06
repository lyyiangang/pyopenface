# openface python warpper
this a python3 wrapper for OpenFace.
## Installation
1. install swig, wget, unzip, python3, python3-dev, libboost-filesystem-dev, libopenblas-dev, libtbb-dev
2. clone OpenFace ```git submodule add https://github.com/TadasBaltrusaitis/OpenFace.git```
3. install OpenFace ```./install.sh```
here, you can run OpenFace demo now, but it's not enough for pyopenface.
4. in OpenFace/OpenFaceConfig.cmake.in, you need replace   
```
set_and_check(OpenFace_INCLUDE_DIRS "@PACKAGE_OPENFACE_INCLUDE_DIRS@;
                            ${dlib_INCLUDE_DIRS};
                            ${Boost_INCLUDE_DIRS};
                            ${Boost_INCLUDE_DIRS}/boost;")
```
with 
```
set_and_check(OpenFace_INCLUDE_DIRS "@PACKAGE_OPENFACE_INCLUDE_DIRS@")
```
5. add 
```  
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
``` 
in both dlib/CmakeLists.txt and OpenFace/CMakeLists.txt.
then re-install dlib and OpenFace.
```
cd OpenFace/dlib/build
rm CM* -rf && cmake .. && make -j4 install
cd ../../build
rm CM* -rf && cmake .. && make -j4 install
```
6. now you can build pyopenface.
```
cd pyopenface/build && cmake .. && make -j4 install
cd ..
ln -s ./dist/model model
```
## Test
replace the **VIDEO_NAME** with your own test video and run
```
python3 test.py
```
