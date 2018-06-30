TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    rtmp_h264.c \
    main.c
linux-g++{
QMAKE_CFLAGS += -m32

QMAKE_LFLAGS += -m32

QMAKE_CXXFLAGS += -m32 \
                -Wno-narrowing

FFMPEG_DIR=/home/ln/ext_disk/ffmpeg_32bit/ffmpeg-3.2.2/_install

}

linux-hisiv300-gnueabi-g++{

FFMPEG_DIR=/home/ln/hisi/dual1080p/ff_lib


}

INCLUDEPATH += $${FFMPEG_DIR}/include



LIBS += -L$${FFMPEG_DIR}/lib \
        -lavformat \
        -lavcodec \
        -lswresample \
        -lavdevice \
        -lavfilter \
        -lavutil \
        -lpthread \
        -ldl

linux-g++{
LIBS += -lx264
}

HEADERS += \
    rtmp_h264.h \
    streamblock.h
