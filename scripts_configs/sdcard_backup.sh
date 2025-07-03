#!/bin/bash
set -e

# ===== Defaults =====
DEFAULT_DEVICE="/dev/sda"
DEFAULT_ARCHIVE="./sdcard_backup.tar.gz"

MNT_BOOT="/tmp/sdcard_boot"
MNT_ROOT="/tmp/sdcard_root"
TMP_DIR="/tmp/sdcard_temp"

# ===== Usage hint =====
usage() {
    echo "Usage:"
    echo "  $0 pack   [/dev/sdX] [archive.tar.gz]"
    echo "  $0 restore [/dev/sdX] [archive.tar.gz]"
    exit 1
}

# ===== Parse args =====
ACTION="$1"
DEVICE="${2:-$DEFAULT_DEVICE}"
ARCHIVE="${3:-$DEFAULT_ARCHIVE}"

if [[ "$ACTION" != "pack" && "$ACTION" != "restore" ]]; then
    usage
fi

echo "[*] Action: $ACTION"
echo "[*] Device: $DEVICE"
echo "[*] Archive: $ARCHIVE"

# ===== Prepare =====
rm -rf "$MNT_BOOT" "$MNT_ROOT" "$TMP_DIR"
mkdir -p "$MNT_BOOT" "$MNT_ROOT" "$TMP_DIR"

# ===== Pack =====
if [ "$ACTION" == "pack" ]; then
    echo "[*] Mounting..."
    mount "${DEVICE}1" "$MNT_BOOT"
    mount "${DEVICE}2" "$MNT_ROOT"

    echo "[*] Copying BOOT partition..."
    rsync -a --info=progress2 "$MNT_BOOT/" "$TMP_DIR/boot/"

    echo "[*] Copying ROOT partition..."
    rsync -a --info=progress2 "$MNT_ROOT/" "$TMP_DIR/root/"

    echo "[*] Unmounting..."
    umount "$MNT_BOOT"
    umount "$MNT_ROOT"

    echo "[*] Creating archive (this may take time)..."
    tar -czf - -C "$TMP_DIR" boot root | pv -s $(du -sb "$TMP_DIR" | awk '{print $1}') > "$ARCHIVE"
    echo "[+] Archive created: $ARCHIVE"

# ===== Restore =====
elif [ "$ACTION" == "restore" ]; then
    echo "[*] Extracting archive..."
    pv "$ARCHIVE" | tar -xzf - -C "$TMP_DIR"

    echo "[*] Mounting..."
    mount "${DEVICE}1" "$MNT_BOOT"
    mount "${DEVICE}2" "$MNT_ROOT"

    echo "[*] Copying BOOT partition..."
    rsync -a --info=progress2 "$TMP_DIR/boot/" "$MNT_BOOT/"

    echo "[*] Copying ROOT partition..."
    rsync -a --info=progress2 "$TMP_DIR/root/" "$MNT_ROOT/"

    echo "[*] Syncing & unmounting..."
    sync
    umount "$MNT_BOOT"
    umount "$MNT_ROOT"

    echo "[+] Restore complete."
fi
