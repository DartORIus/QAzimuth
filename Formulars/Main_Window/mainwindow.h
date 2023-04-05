
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QFile>
#include <QDockWidget>
#include <QSettings>

#include "parse_nmea.h"

#include <Formulars/Koord/Coord_QW.h>
#include <Formulars/Course_Heeling_Trim/course_heeling_trim.h>
#include <Formulars/POHPR/POHPR.h>
#include <Formulars/RMC_Formular.h>
#include <Formulars/XY_Widget/XY_Area.h>

namespace Ui {
class MainWindow;
}


class Console;
class SettingsDialog;
class Panel_Buttons;
class Parse_NMEA;

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void Show_NMEA_Text(const QByteArray&);
    void Sent_NMEA(QString&);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void Show_Hide_Slot(bool);
private slots:
    void openSerialPort_1();
    void closeSerialPort_1();
    void about();
    void write_NMEA_Data_SLOT(const QString &);
    void readDatafromPort_1();
    void handleError(QSerialPort::SerialPortError error);
    void Enable_Connect(bool);
    void Resize_Slot(const QSize &);
    void Language_Change(QAction*);
    void ZDA_Slot(const struct POHPR &);
private:
    void initActionsConnections();
    void NMEA_Select();

    QSettings Settings;
    void ReadSettings();
    void WriteSettings();

    QStringList Languiges();
    QStringList Language_File;

    Parse_NMEA *parse_nmea;

    Ui::MainWindow *ui;
    Dialog              *dialog;
    Coord_QW            *coord;
    Course_Heeling_Trim *CHT;
    SettingsDialog      *settings_1;
    QSerialPort         *serial_1;
    Parse_NMEA          *parse_NMEA;
    POHPR_W             *pohpr;

    RMC_Formular        *RMC_F;

    XY_Area             *XY_Wid;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
