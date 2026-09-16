#!/bin/sh
set -e

# Store the TLS certificate and private key here
mkdir -p /etc/nginx/ssl

# Generate a self-signed certificate only when it does not already exist
if [ ! -f /etc/nginx/ssl/inception.key ] || \
   [ ! -f /etc/nginx/ssl/inception.crt ]; then

    openssl req -x509 -nodes \
        -newkey rsa:2048 \
        -keyout /etc/nginx/ssl/inception.key \
        -out /etc/nginx/ssl/inception.crt \
        -days 365 \
        -subj "/CN=eulee.42.fr" \
        -addext "subjectAltName=DNS:eulee.42.fr"
fi

# Keep NGINX in the foreground so it becomes the container's PID 1
exec nginx -g 'daemon off;'
