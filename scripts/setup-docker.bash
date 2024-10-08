#! /bin/bash

docker volume create fatcnt
docker build -t fatcnt docker
docker container rm fatcnt_src
# docker container create --mount source=fatcnt,target=/opt/share/fatcnt -p 22:22 -i -t --name fatcnt_src fatcnt
# docker container start --attach -i  fatcnt_src