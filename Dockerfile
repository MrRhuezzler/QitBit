FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt install -y build-essential

RUN apt-get install -y libboost-all-dev

RUN apt-get install -y cmake libncurses5-dev libncursesw5-dev

COPY . /usr/src/qitbit

WORKDIR /usr/src/qitbit

RUN mkdir build && cd build

RUN cmake ..

CMD [ "build\QitbitBC" ]