FROM ubuntu:latest
# this is for tzdata, which otherwise will freeze the installation
RUN export DEBIAN_FRONTEND=noninteractive
RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime

RUN apt-get update && apt-get --assume-yes install git build-essential cmake 
RUN apt-get --assume-yes install libx11-dev libxxf86vm-dev libxrandr-dev libpthread-workqueue-dev libxi-dev libdlm3 libxinerama-dev libxcursor-dev libgle3-dev

WORKDIR "/usr"
RUN mkdir work
WORKDIR "/usr/work"

# glfw
RUN git clone https://github.com/glfw/glfw
WORKDIR "/usr/work/glfw"
RUN mkdir build
WORKDIR "/usr/work/glfw/build"
RUN cmake ..
RUN make install
WORKDIR "/usr/work"

# glm
RUN git clone https://github.com/g-truc/glm
WORKDIR "/usr/work/glm"
RUN cmake .
RUN make install
WORKDIR "/usr/work"

RUN git clone https://github.com/HilbertCurve/pontilus
WORKDIR "/usr/work/pontilus"
RUN ./build.sh
CMD ["./build/game"]

# to run in directory {D} of this file:
#
# /home/user/{D}$ xhost +
# /home/user/{D}$ sudo docker run -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /tmp/.Xauthority:/tmp/.XAuthority --net=host {IMAGE_ID}
#
# {IMAGE_ID} is the latest docker container shown after the build script is ran
