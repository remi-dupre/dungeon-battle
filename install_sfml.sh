wget --quiet https://www.sfml-dev.org/files/SFML-2.4.2-linux-gcc-64-bit.tar.gz
tar -xf SFML-2.4.2-linux-gcc-64-bit.tar.gz
rsync -avq SFML-2.4.2/ clang/
rmdir SFML-2.4.2
