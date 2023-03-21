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
    QMainWindow(parent), Settings("Navis-Ukraine/QAzimuth", "QAzimuth"), BINR_NMEA(0), read_Accel(false),
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

    serial_1 = new QSerialPort(this);
    serial_2 = new QSerialPort(this);

    settings_1    = new SettingsDialog(Settings, QString("Port_1"));
    settings_2    = new SettingsDialog(Settings, QString("Port_2"));

    parse_nmea = new Parse_NMEA(ui->menuNMEA);

    CHT = new Course_Heeling_Trim();
    CHT->setWindowTitle("Course Roll Pitch");
    QAction *CHT_Action = ui->menuView->addAction("Course Roll Pitch");
    connect(CHT_Action, SIGNAL(triggered()), CHT, SLOT(show()));

    pohpr = new POHPR_W();
    pohpr->setWindowTitle("POHPR");
    QAction *pohpr_Action = ui->menuView->addAction("POHPR");
    connect(pohpr_Action, SIGNAL(triggered()), pohpr, SLOT(show()));

    NMEA_Form = new QScrollArea();
    NMEA_Form->setWidget(dialog->Get_NMEA_Formular());
    NMEA_Form->setWindowTitle("NMEA Formular");
    NMEA_Form->setFixedWidth(470);
    NMEA_Form->setMaximumHeight(900);
    QAction *Scroll_Action = ui->menuView->addAction("NMEA Formular");
    connect(Scroll_Action, SIGNAL(triggered()), NMEA_Form, SLOT(show()));

    POUGT_F = new POUGT_Formular();
    POUGT_F->setWindowTitle("POUGT");
    QAction *pougt_Action = ui->menuView->addAction("POUGT");
    connect(pougt_Action, SIGNAL(triggered()), POUGT_F, SLOT(show()));

    Show_parse *Show_Parse_NMEA = new Show_parse;

    Accel_F = new Accel_Formular();
    QAction *Accel_Action = ui->menuView->addAction("Accel");
    connect(Accel_Action, SIGNAL(triggered()), Accel_F, SLOT(show()));

    Gyro_F = new Gyro_Formular();
    QAction *Gyro_Action = ui->menuView->addAction("Gyroscope");
    connect(Gyro_Action, SIGNAL(triggered()), Gyro_F, SLOT(show()));

    RMC_F = new RMC_Formular();
    RMC_F->setWindowTitle("RMC");
    QAction *RMC_Action = ui->menuView->addAction("RMC");
    connect(RMC_Action, SIGNAL(triggered()), RMC_F, SLOT(show()));

    Com_Accel_F = new Com_Accel();
    QAction *Com_Accel_Action = ui->menuView->addAction("Com_Accel");
    connect(Com_Accel_Action, SIGNAL(triggered()), Com_Accel_F, SLOT(show()));

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

    connect(serial_2,           SIGNAL(error(QSerialPort::SerialPortError)),
            this,               SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial_2,           SIGNAL(readyRead()),
            this,               SLOT(readDatafromPort_2()));

    connect(dialog,             SIGNAL(Write_NMEA_Data_SIGNAL(const QString &)),
            this,               SLOT(write_NMEA_Data_SLOT(const QString &)));

    connect(dialog,             SIGNAL(Write_BINR_Data_SIGNAL(const QString &)),
            this,               SLOT(write_BINR_Data_SLOT(const QString &)));

    connect(this,               SIGNAL(Show_NMEA_Text(const QByteArray &)),
            dialog,             SLOT(Show_NMEA_Text(const QByteArray &)));
    connect(this,               SIGNAL(Show_BINR_Text(const QByteArray &)),
            dialog,             SLOT(Show_BINR_Text(const QByteArray &)));

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

    connect(dialog,             SIGNAL(Clear_Signal()),
            Accel_F,            SLOT(Clear_Slot()));
    connect(dialog,             SIGNAL(Push_Read_Buton_Signal(bool)),
            Accel_F,            SLOT(Push_Read_Buton_Slot(bool)));
    connect(dialog,             SIGNAL(Read_All_Button_Signal()),
            Accel_F,            SLOT(Read_All_Button_Slot()));

    connect(dialog,             SIGNAL(Clear_Signal()),
            Gyro_F,             SLOT(Clear_Slot()));
    connect(dialog,             SIGNAL(Push_Read_Buton_Signal(bool)),
            Gyro_F,             SLOT(Push_Read_Buton_Slot(bool)));
    connect(dialog,             SIGNAL(Read_All_Button_Signal()),
            Gyro_F,             SLOT(Read_All_Button_Slot()));

    connect(pohpr,              SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(POUGT_F,            SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(RMC_F,              SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(Accel_F,            SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));
    connect(Gyro_F,             SIGNAL(Find_Text_Signal(double)),
            dialog,             SLOT(Find_Signal_Slot(double)));

    connect(Accel_F,            SIGNAL(Press_Cursor_Signal(QPoint)),
            Gyro_F,             SLOT(Press_Cursor_Slot(QPoint)));
    connect(Accel_F,            SIGNAL(Release_Cursor_Signal(QPoint)),
            Gyro_F,             SLOT(Release_Cursor_Slot(QPoint)));
    connect(Accel_F,            SIGNAL(Move_Cursor_Signal(QPoint)),
            Gyro_F,             SLOT(Move_Cursor_Slot(QPoint)));

    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR &)),
            Show_Parse_NMEA,    SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,         SIGNAL(Parse_GPGGA_Signal(const GPGGA&)),
            coord,              SLOT(Parse_GPGGA_Slot(const GPGGA&)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PODST(const POHPR&)));

    connect(parse_nmea,                     SIGNAL(Parse_PONDM_Signal(const PONDM&)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PONDM(const PONDM&)));

    connect(parse_nmea,                     SIGNAL(Parse_PORZA_Signal(const PORZA&)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PORZA(const PORZA&)));

    connect(parse_nmea,                     SIGNAL(Parse_PNVGO_Signal(const PNVGO&)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PNVGO(PNVGO)));

    connect(parse_nmea,                     SIGNAL(Parse_PNVGVER_Signal(const PNVGVER)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PNVGVER(PNVGVER)));

    connect(parse_nmea,                     SIGNAL(Parse_PNVGS_Signal(const PNVGS)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PNVGS(PNVGS)));

    connect(parse_nmea,                     SIGNAL(Parse_PNVGR_Signal(const PNVGR)),
            dialog->Get_NMEA_Formular(),    SLOT(Parse_PNVGR(PNVGR)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            CHT,                            SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,                     SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            pohpr,                          SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,                     SIGNAL(Parse_ACCEL_Signal(const ACCEL&)),
            Accel_F,                        SLOT(Parse_ACCEL_Slot(const ACCEL&)));
    connect(parse_nmea,                     SIGNAL(Parse_ACCEL_Signal(const ACCEL&)),
            Gyro_F,                         SLOT(Parse_ACCEL_Slot(const ACCEL&)));

    connect(parse_nmea,                     SIGNAL(Parse_ACCEL_Signal(const ACCEL&)),
            XY_Wid,                         SLOT(Parse_ACCEL_Slot(const ACCEL&)));

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

    connect(parse_nmea,  SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            Com_Accel_F, SLOT(Parse_POHPR_Slot(const POHPR&)));
    connect(Accel_F,     SIGNAL(Accel_Deg_SIGNAL(float, char)),
            Com_Accel_F, SLOT(Accel_Deg_SLOT(float, char)));

    connect(parse_nmea,  SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            this,        SLOT(ACCEL_TO_Dialog(const POHPR&)));

    ReadSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *)
{
    WriteSettings();
    delete(settings_1);
    delete(settings_2);
    delete(dialog);
    delete(pohpr);
    delete(coord);
    delete(CHT);
    delete(NMEA_Form);
    delete(POUGT_F);
    delete(Accel_F);
    delete(Gyro_F);
    delete(parse_nmea);
    delete(RMC_F);
    delete(Com_Accel_F);
    delete(XY_Wid);
}

