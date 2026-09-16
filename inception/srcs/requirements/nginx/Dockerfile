# Use Debian Bookworm as the base image
FROM debian:bookworm

# Install NGINX and OpenSSL for HTTPS
RUN apt update && \
    apt install -y \
        nginx \
        openssl \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Remove the default virtual host configuration
RUN rm -f /etc/nginx/sites-enabled/default

# Install the custom NGINX configuration
COPY conf/default.conf /etc/nginx/conf.d/default.conf

# Install the container initialization script
COPY tools/init.sh /usr/local/bin/init.sh
RUN chmod +x /usr/local/bin/init.sh

ENTRYPOINT ["/usr/local/bin/init.sh"]
