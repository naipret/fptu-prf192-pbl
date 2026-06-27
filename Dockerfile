FROM alpine:3.19 AS builder

RUN apk add --no-cache clang make musl-dev

WORKDIR /build

COPY Makefile ./
COPY include/ ./include/
COPY src/ ./src/

# Compile with static flags using make
RUN make LDFLAGS="-m64 -static"

FROM alpine:3.19

WORKDIR /app

# Copy the compiled static binary from the builder's bin folder
COPY --from=builder /build/bin/prf192-pbl-e-commerce-electronics-bundle-and-management-system /app/app

ENTRYPOINT ["./app"]
