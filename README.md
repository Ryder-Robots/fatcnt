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


# INSTALLING SERVICE

Before using these instructions, a GitHub token must be created and the environmental variable
GH_TOKEN must be set up.

```
gh release -R git@github.com:Ryder-Robots/fatcnt.git download 0.0.4
dpkg -i ./fatcnt_0.0.4_arm64.deb 
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