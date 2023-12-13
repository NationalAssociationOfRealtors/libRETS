# libRETS

## libRETS-Docker
libRETS Docker is a RETS client library written in C++ (forked from the NAR library) built and packaged for Docker. This allows rapid development of RETS client applications by saving the developer from dealing with the RETS protocol details. The application writer can concentrate on their application thusly saving them time and money in the process.

Platform : 
OS - Debian 9.5

Configured Language Bindings:
Python 3, 
Ruby 2.4

Requirements:
Docker

Build and test image by running the librets-build-docker.sh script.

```bash
docker build -t librets:latest .
docker run -it librets:latest /bin/bash
```
