
FROM ubuntu:14.04
MAINTAINER Ernesto Alejo <ernestokarim@gmail.com>

RUN apt-get update
RUN apt-get -y install p7zip-full

ADD assets/cef_binary_3.1750.1738_linux64.7z /tmp/cef.7z
RUN 7z x /tmp/cef.7z && mv cef_binary_3.1750.1738_linux64 /opt/cef

RUN apt-get -y install cmake g++ libgtkglext1-dev
RUN apt-get -y install libudev-dev libasound-dev libXtst-dev libnss3-dev libgconf2-dev
RUN ln -sf /lib/$(arch)-linux-gnu/libudev.so.1 /lib/$(arch)-linux-gnu/libudev.so.0
RUN cd /opt/cef && make BUILDTYPE=Release

CMD /bin/bash
