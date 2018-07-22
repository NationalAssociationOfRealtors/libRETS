FROM debian:9.5
RUN apt-get update && apt-get install -y \
build-essential \
ruby \
ruby-dev \
python3-dev \
python3-pip \
autoconf \
swig \
libboost-dev \
libboost-filesystem-dev \
libcurl4-openssl-dev \
libexpat-dev \
default-jdk
ADD . / librets/
WORKDIR /librets
RUN ./autogen.sh && ./configure \
--enable-depends \
--enable-shared_dependencies \
&& make \
&& make install
WORKDIR /
