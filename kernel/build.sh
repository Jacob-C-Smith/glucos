clear;rm lib;cd ../gnu-efi/;make;make bootloader;cd ../kernel/;make;make kernel;make link;make buildimg
