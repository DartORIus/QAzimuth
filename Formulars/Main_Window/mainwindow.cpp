#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QLabel>
#include <QDir>
#include <QTranslator>
#include <QAction>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "../NMEA_DIALOG/dialog.h"
#include "../../parse_nmea.h"
#include "../show_parse.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), Settings("Navis-Ukraine/QAzimuth", "QAzimuth"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialog = new Dialog(Settings);
    setCentralWidget(dialog);
    QAction *dialog_Action = ui->menuView->addAction("Dialog");
    dialog_Action->setCheckable(true);
    dialog_Action->setChecked(true);
    connect(dialog_Action, SIGNAL(triggered(bool)), dialog, SLOT(setVisible(bool)));

    coord = new Coord_QW(Settings);
    coord->setWindowTitle("Coord");
    QAction *Coord_Action = ui->menuView->addAction("Coord");
    connect(Coord_Action, SIGNAL(triggered()), coord, SLOT(show()));

    serial_1   = new QSerialPort(this);
    settings_1 = new SettingsDialog(Settings, QString("Port_1"));

    parse_nmea = new Parse_NMEA(ui->menuNMEA);

    CHT = new Course_Heeling_Trim();
    CHT->setWindowTitle("Course Roll Pitch");
    QAction *CHT_Action = ui->menuView->addAction("Course Roll Pitch");
    connect(CHT_Action, SIGNAL(triggered()), CHT, SLOT(show()));

    pohpr = new POHPR_W();
    pohpr->setWindowTitle("POHPR");
    QAction *pohpr_Action = ui->menuView->addAction("POHPR");
    connect(pohpr_Action, SIGNAL(triggered()), pohpr, SLOT(show()));

    POUGT_F = new POUGT_Formular();
    POUGT_F->setWindowTitle("POUGT");
    QAction *pougt_Action = ui->menuView->addAction("POUGT");
    connect(pougt_Action, SIGNAL(triggered()), POUGT_F, SLOT(show()));

    Show_parse *Show_Parse_NMEA = new Show_parse;

    RMC_F = new RMC_Formular();
    RMC_F->setWindowTitle("RMC");
    QAction *RMC_Action = ui->menuView->addAction("RMC");
    connect(RMC_Action, SIGNAL(triggered()), RMC_F, SLOT(show()));

    XY_Wid = new XY_Area();
    QAction *XY_graph_Actin = ui->menuView->addAction("XY_Formular");
    connect(XY_graph_Actin, SIGNAL(triggered()), XY_Wid, SLOT(show()));

    ui->actionConnect_1->setEnabled(true);
    ui->actionDisconnect_1->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure_1->setEnabled(true);

    initActionsConnections();

    connect(serial_1,           SIGNAL(error(QSerialPort::SerialPortError)),
            this,               SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial_1,           SIGNAL(readyRead()),
            this,               SLOT(readDatafromPort_1()));

    connect(dialog,             SIGNAL(Write_NMEA_Data_SIGNAL(const QString &)),
            this,               SLOT(write_NMEA_Data_SLOT(const QString &)));

    connect(this,               SIGNAL(Show_NMEA_Text(const QByteArray &)),
            dialog,             SLOT(Show_NMEA_Text(const QByteArray &)));

    connect(dialog,             SIGNAL(Parse_NMEA_Signal(const QString &)),
            parse_nmea,         SLOT(Parse(const QString &)));

    connect(dialog,             SIGNAL(Enable_Connect(bool)),
            this,               SLOT(Enable_Connect(bool)));

    connect(dialog,             SIGNAL(Show_Hide_Signal(bool)),
            this,               SLOT(Show_Hide_Slot(bool)));

    connect(dialog,             SIGNAL(Push_Read_Buton_Signal(bool)),
            pohpr,              SLOT(Push_Read_Buton_Slot(bool)));
    connect(dialog,             SIGNAL(Read_All_Button_Signal()),
            pohpr,              SLOT(Read_All_Button_Slot()));
    connect(dialog,             SIGNAL(Clear_Signal()),
            pohpr,              SLOT(Clear_Slot()));
    connect(dialog,             SIGNAL(Clear_Signal()),
            XY_Wid,             SLOT(Clear()));

    connect(pohpr,              SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(POUGT_F,            SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(RMC_F,              SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));

    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR &)),
            Show_Parse_NMEA,    SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,         SIGNAL(Parse_GPGGA_Signal(const GPGGA&)),
            coord,              SLOT(Parse_GPGGA_Slot(const GPGGA&)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            CHT,                            SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            pohpr,                          SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR &)),
            this,                           SLOT(ZDA_Slot(const POHPR &)));

    connect(parse_nmea, SIGNAL(Parse_POUGT_Signal(const POUGT&)),
            POUGT_F,    SLOT(Parse_POUGT_Slot(const POUGT&)));
    connect(dialog,     SIGNAL(Clear_Signal()),
            POUGT_F,    SLOT(Clear_Slot()));
    connect(dialog,     SIGNAL(Push_Read_Buton_Signal(bool)),
            POUGT_F,    SLOT(Push_Read_Buton_Slot(bool)));
    connect(dialog,     SIGNAL(Read_All_Button_Signal()),
            POUGT_F,    SLOT(Read_All_Button_Slot()));

    connect(POUGT_F,    SIGNAL(Find_Point_Signal(int)),
            pohpr,      SLOT(Find_Point_Slot(int)));
    connect(pohpr,      SIGNAL(Find_Point_Signal(int)),
            RMC_F,      SLOT(Find_Point_Slot(int)));
    connect(pohpr,      SIGNAL(Find_Point_Signal(int)),
            POUGT_F,    SLOT(Find_Point_Slot(int)));

    connect(parse_nmea, SIGNAL(Parse_GPRMC_Signal(const GPRMC&)),
            RMC_F,      SLOT(Parse_GPRMC_Signal(const GPRMC&)));
    connect(dialog,     SIGNAL(Clear_Signal()),
            RMC_F,      SLOT(Clear_Slot()));
    connect(dialog,     SIGNAL(Push_Read_Buton_Signal(bool)),
            RMC_F,      SLOT(Push_Read_Buton_Slot(bool)));
    connect(dialog,     SIGNAL(Read_All_Button_Signal()),
            RMC_F,      SLOT(Read_All_Button_Slot()));

    ReadSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *)
{
    WriteSettings();
    delete(settings_1);
    delete(dialog);
    delete(pohpr);
    delete(coord);
    delete(CHT);
    delete(POUGT_F);
    delete(parse_nmea);
    delete(RMC_F);
    delete(XY_Wid);
}

