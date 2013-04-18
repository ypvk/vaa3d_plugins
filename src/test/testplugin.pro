TEMPLATE      = lib
CONFIG       += qt plugin warn_off
#CONFIG       += release x86_64

V3DMAINDIR = /Users/yuping/yp/vaa3d/trunk/v3d_main

INCLUDEPATH  += $$V3DMAINDIR/basic_c_fun
INCLUDEPATH  += $$V3DMAINDIR/common_lib/include

HEADERS       = testplugin.h

SOURCES       = testplugin.cpp

LIBS         += -L$$V3DMAINDIR/common_lib/lib -lv3dtiff
#LIBS	     += -L/usr/local/lib/libfftw3f.a -L/usr/local/lib/libfftw3f_threads.a

TARGET        = $$qtLibraryTarget(testplugin)
DESTDIR       = /Users/yuping/yp/vaa3d/trunk/bin/plugins/vaa3d_plugins/testplugin
