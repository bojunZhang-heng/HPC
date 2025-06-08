#! /bash/bin

if [ -n "$1" ]; then
  SOURCE_DIR="$1"
else
  SOURCE_DIR="$HOME/mae5032"
fi

BACKUP_DIR="$HOME/mae5032_backup"

mkdir -p "$BACKUP_DIR"

if [ ! -d "$SOURCE_DIR" ]; then
  echo "ERROR: "SOURCE_DIR:" does not exist!"
fi

BACKUP_FILE="$BACKUP_DIR/backup_$(date +%Y%m%d_%H%M%S).tar.gz"

echo "Create backup for: $SOURCE_DIR"

tar -czvf "$BACKUP_FILE" -C "$BACKUP_DIR" .

echo "Backup saved as: $BACKUP_FILE"

echo "Backup process completed!"

# EOF
