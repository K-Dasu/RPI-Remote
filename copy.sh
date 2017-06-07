#clear; make clean; make; sudo kextunload -b com.apple.driver.AppleSDXC; sudo kextload -b com.apple.driver.AppleSDXC; diskutil mount disk3s1; cp uart05.bin /Volumes/boot/kernel.img; sudo umount /Volumes/boot/; echo "Copied and unmounted"
clear
make clean
make
sudo kextunload -b com.apple.driver.AppleSDXC
sudo kextload -b com.apple.driver.AppleSDXC
diskutil mount KDASUBOOT
cp kernel7.img /Volumes/KDASUBOOT/kernel7.img
diskutil umount /Volumes/KDASUBOOT/
echo "Copied and unmounted"
