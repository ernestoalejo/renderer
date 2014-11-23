
FROM ubuntu:14.10
MAINTAINER Ernesto Alejo <ernestokarim@gmail.com>

# Update the list of packages and install 7zip
RUN apt-get update

# Add libraries to compile the app
RUN apt-get -y install cmake g++ scons libgtk2.0-dev wget autoconf subversion git valgrind xvfb p7zip-full libgtkglext1-dev libudev-dev libasound-dev libXtst-dev libnss3-dev libgconf2-dev
RUN ln -sf /lib/$(arch)-linux-gnu/libudev.so.1 /lib/$(arch)-linux-gnu/libudev.so.0

# # Extract CEF
# RUN apt-get -y install p7zip-full
# ADD assets/cef_binary_3.1750.1738_linux64.7z /tmp/cef.7z
# RUN 7z x /tmp/cef.7z && mv cef_binary_3.1750.1738_linux64 /opt/cef

# RUN cd /opt/cef && make BUILDTYPE=Release

# # Link CEF libraries globally (aka 'sudo make install')
# RUN ln -s /opt/cef/out/Release/libcef.so /usr/lib/libcef.so
# RUN ln -s /opt/cef/out/Release/obj.target/libcef_dll_wrapper.a /usr/lib/libcef_dll_wrapper.a
# RUN ln -s /opt/cef/out/Release/obj.target/libcef_dll_wrapper /usr/lib/libcef_dll_wrapper

# # Install needed locale
# RUN locale-gen en_US && locale-gen en_US.UTF-8 && update-locale

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

# Add environment variables needed by the app
ENV LD_LIBRARY_PATH /renderer/cef
ENV CHROME_DEVEL_SANDBOX /renderer/cef/chrome-sandbox
ENV DISPLAY :100.0

CMD (Xvfb :100 -ac &) && cd /renderer && /bin/bash
