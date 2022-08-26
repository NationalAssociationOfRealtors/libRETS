# libRETS

Please note: I have been away from the Real Estate industry for the past decade and it appears
that NAR has gone silent over these repos. My last few attempts to contact them have been 
fruitless. The only reason I'm stil around and watching things is that I'm the last one with
write access to the repo.

That said, I'll be happy to do PRs, but I no longer have a way of proving they work, so I will
rely upon the community (or silence from the community) as an agreement to merge a PR.

Lastly, please remember that this _is_ Open Source.

MK, 26 Aug, 2022.

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
