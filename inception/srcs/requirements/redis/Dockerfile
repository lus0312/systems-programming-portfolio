# Use Debian Bookworm as the base image
FROM debian:bookworm

# Install the Redis server
RUN apt update && \
    apt install -y redis-server && \
    rm -rf /var/lib/apt/lists/*

# Install the custom Redis configuration
COPY conf/redis.conf /etc/redis/redis.conf

# Run Redis in the foreground as the main container process
ENTRYPOINT ["redis-server", "/etc/redis/redis.conf"]
