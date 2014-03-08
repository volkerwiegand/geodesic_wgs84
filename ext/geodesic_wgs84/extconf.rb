require "mkmf"

have_header 'math.h'
find_header 'geodesic.h'

abort "missing sscanf()" unless have_func "sscanf"
abort "missing memset()" unless have_func "memset"
abort "missing memcpy()" unless have_func "memcpy"
abort "missing strchr()" unless have_func "strchr"
abort "missing round()" unless have_func "round"

create_makefile 'geodesic_wgs84/geodesic_wgs84'

