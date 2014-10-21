# GeodesicWgs84 -- Distance calculations on the Earth's surface

This an implementation of the geodesic algorithms described in
C. F. F. Karney "Algorithms for geodesics" ( http://dx.doi.org/10.1007/s00190-012-0578-z ).
It uses the WGS84 standard as a reference ( http://en.wikipedia.org/wiki/World_Geodetic_System ).

### From the abstract of this article

Algorithms for the computation of geodesics on
an ellipsoid of revolution are given. These provide accurate,
robust, and fast solutions to the direct and inverse geodesic
problems and they allow differential and integral properties
of geodesics to be computed.

### Original work

The files geodesic.c and geodesic.h make up for the real work and can be found
at ( http://geographiclib.sourceforge.net/html/C/ ).

## Installation

Add this line to your application's Gemfile:

    gem 'geodesic_wgs84'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install geodesic_wgs84

## Sample Usage

    require 'geodesic_wgs84'

    wgs84 = Wgs84.new

    wgs84.lat_lon ...

    wgs84.distance ...

    wgs84.average ...

    wgs84.center ...

## Running tests

Tests are based on minitest.

    rake test

## Contributing

1. Fork it ( https://github.com/volkerwiegand/geodesic_wgs84/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
