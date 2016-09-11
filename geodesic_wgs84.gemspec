# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'geodesic_wgs84/version'

Gem::Specification.new do |spec|
  spec.name          = "geodesic_wgs84"
  spec.version       = GeodesicWgs84::VERSION
  spec.date          = "2016-09-11"
  spec.authors       = ["Volker Wiegand"]
  spec.email         = ["volker.wiegand@cvw.de"]
  spec.summary       = "Calculate distances on Earth using WGS84"
  spec.description   = "Geodesic functions based on http://geographiclib.sourceforge.net/"
  spec.homepage      = "http://github.com/volkerwiegand/geodesic_wgs84"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake", ">= 10.1"
  spec.add_development_dependency "rake-compiler", "~> 1.0"

  spec.extensions    = %w[ext/geodesic_wgs84/extconf.rb]
end
