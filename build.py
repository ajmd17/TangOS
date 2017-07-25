import os
import os.path

path = str("./")

for dirpath, dirnames, filenames in os.walk(path):
    for file in [f for f in filenames]:

        if file.endswith(".c") or file.endswith(".cpp"):
            os.system("./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c {}/{} -o obj/{}.o -ffreestanding -O2 -w".format(dirpath, file, os.path.splitext(file)[0]))
            #print("running C file '{}'".format(os.path.splitext(file)[0] + ".c"))            

for dirpath, dirnames, filenames in os.walk(path):
    for file in [f for f in filenames]:

        if file.endswith(".s"):
            os.system("./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as {}/{} -o obj/{}.o".format(dirpath, file, os.path.splitext(file)[0]))
            #print("running ASM file '{}'".format(file))

linkfile = "./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib "
#Link files

for dirpath, dirnames, filenames in os.walk(path + "/obj"):
    for file in [f for f in filenames]:
        if file.endswith(".o"):
            #print("linking .o file '{}'".format(file))
            linkfile = "{} obj/{}".format(linkfile, file)

            
os.system("{}".format(linkfile))
#os.system("grub-mkrescue -o myos.iso myos.bin")
os.system("qemu-system-i386 -hdd fs.img -kernel myos.bin")
#os.system("qemu-system-i386 -cdrom main.iso")