void MainWindow::ReadSettings()
{
    Settings.beginGroup("/Windows");
        dialog      ->setHidden(Settings.value("/NMEA_DIalog").toBool());
        coord       ->setHidden(Settings.value("/Coord").toBool());
        CHT         ->setHidden(Settings.value("/CHT").toBool());
        pohpr       ->setHidden(Settings.value("/POHPR").toBool());
        NMEA_Form   ->setHidden(Settings.value("/NMEA_Formular").toBool());
        POUGT_F     ->setHidden(Settings.value("/POUGT_Formular").toBool());
    //    Accel_F     ->setHidden(Settings.value("/Accel_Formular").toBool());
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
        Settings.setValue("/NMEA_Formular", NMEA_Form->isHidden());
        Settings.setValue("/POUGT_Formular",POUGT_F->isHidden());
        Settings.setValue("/Accel_Formular",Accel_F->isHidden());
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

void MainWindow::openSerialPort_2()
{
    SettingsDialog::Settings p = settings_2->settings();
    serial_2->setPortName(p.name);
    if (serial_2->open(QIODevice::ReadWrite)) {
        if (serial_2->setBaudRate(p.baudRate)
                && serial_2->setDataBits(p.dataBits)
                && serial_2->setParity(p.parity)
                && serial_2->setStopBits(p.stopBits)
                && serial_2->setFlowControl(p.flowControl)) {

            dialog->setEnabled(true);
            dialog->Read_Button_Settings(false);
            dialog->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect_2->setEnabled(false);
            ui->actionDisconnect_2->setEnabled(true);
            ui->actionConfigure_2->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

            dialog->Disable_Enable_Send(true);
        } else {
            serial_2->close();
            QMessageBox::critical(this, tr("Error"), serial_2->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial_2->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }
}


void MainWindow::closeSerialPort_2()
{
    serial_2->close();
    ui->actionConnect_2->setEnabled(true);
    ui->actionDisconnect_2->setEnabled(false);
    ui->actionConfigure_2->setEnabled(true);
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
    BINR_NMEA = 0;
    QByteArray K = str.toLocal8Bit();
    serial_1->write(K);
}

void MainWindow::write_BINR_Data_SLOT(const QString &str)
{
    BINR_NMEA = 1;

    quint8 Size = (quint8)str.size()/2;
    quint8 *Z   = new quint8[Size];

    QString N = str;

    for(int i = 0; i < Size; i++)
    {
        N.resize(2);
        Z[i] = N.toUInt(0, 16);
        N = str;
        N.remove(0, i*2+2);
    }

    QByteArray K(Size, 0);

    memcpy(K.data(), Z, sizeof(quint8)*Size);

    serial_1->write(K);

    delete [] Z;
}

void MainWindow::write_BINR_Data_SLOT(const QByteArray &BINR)
{
    BINR_NMEA = 1;
    serial_1->write(BINR);
}


void MainWindow::readDatafromPort_1()
{
    QByteArray data = serial_1->readAll();

  //  if(BINR_NMEA == false)
    //    emit Show_NMEA_Text(data);
    dialog->Show_NMEA_Text(data);
//    else
   //     emit Show_BINR_Text(data);
}

void MainWindow::readDatafromPort_2()
{
    QByteArray data = serial_2->readAll();

    dialog->Show_NMEA_Text_1(data);
    QString Text  = QString::fromLocal8Bit(data);

    parse_nmea->Parse_Accel_Slot(Text, Accel);

    Accel_F->Filter_Accel(Accel);
}

void MainWindow::ACCEL_TO_Dialog(const struct POHPR &)
{
    struct ACCEL A = Accel_F->Make_NMEA();

    if(A.Status == 'A')
    {
        QByteArray data = parse_nmea->Make_NMEA(A);
        dialog->Show_NMEA_Text(data);
    }
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
        NMEA_Form->hide();
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
    read_Accel = true;
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect_1,    SIGNAL(triggered()), this,        SLOT(openSerialPort_1()));
    connect(ui->actionDisconnect_1, SIGNAL(triggered()), this,        SLOT(closeSerialPort_1()));
    connect(ui->actionConnect_2,    SIGNAL(triggered()), this,        SLOT(openSerialPort_2()));
    connect(ui->actionDisconnect_2, SIGNAL(triggered()), this,        SLOT(closeSerialPort_2()));
    connect(ui->actionQuit,         SIGNAL(triggered()), this,        SLOT(close()));
    connect(ui->actionConfigure_1,  SIGNAL(triggered()), settings_1,  SLOT(show()));
    connect(ui->actionConfigure_2,  SIGNAL(triggered()), settings_2,  SLOT(show()));
    //connect(ui->actionClear,      SIGNAL(triggered()), console,   SLOT(clear()));
    connect(ui->actionAbout,        SIGNAL(triggered()), this,        SLOT(about()));
    connect(ui->actionAboutQt,      SIGNAL(triggered()), qApp,        SLOT(aboutQt()));
}
