FROM alpine:3.19 AS builder

RUN apk add --no-cache clang make musl-dev

WORKDIR /build

COPY Makefile ./
COPY include/ ./include/
COPY src/ ./src/

RUN make all

FROM alpine:3.19

WORKDIR /app

COPY --from=builder /build/app .

ENTRYPOINT ["./app"]
