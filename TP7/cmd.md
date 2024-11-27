sudo apt-get install sed make binutils gcc g++ bash patch gzip bzip2 perl tar cpio python3 unzip rsync wget libncurses-dev libssl-dev

sudo apt-get update

which bzip2

git clone https://github.com/buildroot/buildroot.git
cd buildroot

make beaglebone_defconfig

wget http://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.xz

mv binutils-2.38.tar.xz /home/fredlinux/Documents/OS-embarque/TP7/buildroot-2023.05/dl/

sudo ifconfig enx000ec6647e31 192.168.1.1 netmask 255.255.255.0 up

cd /sys/class/leds/beaglebone:green:heartbeat

echo none > trigger

echo 1 > brightness
echo 0 > brightness

echo timer > trigger

arm-linux-gcc -o knight_rider knight_rider.c

sudo cp knight_rider /srv/tftp/

tftp -g -r knight_rider 192.168.1.1

chmod +x knight_rider 
