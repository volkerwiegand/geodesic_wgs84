require 'minitest/autorun'
require 'geodesic_wgs84'

class TestGeodesicWgs84 < MiniTest::Test
  def setup
    @wgs84 = Wgs84.new
  end

  def test_deg_as_deg
    assert_equal 50.833833, @wgs84.as_deg(50.833833)
  end

  def test_as_deg
    assert_equal 50.833833, @wgs84.as_deg("50 50 01.8")
  end

  def test_deg_as
    assert_equal "50 50 1.8", @wgs84.as_dms(50.833833)
  end

  def test_as
    assert_equal "50 50 1.8", @wgs84.as_dms("50 50 01.8")
  end

  def test_lat_lon_two_doubles
    assert_equal [50.833833, 8.769333], @wgs84.lat_lon(50.833833, 8.769333)
  end

  def test_lat_lon_array_of_doubles
    assert_equal [3.5, 4.123], @wgs84.lat_lon([3.5, 4.123])
  end

  def test_lat_lon_array_of_strings
    assert_equal [3.5, 4.123], @wgs84.lat_lon(["3.5", "4.123"])
  end

  def test_lat_lon_alsfeld
    assert_equal [50.751778, 9.251861], @wgs84.lat_lon("50 45 6.4", "9 15 6.7")
  end

  def test_lat_lon_vilbel
    assert_equal [50.192028, 8.734722], @wgs84.lat_lon("50 11 31.3", "8 44 5.0")
  end

  def test_lat_lon_butzbach
    assert_equal [50.427417, 8.680056], @wgs84.lat_lon("50 25 38.7", "8 40 48.2")
  end

  def test_lat_lon_gilserberg
    assert_equal [50.946111, 9.070972], @wgs84.lat_lon("50 56 46.0", "9 4 15.5")
  end

  def test_lat_lon_gruenberg
    assert_equal [50.609694, 8.904167], @wgs84.lat_lon("50 36 34.9", "8 54 15.0")
  end

  def test_lat_lon_haiger
    assert_equal [50.761806, 8.156833], @wgs84.lat_lon("50 45 42.5", "8 9 24.6")
  end

  def test_lat_lon_herborn
    assert_equal [50.670333, 8.258556], @wgs84.lat_lon("50 40 13.2", "8 15 30.8")
  end

  def test_lat_lon_homberg
    assert_equal [50.743639, 9.019222], @wgs84.lat_lon("50 44 37.1", "9 1 9.2")
  end

  def test_lat_lon_limburg
    assert_equal [50.405778, 8.082222], @wgs84.lat_lon("50 24 20.8", "8 4 56.0")
  end

  def test_lat_lon_marburg
    assert_equal [50.833833, 8.769333], @wgs84.lat_lon("50 50 1.8", "8 46 9.6")
  end

  def test_lat_lon_pohlheim
    assert_equal [50.536833, 8.7125], @wgs84.lat_lon("50 32 12.6", "8 42 45.0")
  end

  def test_lat_lon_siegen
    assert_equal [50.853028, 8.000667], @wgs84.lat_lon("50 51 10.9", "8 0 2.4")
  end

  def test_lat_lon_wetzlar
    assert_equal [50.553778, 8.492278], @wgs84.lat_lon("50 33 13.6", "8 29 32.2")
  end
end