void MainWindow::ReadSettings()
{
    Settings.beginGroup("/Windows");
        dialog      ->setHidden(Settings.value("/NMEA_DIalog").toBool());
        coord       ->setHidden(Settings.value("/Coord").toBool());
        CHT         ->setHidden(Settings.value("/CHT").toBool());
        pohpr       ->setHidden(Settings.value("/POHPR").toBool());
        POUGT_F     ->setHidden(Settings.value("/POUGT_Formular").toBool());
        RMC_F       ->setHidden(Settings.value("/RMC_Formular").toBool());
    Settings.endGroup();
}

void MainWindow::WriteSettings()
{
    Settings.beginGroup("/Date");
        QString  date(__DATE__);
        date += QString(__TIME__);
        Settings.setValue("Date", date);
    Settings.endGroup();

    Settings.beginGroup("/Windows");
        Settings.setValue("/NMEA_DIalog",   dialog->isHidden());
        Settings.setValue("/Coord",         coord->isHidden());
        Settings.setValue("/CHT",           CHT->isHidden());
        Settings.setValue("/POHPR",         pohpr->isHidden());
        Settings.setValue("/POUGT_Formular",POUGT_F->isHidden());
        Settings.setValue("/RMC_Formular",  RMC_F->isHidden());
    Settings.endGroup();
}

