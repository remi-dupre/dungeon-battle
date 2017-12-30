.ONESHELL:

# Create debian package
package-deb: CFLAGS += -s -DPACKAGE
package-deb:
	mkdir -p for-debian/usr/share/doc/dungeon-battle
	cp -r DEBIAN for-debian
	gzip --best -nc changelog > for-debian/usr/share/doc/dungeon-battle/changelog.gz
	cp copyright for-debian/usr/share/doc/dungeon-battle

	mkdir -p for-debian/usr/games
	strip ../dungeon-battle
	cp ../dungeon-battle for-debian/usr/games

	mkdir -p for-debian/usr/share/dungeon-battle
	cp -r ../data/* for-debian/usr/share/dungeon-battle

	fakeroot dpkg-deb --build for-debian dungeon-battle.deb

# Create portable version
package-tar:
	mkdir -p for-tar
	cp changelog for-tar
	cp copyright for-tar

	strip ../dungeon-battle
	cp ../dungeon-battle for-tar
	cp -r ../data for-tar

	tar -czf dungeon-battle.tar.gz for-tar --transform s/for-tar/dungeon-battle/