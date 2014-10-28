/*
 * Ruby wrapper code for geodesic functions
 *
 * Copyright (c) 2014 Volker Wiegand <volker.wiegand@cvw.de>
 */

#include <ruby.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "geodesic.h"


static VALUE cWGS84;
static struct geod_geodesic g;


static VALUE
wgs84_init(VALUE klass)
{
  double a = 6378137, f = 1/298.257223563; /* WGS84 */

  geod_init(&g, a, f);

  return klass;
}


static double
wgs84_get_value(VALUE arg)
{
  char buf[64];
  int dd, mm, ss, ff;
  double dbl;

  if (TYPE(arg) == T_DATA) // assume BigDecimal and try to convert
    arg = rb_funcall(arg, rb_intern("to_f"), 0);

  if (TYPE(arg) == T_FLOAT)
    return NUM2DBL(arg);

  if (TYPE(arg) == T_FIXNUM)
    return (double) NUM2INT(arg);

  if (TYPE(arg) == T_STRING && RSTRING_LEN(arg) < 60) {
    memset(buf, 0, sizeof(buf));
    memcpy(buf, RSTRING_PTR(arg), RSTRING_LEN(arg));

    if (sscanf(buf, "%d %d %d.%d", &dd, &mm, &ss, &ff) == 4) {
      ff += (dd * 36000) + (mm * 600) + (ss * 10);
      dbl = (double) ff;
      dbl /= 36000.0;
      sprintf(buf, "%.6lf", dbl);
    }

    if (sscanf(buf, "%d.%d", &dd, &ff) == 2) {
      sscanf(buf, "%lf", &dbl);
      return dbl;
    }
  }

  rb_raise(rb_eArgError, "invalid (lat/lon) argument");
  return 0.0;
}


static VALUE
wgs84_as_deg(VALUE klass, VALUE arg)
{
  double val;

  val = wgs84_get_value(arg);

  return rb_float_new(val);
}


static VALUE
wgs84_as_dms(VALUE klass, VALUE arg)
{
  double val;
  char buf[64], *ptr;

  val = wgs84_get_value(arg);
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%d ", (int) trunc(val));
  ptr = buf + strlen(buf);
  sprintf(ptr, "%d ", (int) fmod(trunc(fabs(val) * 60.0), 60.0));
  ptr = buf + strlen(buf);
  sprintf(ptr, "%.1lf", fmod(fabs(val) * 3600.0, 60.0));

  return rb_str_new2(buf);
}


static VALUE
wgs84_as_bigdec(VALUE klass, VALUE arg)
{
  double val;
  char buf[64], *ptr;

  val = wgs84_get_value(arg);
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%07.0lf", round(val * 1000000.0));
  ptr = buf + (strlen(buf) - 6);
  memmove(ptr + 1, ptr, 6);
  *ptr = '.';

  return rb_funcall(rb_path2class("BigDecimal"), rb_intern("new"), 1, rb_str_new2(buf));
}


static VALUE
wgs84_lat_lon(int argc, VALUE *argv, VALUE klass)
{
  double lat, lon;
  VALUE tmp;

  if (argc == 2) {
    lat = wgs84_get_value(argv[0]);
    lon = wgs84_get_value(argv[1]);
    return rb_ary_new3(2L, rb_float_new(lat), rb_float_new(lon));
  }

  if (argc != 1) {
    rb_raise(rb_eArgError, "wrong number of arguments");
    return Qnil;
  }

  if (TYPE(*argv) == T_ARRAY && RARRAY_LEN(*argv) == 2) {
    lat = wgs84_get_value(rb_ary_entry(*argv, 0));
    lon = wgs84_get_value(rb_ary_entry(*argv, 1));
    return rb_ary_new3(2L, rb_float_new(lat), rb_float_new(lon));
  }

  tmp = rb_funcall(*argv, rb_intern("to_lat_lon"), 0);
  lat = wgs84_get_value(rb_ary_entry(tmp, 0));
  lon = wgs84_get_value(rb_ary_entry(tmp, 1));
  return rb_ary_new3(2L, rb_float_new(lat), rb_float_new(lon));
}


static VALUE
wgs84_distance(int argc, VALUE *argv, VALUE klass)
{
  int i;
  double dbl[4], azi1, azi2, s12;
  VALUE tmp1, tmp2;

  if (argc == 4) {
    for (i = 0; i < 4; i++)
      dbl[i] = wgs84_get_value(argv[i]);
    geod_inverse(&g, dbl[0], dbl[1], dbl[2], dbl[3], &s12, &azi1, &azi2);
    return rb_ary_new3(2L, INT2NUM((int) round(s12)), INT2NUM((int) azi1));
  }

  if (argc == 1 && TYPE(*argv) == T_ARRAY && RARRAY_LEN(*argv) == 4) {
    for (i = 0; i < 4; i++)
      dbl[i] = wgs84_get_value(rb_ary_entry(*argv, i));
    geod_inverse(&g, dbl[0], dbl[1], dbl[2], dbl[3], &s12, &azi1, &azi2);
    return rb_ary_new3(2L, INT2NUM((int) round(s12)), INT2NUM((int) azi1));
  }

  if (argc == 2) {
    tmp1 = rb_check_array_type(argv[0]);
    tmp2 = rb_check_array_type(argv[1]);
    if (!NIL_P(tmp1) && !NIL_P(tmp2)) {
      dbl[0] = wgs84_get_value(rb_ary_entry(tmp1, 0));
      dbl[1] = wgs84_get_value(rb_ary_entry(tmp1, 1));
      dbl[2] = wgs84_get_value(rb_ary_entry(tmp2, 0));
      dbl[3] = wgs84_get_value(rb_ary_entry(tmp2, 1));
      geod_inverse(&g, dbl[0], dbl[1], dbl[2], dbl[3], &s12, &azi1, &azi2);
      return rb_ary_new3(2L, INT2NUM((int) round(s12)), INT2NUM((int) azi1));
    }
  }

  if (argc == 1 && TYPE(*argv) == T_ARRAY && RARRAY_LEN(*argv) == 2) {
    tmp1 = rb_check_array_type(rb_ary_entry(*argv, 0));
    tmp2 = rb_check_array_type(rb_ary_entry(*argv, 1));
    if (!NIL_P(tmp1) && !NIL_P(tmp2)) {
      dbl[0] = wgs84_get_value(rb_ary_entry(tmp1, 0));
      dbl[1] = wgs84_get_value(rb_ary_entry(tmp1, 1));
      dbl[2] = wgs84_get_value(rb_ary_entry(tmp2, 0));
      dbl[3] = wgs84_get_value(rb_ary_entry(tmp2, 1));
      geod_inverse(&g, dbl[0], dbl[1], dbl[2], dbl[3], &s12, &azi1, &azi2);
      return rb_ary_new3(2L, INT2NUM((int) round(s12)), INT2NUM((int) azi1));
    }
  }

  rb_raise(rb_eArgError, "wrong number of arguments");
  return Qnil;
}


