# fatcnt



# BUILD SERVICE

```
mkdir build
cd build
cmake -S .. -B . 
cmake --build . --taget install/strip
cmake --build . --target test
cmake -DCMAKE_INSTALL_PREFIX=/opt/fatcnt ..
cpack -G DEB 
```


# RUN SERVICE

```
systemctl start fatcnt
journalctl -u fatcnt -f
```
