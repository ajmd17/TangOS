sudo apt-get install qemu
mkdir obj
qemu-img create fs.raw 1024
echo
echo download cross-compiler from here:
echo http://newos.org/toolchains/i686-elf-4.9.1-Linux-x86_64.tar.xz
echo place folder into directory. Now run command build script and you can use TangOS!