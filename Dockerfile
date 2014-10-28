
FROM ubuntu:14.04
MAINTAINER Ernesto Alejo <ernestokarim@gmail.com>

# Update the list of packages and install 7zip
RUN apt-get update
RUN apt-get -y install p7zip-full

# Extract CEF
ADD assets/cef_binary_3.1750.1738_linux64.7z /tmp/cef.7z
RUN 7z x /tmp/cef.7z && mv cef_binary_3.1750.1738_linux64 /opt/cef

# Install required libraries and compile CEF
RUN apt-get -y install cmake g++ libgtkglext1-dev
RUN apt-get -y install libudev-dev libasound-dev libXtst-dev libnss3-dev libgconf2-dev
RUN ln -sf /lib/$(arch)-linux-gnu/libudev.so.1 /lib/$(arch)-linux-gnu/libudev.so.0
RUN cd /opt/cef && make BUILDTYPE=Release

# Link CEF libraries globally (aka 'sudo make install')
RUN ln -s /opt/cef/out/Release/libcef.so /usr/lib/libcef.so
RUN ln -s /opt/cef/out/Release/obj.target/libcef_dll_wrapper.a /usr/lib/libcef_dll_wrapper.a

# Add libraries for our app
RUN apt-get -y install scons libgtk2.0-dev

# Add SConstruct file from the root of the project (doesn't change a lot)
ADD SConstruct /prerender/SConstruct

CMD cd /prerender && /bin/bash
