
FROM ubuntu:14.10
MAINTAINER Ernesto Alejo <ernestokarim@gmail.com>

# Update the list of packages and install 7zip
RUN apt-get update

# Add libraries to compile the app
RUN apt-get -y install cmake g++ scons libgtk2.0-dev wget autoconf subversion git valgrind xvfb p7zip-full libgtkglext1-dev libudev-dev libasound-dev libXtst-dev libnss3-dev libgconf2-dev
RUN ln -sf /lib/$(arch)-linux-gnu/libudev.so.1 /lib/$(arch)-linux-gnu/libudev.so.0

# Install latest version of CMake to avoid problems with the PIC flag for the compiler
ADD http://www.cmake.org/files/v3.0/cmake-3.0.2.tar.gz /tmp/cmake.tar
RUN cd /tmp && \
    tar -xf cmake.tar && \
    cd cmake-3.0.2 && \
    ./configure && \
    make && \
    make install

# Checkout and build gflags
RUN git clone https://code.google.com/p/gflags/ && \
    cd gflags && \
    mkdir build && \
    cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_CXX_FLAGS=-fPIC -DGFLAGS_NAMESPACE=google && \
    make && \
    make install
    
# Checkout and build glog
RUN svn checkout http://google-glog.googlecode.com/svn/trunk/ google-glog && \
    cd google-glog && \
    ./configure --prefix=/usr && \
    make && \
    make install

# Add .bashrc commands to the already present file
ADD docker/provision/.bashrc /tmp/.bashrc
RUN cat /tmp/.bashrc >> ~/.bashrc

# Install protobuf
RUN apt-get install -y curl libtool
RUN cd /tmp && \
    git clone https://github.com/google/protobuf.git && \
    cd protobuf && \
    ./autogen.sh && \
    ./configure --prefix=/usr && \
    make && \
    make install

# Checkout and compile Go
RUN cd / && \
    hg clone -u release-branch.go1.3 https://code.google.com/p/go && \
    cd /go/src && \
    ./make.bash

# Download CEF from my own public version and extract it
ADD https://googledrive.com/host/0B-unE3_O4B0uSWFYdEJZa1hEQmc/cef_binary_3.2171.1949_linux64.7z /tmp/cef.7z
RUN 7za x /tmp/cef.7z && \
    mv cef_binary_3.2171.1949_linux64 cef && \
    cd /cef && \
    make BUILDTYPE=Release

# Add environment variables needed by the app
ENV LD_LIBRARY_PATH /cef/out/Release:/cef/out/Release/obj.target
ENV CHROME_DEVEL_SANDBOX /cef/chrome-sandbox
ENV DISPLAY :100.0

CMD (Xvfb :100 -ac &) && cd /renderer && /bin/bash
