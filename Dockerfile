
FROM ubuntu:14.10
MAINTAINER Ernesto Alejo <ernestokarim@gmail.com>

# Update the list of packages and install 7zip
RUN apt-get update

# Add libraries to compile the app
RUN apt-get -y install cmake g++ scons libgtk2.0-dev valgrind

# # Extract CEF
# RUN apt-get -y install p7zip-full
# ADD assets/cef_binary_3.1750.1738_linux64.7z /tmp/cef.7z
# RUN 7z x /tmp/cef.7z && mv cef_binary_3.1750.1738_linux64 /opt/cef

# # Install required libraries and compile CEF
RUN apt-get -y install libgtkglext1-dev
RUN apt-get -y install libudev-dev libasound-dev libXtst-dev libnss3-dev libgconf2-dev
RUN ln -sf /lib/$(arch)-linux-gnu/libudev.so.1 /lib/$(arch)-linux-gnu/libudev.so.0
# RUN cd /opt/cef && make BUILDTYPE=Release

# # Link CEF libraries globally (aka 'sudo make install')
# RUN ln -s /opt/cef/out/Release/libcef.so /usr/lib/libcef.so
# RUN ln -s /opt/cef/out/Release/obj.target/libcef_dll_wrapper.a /usr/lib/libcef_dll_wrapper.a
# RUN ln -s /opt/cef/out/Release/obj.target/libcef_dll_wrapper /usr/lib/libcef_dll_wrapper

# # Install needed locale
# RUN locale-gen en_US && locale-gen en_US.UTF-8 && update-locale

# Add environment variables needed by the app
ENV LD_LIBRARY_PATH /renderer/cef
ENV CHROME_DEVEL_SANDBOX /renderer/cef/chrome-sandbox

# Add CEF files
ADD cef /renderer/cef

# Correct permissions for the Chrome sandbox
RUN chmod 4755 /renderer/cef/chrome-sandbox

# Add SConstruct file from the root of the project (doesn't change a lot to be a volume)
ADD SConstruct /renderer/SConstruct

CMD cd /renderer && /bin/bash
