# Use Debian Bookworm as the base image
FROM debian:bookworm

# Install the MariaDB server and remove the package cache
RUN apt update && \
    apt install -y mariadb-server && \
    rm -rf /var/lib/apt/lists/*

# Apply the custom MariaDB network configuration
COPY conf/99-inception.cnf /etc/mysql/mariadb.conf.d/99-inception.cnf

# Install the container initialization script
COPY tools/init.sh /usr/local/bin/init.sh
RUN chmod +x /usr/local/bin/init.sh

ENTRYPOINT ["/usr/local/bin/init.sh"]
