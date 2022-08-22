#  Hold on tight, this is going to be a bumpy ride
#
#  I wasn't able to get this building directly under Ubuntu 22.04.  However, I
#  could get it to build on Debian 11, but that only has Python 3.9 where 22.04
#  has 3.10.  So I was able to use Debian testing to bring in Python 3.10,
#  because this wouldn't build entirely under Debian testing due to similar
#  issues to what I was seeing in 22.04.
#  
#  Note, once Debian Testing moves on from Python 3.10, building under Ubuntu
#  22.04 is going to be a lost cause until the core of this build issue is
#  resolved.

FROM debian:11.0
RUN apt-get update && apt-get install -y \
build-essential \
ruby \
ruby-dev \
autoconf \
libboost-dev \
libboost-filesystem-dev \
libcurl4-openssl-dev \
libexpat-dev \
git \
default-jdk
#  install python3 from testing to get 3.10
RUN bash -c "echo deb http://deb.debian.org/debian testing main >/etc/apt/sources.list.d/testing.list"
COPY pin-all /etc/apt/preferences.d/990-pin-all
COPY pin-python /etc/apt/preferences.d/991-pin-python
RUN apt-get update && apt-get install -y \
python3-dev \
libpython3-dev \
dh-python \
devscripts \
swig \
python3-pip
RUN dpkg -l | grep python3
RUN dpkg -l | grep dh-python
RUN dpkg -l | grep devscripts
ADD . / librets/
WORKDIR /librets
RUN autoupdate
RUN tar cfz ../librets_1.6.5.orig.tar.gz -C .. librets
#RUN autoconf
RUN ./autogen.sh

RUN ./configure \
--enable-depends \
--disable-python \
--enable-shared_dependencies

RUN rm -rf debian
RUN git clone https://github.com/realgo/librets-package debian
RUN debuild || true

RUN ls -l /*.deb
WORKDIR /
