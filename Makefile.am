AUTOMAKE_OPTIONS=foreign 

bin_PROGRAMS= container

container_SOURCES=container.h container.c dateobj.h dateobj.c containerobj.h containerobj.c rootobj.h rootobj.c  \
cdmi_log.h cdmi_log.c cdmi_obj.h cdmi_obj.c request_analysis.h request_analysis.c
#container_LDADD = libmicrohttpd.a libjson.a
container_LDFLAGS = -lpthread -lglib-2.0 -lmicrohttpd -ljson -L/usr/bin $(all_libraries)
