QT += core gui network webenginewidgets

TEMPLATE = app
TARGET = infotechx

INCLUDEPATH += ../o2/src

SOURCES += \
    downloader.cpp \
    formfiller.cpp \
    main.cpp \
    onedrive.cpp \
    tasker.cpp \
    uploader.cpp

HEADERS += \
    downloader.h \
    formfiller.h \
    onedrive.h \
    tasker.h \
    uploader.h

LIBS += $$OUT_PWD/../o2/src/libo2.a
