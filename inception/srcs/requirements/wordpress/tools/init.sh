#!/bin/sh
set -e

# Create wp-config.php only during the first initialization
if [ ! -f /var/www/wordpress/wp-config.php ]; then
    cat > /var/www/wordpress/wp-config.php << 'EOF'
<?php

define('DB_NAME', getenv('MYSQL_DATABASE'));
define('DB_USER', getenv('MYSQL_USER'));
define('DB_PASSWORD', getenv('MYSQL_PASSWORD'));
define('DB_HOST', 'mariadb');

define('WP_REDIS_HOST', 'redis');
define('WP_REDIS_PORT', 6379);

define('DB_CHARSET', 'utf8mb4');
define('DB_COLLATE', '');

$table_prefix = 'wp_';

define('WP_DEBUG', false);

if (!defined('ABSPATH')) {
    define('ABSPATH', __DIR__ . '/');
}

require_once ABSPATH . 'wp-settings.php';
EOF
fi

# Existing persistent installations may have an older wp-config.php,
# so add Redis settings only if they are missing
if ! grep -q "WP_REDIS_HOST" /var/www/wordpress/wp-config.php; then
    sed -i "/define('DB_HOST', 'mariadb');/a\\
define('WP_REDIS_HOST', 'redis');\\
define('WP_REDIS_PORT', 6379);" \
        /var/www/wordpress/wp-config.php
fi

# Read confidential values from Docker secrets
MYSQL_PASSWORD="$(cat /run/secrets/db_password)"
WP_ADMIN_PASSWORD="$(cat /run/secrets/wp_admin_password)"
WP_USER_PASSWORD="$(cat /run/secrets/wp_user_password)"

# PHP-FPM and WP-CLI need the database password as an environment variable
export MYSQL_PASSWORD

# Install WordPress only if the database has not already been initialized
if ! wp core is-installed \
    --path=/var/www/wordpress \
    --allow-root 2>/dev/null
then
    # Retry because MariaDB may still be starting
    for i in $(seq 1 30)
    do
        wp core install \
            --path=/var/www/wordpress \
            --url="https://${DOMAIN_NAME}" \
            --title="${WP_TITLE}" \
            --admin_user="${WP_ADMIN_USER}" \
            --admin_password="${WP_ADMIN_PASSWORD}" \
            --admin_email="${WP_ADMIN_EMAIL}" \
            --skip-email \
            --allow-root && break

        sleep 2
    done

    # Fail the container if WordPress is still not installed
    wp core is-installed \
        --path=/var/www/wordpress \
        --allow-root
fi

# Create the regular WordPress user only once
if ! wp user get "${WP_USER}" \
    --path=/var/www/wordpress \
    --allow-root \
    >/dev/null 2>&1
then
    wp user create \
        "${WP_USER}" \
        "${WP_USER_EMAIL}" \
        --role=subscriber \
        --user_pass="${WP_USER_PASSWORD}" \
        --path=/var/www/wordpress \
        --allow-root
fi

# Install and activate the Redis Object Cache plugin if necessary
if ! wp plugin is-installed redis-cache \
    --path=/var/www/wordpress \
    --allow-root 2>/dev/null
then
    wp plugin install redis-cache \
        --activate \
        --path=/var/www/wordpress \
        --allow-root
fi

# Enable the Redis object-cache drop-in
wp redis enable \
    --path=/var/www/wordpress \
    --allow-root

# Run PHP-FPM in the foreground as the main container process
exec php-fpm8.2 -F
