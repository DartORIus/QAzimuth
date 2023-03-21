
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
#include <Formulars/POUGT/POUGT_Formular.h>
#include <Formulars/Accel_Formular.h>
#include <Formulars/Gyro_Formular.h>
#include <Formulars/RMC_Formular.h>
#include <Formulars/Com_Accel/Com_Accel.h>
#include <Formulars/XY_Widget/XY_Area.h>
#include "language.h"

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
    void Show_BINR_Text(const QByteArray&);
    void Sent_NMEA(QString&);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void Show_Hide_Slot(bool);
private slots:
    void openSerialPort_1();
    void closeSerialPort_1();
    void openSerialPort_2();
    void closeSerialPort_2();
    void about();
    void write_NMEA_Data_SLOT(const QString &);
    void write_BINR_Data_SLOT(const QString &);
    void write_BINR_Data_SLOT(const QByteArray &);
    void readDatafromPort_1();
    void readDatafromPort_2();
    void ACCEL_TO_Dialog(const struct POHPR &);
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

    bool BINR_NMEA;
    bool read_Accel;
    Ui::MainWindow *ui;
    Dialog              *dialog;
    Coord_QW            *coord;
    Course_Heeling_Trim *CHT;
    SettingsDialog      *settings_1, *settings_2;
    QSerialPort         *serial_1, *serial_2;
    Parse_NMEA          *parse_NMEA;
    POHPR_W             *pohpr;

    QScrollArea         *NMEA_Form;

    POUGT_Formular      *POUGT_F;

    Accel_Formular      *Accel_F;
    Gyro_Formular       *Gyro_F;
    struct ACCEL         Accel;

    RMC_Formular        *RMC_F;

    Com_Accel           *Com_Accel_F;

    QByteArray          Accel_Byte;

    XY_Area             *XY_Wid;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
