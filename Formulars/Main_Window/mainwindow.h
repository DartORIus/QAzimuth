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
#include <Formulars/RMC_Formular.h>
#include <Formulars/XY_Widget/XY_Area.h>
#include <Formulars/DrawGPS/drawgps.h>
#include "serialportaction.h"

extern const QIcon ICON[5];

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
    void Sent_NMEA(QString&);

public:
    const QIcon ICON[5] = {QIcon::fromTheme("", QIcon(":/images/connect.png")),
                        QIcon::fromTheme("", QIcon(":/images/disconnect.png")),
                        QIcon::fromTheme("", QIcon(":/images/settings.png")),
                        QIcon::fromTheme("", QIcon(":/images/add.png")),
                        QIcon::fromTheme("", QIcon(":/images/application-exit.png"))};

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void Show_Hide_Slot(bool);
    void actTriggered(SerialPortAction*);
    void addNewPort();

private slots:
    void closeSerialPort(SerialPortAction* action);
    void about();
    void write_NMEA_Data_SLOT(const QString &, int);
    void Resize_Slot(const QSize &);
    void Language_Change(QAction*);
//    void ZDA_Slot(const struct POHPR &);

private:
    void initAddDeleteActions();
    void addPorts(int);
    void deletePort();
    void openSerialPort(SerialPortAction*);
    void handlePortError(int);
    void initActionsConnections();
    void readDatafromPort(QSerialPort*, int);
    void NMEA_Select();
    void ReadSettings();
    void WriteSettings();

    Ui::MainWindow *ui;
    QSettings Settings;

    QStringList Languiges();
    QStringList Language_File;

    Parse_NMEA *parse_nmea;

    Dialog              *dialog;
    Coord_QW            *coord;
    Course_Heeling_Trim *CHT;
    Parse_NMEA          *parse_NMEA;
    POHPR_W             *pohpr;
    POUGT_Formular      *POUGT_F;
    RMC_Formular        *RMC_F;
    XY_Area             *XY_Wid;
    DrawGPS             *draw_GPS;

    int portsNumber;
    SerialPortAction* paddAction;
    SerialPortAction* pDeleteAction;
    QList<SettingsDialog*> serialPorts;
    QList<QList<SerialPortAction*>> serialPortActions;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