void MainWindow::openSerialPort_1()
{
    SettingsDialog::Settings p = settings_1->settings();
    serial_1->setPortName(p.name);
    if (serial_1->open(QIODevice::ReadWrite)) {
        if (serial_1->setBaudRate(p.baudRate)
                && serial_1->setDataBits(p.dataBits)
                && serial_1->setParity(p.parity)
                && serial_1->setStopBits(p.stopBits)
                && serial_1->setFlowControl(p.flowControl)) {

            dialog->setEnabled(true);
            dialog->Read_Button_Settings(false);
            dialog->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect_1->setEnabled(false);
            ui->actionDisconnect_1->setEnabled(true);
            ui->actionConfigure_1->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

            dialog->Disable_Enable_Send(true);
        } else {
            serial_1->close();
            QMessageBox::critical(this, tr("Error"), serial_1->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial_1->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }
}


void MainWindow::closeSerialPort_1()
{
    serial_1->close();
    ui->actionConnect_1->setEnabled(true);
    ui->actionDisconnect_1->setEnabled(false);
    ui->actionConfigure_1->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
    dialog->Read_Button_Settings(true);
    //pohpr->Push_Read_Buton_Slot(false);

    dialog->Disable_Enable_Send(false);
}

void MainWindow::about()
{
    QString  date(__DATE__);
    date += " " + QString(__TIME__);

    QString Version(VERSION_STRING);

    QMessageBox::about(this, tr("About QAzimuth") + " " + date,
                       tr("The <b>QAzimuth</b> demonstrates how to "
                          "use the devices made by Navis-Ukraine. ") + "<b>" + Version + "</b>");
}

void MainWindow::write_NMEA_Data_SLOT(const QString &str)
{
    QByteArray K = str.toLocal8Bit();
    serial_1->write(K);
}

void MainWindow::readDatafromPort_1()
{
    QByteArray data = serial_1->readAll();

    dialog->Show_NMEA_Text(data);
}

void MainWindow::Enable_Connect(bool Enable)
{
    ui->actionConnect_1->setEnabled(Enable);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        closeSerialPort_1();
        QMessageBox::critical(this, tr("Critical Error"), serial_1->errorString());
    }
}

void MainWindow::Show_Hide_Slot(bool enable)
{
    if(enable)
    {
        ReadSettings();
    }
    else
    {
        WriteSettings();

        pohpr->hide();
        coord->hide();
        CHT->hide();
        POUGT_F->hide();
    }
}

void MainWindow::Resize_Slot(const QSize &Size)
{
    QSize S = this->size();
    S.setWidth(S.width() - Size.width());
    this->setSizeIncrement(S);
}

QStringList MainWindow::Languiges()
{
    QDir dir("/home/orius/Qt5.1.1/prog/QtAzimuth-5.1.1/QtAzimuth_5/Languages/");
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);
    QMutableStringListIterator i(fileNames);
    while (i.hasNext())
    {
        i.next();
        i.setValue(dir.filePath(i.value()));
    }
    return fileNames;
}

void MainWindow::Language_Change(QAction* )
{
    QMessageBox::about(this, "QAzimuth", tr("You must restart program for set chosen language"));
}

void MainWindow::ZDA_Slot(const struct POHPR &)
{

}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect_1,    SIGNAL(triggered()), this,        SLOT(openSerialPort_1()));
    connect(ui->actionDisconnect_1, SIGNAL(triggered()), this,        SLOT(closeSerialPort_1()));
    connect(ui->actionQuit,         SIGNAL(triggered()), this,        SLOT(close()));
    connect(ui->actionConfigure_1,  SIGNAL(triggered()), settings_1,  SLOT(show()));
    //connect(ui->actionClear,      SIGNAL(triggered()), console,   SLOT(clear()));
    connect(ui->actionAbout,        SIGNAL(triggered()), this,        SLOT(about()));
    connect(ui->actionAboutQt,      SIGNAL(triggered()), qApp,        SLOT(aboutQt()));
}
