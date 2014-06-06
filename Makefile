# vim: set ts=8 tw=0 noet :
#
# Makefile for building the Gem
#

all:
	rake compile
	git add ext
	git add lib
	git add spec
	vim lib/geodesic_wgs84/version.rb
	git commit -a
	rake release
