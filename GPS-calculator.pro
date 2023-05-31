QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Koord/Coord_QW.cpp \
    Koord/Visual_Coord.cpp \
    drawgps.cpp \
    lib/parsenmea.c \
    main.cpp

HEADERS += \
    Koord/Coord_QW.h \
    Koord/Visual_Coord.h \
    drawgps.h \
    lib/NMEA_Struct.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
