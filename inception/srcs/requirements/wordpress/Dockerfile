# Use Debian Bookworm as the base image
FROM debian:bookworm

# Install PHP-FPM, database support, Redis support, and download tools
RUN apt update && \
    apt install -y \
        php-fpm \
        php-mysql \
        php-redis \
        curl \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Make PHP-FPM listen on TCP port 9000 instead of a Unix socket.
# Pass database environment variables to PHP-FPM workers.
RUN sed -i 's|listen = /run/php/php8.2-fpm.sock|listen = 9000|' \
    /etc/php/8.2/fpm/pool.d/www.conf && \
    printf '\nenv[MYSQL_DATABASE] = $MYSQL_DATABASE\nenv[MYSQL_USER] = $MYSQL_USER\nenv[MYSQL_PASSWORD] = $MYSQL_PASSWORD\n' \
    >> /etc/php/8.2/fpm/pool.d/www.conf

# Download the WordPress source files
RUN mkdir -p /var/www && \
    curl -o /tmp/wordpress.tar.gz https://wordpress.org/latest.tar.gz && \
    tar -xzf /tmp/wordpress.tar.gz -C /var/www && \
    rm /tmp/wordpress.tar.gz

# Install WP-CLI for automated WordPress configuration
RUN curl -o /usr/local/bin/wp \
    https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar && \
    chmod +x /usr/local/bin/wp

COPY tools/init.sh /usr/local/bin/init.sh
RUN chmod +x /usr/local/bin/init.sh

ENTRYPOINT ["/usr/local/bin/init.sh"]
