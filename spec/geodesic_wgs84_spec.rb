require 'minitest/autorun'
require 'geodesic_wgs84'

class TestGeodesicWgs84 < MiniTest::Test
  def setup
    @wgs84 = Wgs84.new
  end

  def test_lat_lon_two_doubles
    assert_equal [1.0, 2.0], @wgs84.lat_lon(1.0, 2.0)
  end

  def test_lat_lon_array_of_doubles
    assert_equal [3.5, 4.123], @wgs84.lat_lon([3.5, 4.123])
  end

  def test_lat_lon_array_of_strings
    assert_equal [3.5, 4.123], @wgs84.lat_lon(["3.5", "4.123"])
  end

  def test_lat_lon_doubles_with_comma
    assert_equal [1.0, 2.0], @wgs84.lat_lon("1,0", "2,0")
  end
end

