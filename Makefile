# vim: set ts=8 tw=0 noet :
#
# Makefile for building the Gem
#

all: build
	git status

rel: build
	vim lib/geodesic_wgs84/version.rb
	git commit -a
	gem uninstall geodesic_wgs84 --all
	rake release

install: build
	git commit -a
	gem uninstall geodesic_wgs84 --all
	rake install
	rm -rf pkg

build:
	rake compile
	git add ext
	git add lib
	git add spec

