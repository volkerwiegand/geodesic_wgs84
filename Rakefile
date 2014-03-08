require "bundler/gem_tasks"

require 'rake/extensiontask'

Rake::ExtensionTask.new "geodesic_wgs84" do |ext|
  ext.lib_dir = "lib/geodesic_wgs84"
end

require 'rake/testtask'

Rake::TestTask.new do |t|
  t.pattern = "spec/*_spec.rb"
end

task :default => :test
task :spec => :test

