# fatcnt

Create debian file.

```
cmake -S . -B build && cmake --build build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/fatcnt ..

# cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cpack -G DEB 
```