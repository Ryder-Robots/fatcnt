# fatcnt



# BUILD SERVICE

```
mkdir build
cd build
cmake -S .. -B . 
cmake --build . --target install/strip
cmake --build . --target test
cpack -G DEB 
```


# RUN SERVICE

```
systemctl start fatcnt
journalctl -u fatcnt -f
```


# DESIGN

```

RRP_COMMAND, STATE, MODE, ENV ->  EaiHandler -> AiPredictor->addFeature()
FC <-  EaiHandler <- AiPredictor.predict()

```