#!/bin/bash

# Create directories
mkdir -p boot
mkdir -p root

# Download required firmware files
wget https://github.com/raspberrypi/firmware/raw/master/boot/bootcode.bin -O boot/bootcode.bin
wget https://github.com/raspberrypi/firmware/raw/master/boot/start.elf -O boot/start.elf
wget https://github.com/raspberrypi/firmware/raw/master/boot/fixup.dat -O boot/fixup.dat

# Create basic root filesystem structure
mkdir -p root/{bin,sbin,etc,proc,sys,dev,tmp,home,var/log}

# Download Busybox for basic userspace tools
wget https://busybox.net/downloads/binaries/1.31.0-defconfig-multiarch-musl/busybox-armv6l -O root/bin/busybox
chmod +x root/bin/busybox

# Create necessary symlinks for Busybox
cd root/bin
for cmd in $(./busybox --list); do
    ln -s busybox $cmd
done
cd ../..

# Create basic init script
cat > root/init << 'EOF'
#!/bin/busybox sh

# Mount essential filesystems
mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev

# Create device nodes
mknod /dev/null c 1 3
mknod /dev/tty c 5 0
mknod /dev/console c 5 1
mknod /dev/ttyAMA0 c 204 64

# Set up environment
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
export HOME=/root
export PS1='ghostc:\w\$ '

# Start system logger
syslogd
klogd

# Start shell on serial console
exec /bin/sh
EOF

chmod +x root/init

# Create basic fstab
cat > root/etc/fstab << EOF
proc  /proc     proc    defaults    0   0
sysfs /sys      sysfs   defaults    0   0
EOF

# Create minimal inittab
cat > root/etc/inittab << EOF
::sysinit:/init
ttyAMA0::respawn:/bin/sh
::ctrlaltdel:/sbin/reboot
::shutdown:/bin/umount -a -r
EOF

# Update config.txt for Pi Zero W
cat > boot/config.txt << EOF
# Raspberry Pi Zero W configuration
kernel=kernel.img
arm_control=0x200
disable_commandline_tags=1
uart_2ndstage=1
enable_uart=1
core_freq=250
dtoverlay=miniuart-bt
gpu_mem=16

# Force console on UART
console=serial0,115200
EOF

# Create cmdline.txt
echo "console=serial0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait" > boot/cmdline.txt
