# fatcnt

Create debian file.

```
mkdir build
cd build
cmake -S .. -B . && cmake --build . --target check
cmake -DCMAKE_INSTALL_PREFIX=/opt/fatcnt ..
cpack -G DEB 
```