static VALUE
wgs84_average(VALUE klass, VALUE array, VALUE target)
{
  VALUE tmp, arr_val;
  double dst_lat, dst_lon, arr_lat, arr_lon, azi1, azi2, s12;
  double arr_min = 0.0, arr_max = 0.0, arr_sum = 0.0;
  int cnt, arr_azi = 0;

  tmp = rb_check_array_type(target);
  if (NIL_P(tmp))
    rb_raise(rb_eArgError, "invalid (target) argument");
  dst_lat = wgs84_get_value(rb_ary_entry(tmp, 0));
  dst_lon = wgs84_get_value(rb_ary_entry(tmp, 1));

  arr_val = rb_check_convert_type(array, T_ARRAY, "Array", "to_a");
  for (cnt = 0; cnt < RARRAY_LEN(arr_val); cnt++) {
    tmp = rb_check_array_type(rb_ary_entry(arr_val, cnt));
    if (NIL_P(tmp))
      rb_raise(rb_eArgError, "invalid (array) argument");
    arr_lat = wgs84_get_value(rb_ary_entry(tmp, 0));
    arr_lon = wgs84_get_value(rb_ary_entry(tmp, 1));
    geod_inverse(&g, arr_lat, arr_lon, dst_lat, dst_lon, &s12, &azi1, &azi2);
    s12 = round(s12);
    if (arr_min == 0.0 || s12 < arr_min)
      arr_min = s12;
    if (arr_max == 0.0 || s12 > arr_max)
      arr_max = s12;
    arr_sum += s12;
    arr_azi += (int) azi1;
  }
  s12  = round(round(arr_sum / (double) cnt) / 1000.0);
  azi1 = round(round((arr_min + arr_max) / 2.0) / 1000.0);

  return rb_ary_new3(3L, INT2NUM((int) s12), INT2NUM((int) azi1), INT2NUM(arr_azi / cnt));
}


static VALUE
wgs84_center(VALUE klass, VALUE array, VALUE center)
{
  VALUE tmp, arr_val;
  double min_lat, min_lon, max_lat, max_lon, tmp_lat, tmp_lon;
  int cnt;

  tmp = rb_check_array_type(center);
  if (NIL_P(tmp))
    rb_raise(rb_eArgError, "invalid (default) argument");
  min_lat = max_lat = wgs84_get_value(rb_ary_entry(tmp, 0));
  min_lon = max_lon = wgs84_get_value(rb_ary_entry(tmp, 1));

  arr_val = rb_check_convert_type(array, T_ARRAY, "Array", "to_a");
  for (cnt = 0; cnt < RARRAY_LEN(arr_val); cnt++) {
    tmp = rb_check_array_type(rb_ary_entry(arr_val, cnt));
    if (NIL_P(tmp))
      rb_raise(rb_eArgError, "invalid (array) argument");
    tmp_lat = wgs84_get_value(rb_ary_entry(tmp, 0));
    tmp_lon = wgs84_get_value(rb_ary_entry(tmp, 1));
    if (tmp_lat < min_lat)
      min_lat = tmp_lat;
    if (tmp_lon < min_lon)
      min_lon = tmp_lon;
    if (tmp_lat > max_lat)
      max_lat = tmp_lat;
    if (tmp_lon > max_lon)
      max_lon = tmp_lon;
  }
  min_lat = (min_lat + max_lat) / 2.0;
  min_lon = (min_lon + max_lon) / 2.0;
  return rb_ary_new3(2L, rb_float_new(min_lat), rb_float_new(min_lon));
}


void
Init_geodesic_wgs84(void)
{
  cWGS84 = rb_define_class("Wgs84", rb_cObject);
  rb_define_method(cWGS84, "initialize",  wgs84_init,          0);
  rb_define_method(cWGS84, "as_deg",      wgs84_as_deg,        1);
  rb_define_method(cWGS84, "as_dms",      wgs84_as_dms,        1);
  rb_define_method(cWGS84, "as_bigdec",   wgs84_as_bigdec,     1);
  rb_define_method(cWGS84, "lat_lon",     wgs84_lat_lon,      -1);
  rb_define_method(cWGS84, "distance",    wgs84_distance,     -1);
  rb_define_method(cWGS84, "average",     wgs84_average,       2);
  rb_define_method(cWGS84, "center",      wgs84_center,        2);
}

