#!/bin/sh
set -e

# MariaDB needs this runtime directory for its socket and PID-related files
mkdir -p /run/mysqld
chown mysql:mysql /run/mysqld

# Read the database password from the Docker secret
MYSQL_PASSWORD="$(cat /run/secrets/db_password)"

# Create an initialization SQL file.
# IF NOT EXISTS and ALTER USER make repeated container starts safe.
cat > /run/mysqld/init.sql << EOF
CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%';
ALTER USER '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';
GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';
FLUSH PRIVILEGES;
EOF

# Restrict access to the temporary SQL file
chown mysql:mysql /run/mysqld/init.sql
chmod 600 /run/mysqld/init.sql

# Replace the shell with MariaDB so mariadbd becomes the container's PID 1
exec mariadbd --user=mysql --init-file=/run/mysqld/init.sql
