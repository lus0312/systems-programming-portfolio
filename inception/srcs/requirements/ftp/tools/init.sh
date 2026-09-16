#!/bin/sh
set -e

# Create the secure empty directory required by vsftpd
mkdir -p /var/run/vsftpd/empty

# Read the FTP password from the Docker secret
FTP_PASSWORD="$(cat /run/secrets/ftp_password)"

# Create the FTP user only if it does not already exist
if ! id ftpuser >/dev/null 2>&1; then
    useradd -m -d /var/www/wordpress -s /bin/bash ftpuser
fi

# Set the FTP user's password
echo "ftpuser:${FTP_PASSWORD}" | chpasswd

# Allow the FTP user to access the WordPress files
chown -R ftpuser:ftpuser /var/www/wordpress

# Keep vsftpd in the foreground as PID 1
exec vsftpd /etc/vsftpd.conf
