g++ -lsfml-system lighting.cpp

cat map1 |./a.out > map1res
diff -B -w -q map1res map1sol

cat map2 |./a.out > map2res
diff -B -w -q map2res map2sol

cat map2b |./a.out > map2bres
diff -B -w -q map2bres map2bsol

cat map3 |./a.out > map3res
diff -B -w -q map3res map3sol

cat map4 |./a.out > map4res
diff -B -w -q map4res map4sol
