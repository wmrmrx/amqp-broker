FROM busybox:latest
WORKDIR app
COPY broker .
ENTRYPOINT ["./broker"]
