wget --quiet https://www.sfml-dev.org/files/SFML-2.4.2-linux-gcc-64-bit.tar.gz
tar -xf SFML-2.4.2-linux-gcc-64-bit.tar.gz
mv SFML-2.4.2 SFML
cp SFML/lib/*.so.2.4 .
cp SFML/lib/*.so.2.4 tests
