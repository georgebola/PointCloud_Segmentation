mkdir -p build
cd build
cmake ..
make -j4

cd ..

./build/sample/sample Z6.obj
