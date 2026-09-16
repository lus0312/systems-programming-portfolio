# Use Debian Bookworm as the base image
FROM debian:bookworm

# Install the FTP server
RUN apt update && \
    apt install -y \
        vsftpd \
        openssl && \
    rm -rf /var/lib/apt/lists/*

# Install the FTP server configuration
COPY conf/vsftpd.conf /etc/vsftpd.conf

# Install the container initialization script
COPY tools/init.sh /usr/local/bin/init.sh
RUN chmod +x /usr/local/bin/init.sh

ENTRYPOINT ["/usr/local/bin/init.sh"]
