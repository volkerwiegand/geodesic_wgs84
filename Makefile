all: compile

compile:
	rake compile

install: clean
	rake compile
	sudo rake install

clean:
	sudo gem uninstall geodesic_wgs84 -a -x --force
	sudo rm -rf tmp pkg lib/geodesic_wgs84/geodesic_wgs84.so

