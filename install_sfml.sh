wget --quiet https://www.sfml-dev.org/files/SFML-2.4.2-linux-gcc-64-bit.tar.gz
tar -xf SFML-2.4.2-linux-gcc-64-bit.tar.gz
mv SFML-2.4.2 SFML
mv SFML/include/SFML clang/include/c++/v1
mv SFML/lib/* clang/lib
