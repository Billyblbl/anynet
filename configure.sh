#!sh

if [ $# -eq 0 ]; then
	buildType=Debug
else
	buildType=$1
fi

mkdir -p build && cd build && conan install --build=missing .. && cmake .. -DCMAKE_BUILD_TYPE=$buildType
