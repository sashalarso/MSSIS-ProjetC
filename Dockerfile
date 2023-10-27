FROM debian:bookworm


RUN apt-get update && apt-get upgrade -y


RUN apt-get install -y gcc make libssl-dev


WORKDIR /app


COPY . /app

RUN make clean

RUN make


