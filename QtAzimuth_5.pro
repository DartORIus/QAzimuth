#-------------------------------------------------
#
# Project created by QtCreator 2013-12-12T09:51:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += serialport

TARGET = QtAzimuth_5
TEMPLATE = app

SOURCES += \
    parse_nmea.cpp \
    main.cpp \
    Formulars/show_parse.cpp \
    Formulars/Course_Heeling_Trim/course_heeling_trim.cpp \
    Formulars/Main_Window/settingsdialog.cpp \
    Formulars/Main_Window/mainwindow.cpp \
    Formulars/Course_Heeling_Trim/Course/Course_QW.cpp \
    Formulars/Course_Heeling_Trim/Course/Heeling_QW.cpp \
    Formulars/Course_Heeling_Trim/Course/Trim_QW.cpp \
    Formulars/NMEA_DIALOG/Show_Text.cpp \
    lib/parsenmea.c \
    Formulars/NMEA_DIALOG/higlighter.cpp \
    Formulars/Koord/Coord_QW.cpp \
    Formulars/Koord/Visual_Coord.cpp \
    Formulars/NMEA_DIALOG/dialog.cpp \
    Formulars/POUGT/POUGT_Formular.cpp \
    Formulars/POHPR/POHPR.cpp \
    Graphic/Analiz.cpp \
    Graphic/Control_Graf.cpp \
    Graphic/Data_Graf.cpp \
    Graphic/graphic.cpp \
    Graphic/Graphic_Widget.cpp \
    Formulars/RMC_Formular.cpp \
    Formulars/XY_Widget/XY_Widget.cpp \
    Formulars/XY_Widget/XY_Graphic.cpp \
    Formulars/XY_Widget/XY_Area.cpp

HEADERS  += \
    parse_nmea.h \
    Formulars/show_parse.h \
    Formulars/Course_Heeling_Trim/course_heeling_trim.h \
    Formulars/Main_Window/settingsdialog.h \
    Formulars/Main_Window/mainwindow.h \
    Formulars/Course_Heeling_Trim/Course/Course_QW.h \
    Formulars/Course_Heeling_Trim/Course/Heeling_QW.h \
    Formulars/Course_Heeling_Trim/Course/Trim_QW.h \
    Formulars/NMEA_DIALOG/Show_Text.h \
    lib/NMEA_Struct.h \
    Formulars/NMEA_DIALOG/higlighter.h \
    Formulars/Koord/Coord_QW.h \
    Formulars/Koord/Visual_Coord.h \
    Formulars/NMEA_DIALOG/dialog.h \
    Formulars/POUGT/POUGT_Formular.h \
    Formulars/POHPR/POHPR.h \
    Graphic/Analiz.h \
    Graphic/Control_Graf.h \
    Graphic/Data_Graf.h \
    Graphic/graphic.h \
    Graphic/Graphic_Widget.h \
    Formulars/RMC_Formular.h \
    Formulars/XY_Widget/XY_Widget.h \
    Formulars/XY_Widget/XY_Graphic.h \
    Formulars/XY_Widget/XY_Area.h

INCLUDEPATH += ./

RESOURCES += \
    terminal.qrc

FORMS += \
    Formulars/Main_Window/settingsdialog.ui \
    Formulars/Main_Window/mainwindow.ui \
    Formulars/XY_Widget/XY_Widget.ui \
    Formulars/XY_Widget/XY_Area.ui

LIBS = -lQt5SerialPort -lm

TRANSLATIONS += Languages/English.ts \
                Languages/Ukrainian.ts

VERS=$$system(git tag) $$system(git log -n 1)

QMAKE_CXXFLAGS += -Wextra -Wformat=2 -Winit-self -Wstrict-overflow=5 \
                  -Wcast-qual -Wcast-align -Wwrite-strings \
                  -Warray-bounds -Wno-missing-field-initializers \
                  -std=c++11 -pedantic -D_POSIX_C_SOURCE=200809L

DEFINES += "'VERSION_STRING=\"$$system(git tag) $$system(git log -n 1)\"'"