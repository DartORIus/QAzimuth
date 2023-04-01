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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    Settings("Navis-Ukraine/QAzimuth", "QAzimuth"), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->clear();

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

    portsNumber = Settings.value("serialPorts/number", 1).toInt();
    qDebug () << portsNumber;
    addPorts(portsNumber);
    initAddAction();
    initActionsConnections();
/**** have to be fixed *****/
    connect(serialPorts[0]->getPort(),           SIGNAL(readyRead()),
            this,               SLOT(readDatafromPort()));

    connect(dialog,             SIGNAL(Write_NMEA_Data_SIGNAL(const QString &)),
            this,               SLOT(write_NMEA_Data_SLOT(const QString &)));
/************************************************/

    connect(this,               SIGNAL(Show_NMEA_Text(const QByteArray &)),
            dialog,             SLOT(Show_NMEA_Text(const QByteArray &)));

    connect(dialog,             SIGNAL(Parse_NMEA_Signal(const QString &)),
            parse_nmea,         SLOT(Parse(const QString &)));

//    connect(dialog,             SIGNAL(Enable_Connect(bool)),
//            this,               SLOT(Enable_Connect(bool)));

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

    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            CHT,                SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR&)),
            pohpr,              SLOT(Parse_POHPR_Slot(const POHPR &)));

    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR &)),
            this,               SLOT(ZDA_Slot(const POHPR &)));

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
    for (unsigned int i = 0; i < portsNumber; ++i) {
        delete(serialPorts[i]);
    }
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

void MainWindow::openSerialPort(SerialPortAction* action)
{
    QSerialPort::SerialPortError portError = action->getSettingsDialog()->openSerialPort();
    if (portError == QSerialPort::NoError) {
        SettingsDialog::portSettings p = action->getSettingsDialog()->settings();
        dialog->setEnabled(true);
        dialog->Read_Button_Settings(false);
        dialog->setLocalEchoEnabled(p.localEchoEnabled);
        action->setType(SerialPortAction::DISCONNECT);
        action->setIcon(ICON[SerialPortAction::CONNECT]);
//        ui->actionConfigure_1->setEnabled(false);     // <- fix
        ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                   .arg(p.name).arg(p.baudRate).arg(p.dataBits)
                                   .arg(p.parity).arg(p.stopBits).arg(p.flowControl));
        dialog->Disable_Enable_Send(true);
    } else if (portError == QSerialPort::OpenError) {
        QMessageBox::critical(this, tr("Error"), action->getSettingsDialog()->getPort()->errorString());
        ui->statusBar->showMessage(tr("Open error"));
    } else {
        QMessageBox::critical(this, tr("Error"), action->getSettingsDialog()->getPort()->errorString());
        ui->statusBar->showMessage(tr("Configure error"));
    }
}

void MainWindow::closeSerialPort(SerialPortAction* action)
{
    action->getSettingsDialog()->getPort()->close();
    action->setType(SerialPortAction::CONNECT);
    action->setIcon(ICON[SerialPortAction::DISCONNECT]);
//    ui->actionConfigure_1->setEnabled(true);      // <- fix
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
                          "use the devices made by Navis-Ukraine. ") + "<br><b>" + Version + "</b>");
}

/* have to be changed */
void MainWindow::write_NMEA_Data_SLOT(const QString &str)
{
    QByteArray K = str.toLocal8Bit();
    serialPorts[0]->getPort()->write(K);
}

void MainWindow::readDatafromPort()
{
    QByteArray data = serialPorts[0]->getPort()->readAll();

    dialog->Show_NMEA_Text(data);
}
/*********************************************/

//void MainWindow::Enable_Connect(bool Enable)
//{
//    ui->actionConnect_1->setEnabled(Enable);
//}

//void MainWindow::portErrorOccurred(QSerialPort::SerialPortError error)
//{
//    if (error == QSerialPort::ResourceError)
//    {
//        emit handlePortError()
//    }
//}

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

void MainWindow::addPorts(unsigned int number)
{
    int portsExist = serialPortActions.size();
    for (unsigned int i = portsExist; i < number + portsExist; ++i) {
        serialPorts.push_back(new SettingsDialog(&Settings, i+1));

//        connect(serialPorts[i]->getPort(), SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
//                this, SLOT(handleError(QSerialPort::SerialPortError)));

        serialPortActions.push_back({new SerialPortAction(ICON[SerialPortAction::DISCONNECT], "Connect serial port", SerialPortAction::CONNECT, serialPorts[i], this),
                                                new SerialPortAction(ICON[SerialPortAction::SETTINGS], "Open port settings", SerialPortAction::SETTINGS, serialPorts[i], this),
                                                new SerialPortAction(ICON[SerialPortAction::DELETE], "Delete port", SerialPortAction::DELETE, serialPorts[i], this)});
        for (int j = 0; j < serialPortActions[i].size(); ++j) {
            ui->mainToolBar->addAction(serialPortActions[i][j]);

            connect(serialPortActions[i][j], SIGNAL(triggered()), serialPortActions[i][j], SLOT(slot1()));
            connect(serialPortActions[i][j], SIGNAL(wasTriggered(SerialPortAction*)), this, SLOT(actTriggered(SerialPortAction*)));
        }
        ui->mainToolBar->addSeparator();
    }
}

void MainWindow::addNewPort(SerialPortAction* action)
{
    ui->mainToolBar->removeAction(action);
    portsNumber += 1;
    addPorts(1);
    initAddAction();
}

void MainWindow::initAddAction()
{
    paddAction = new SerialPortAction(ICON[SerialPortAction::ADD], "Add new serial port", SerialPortAction::ADD, nullptr, this);
    ui->mainToolBar->addAction(paddAction);
    connect(paddAction, SIGNAL(triggered()), paddAction, SLOT(slot1()));
    connect(paddAction, SIGNAL(wasTriggered(SerialPortAction*)), this, SLOT(actTriggered(SerialPortAction*)));
}

/* Currently removes only the last in the list serial port */
void MainWindow::deletePort()
{
    if (serialPorts.size() <= 1) {
        return;
    }
    ui->mainToolBar->removeAction(paddAction);

    for (int j = 0; j < serialPortActions.last().size(); ++j) {
        ui->mainToolBar->removeAction(serialPortActions.last()[j]);
    }
    serialPorts.pop_back();
    serialPortActions.pop_back();
    portsNumber -= 1;
    initAddAction();
}

void MainWindow::actTriggered(SerialPortAction* action)
{
    if (action->getType() == SerialPortAction::CONNECT) {
        openSerialPort(action);
    } else if (action->getType() == SerialPortAction::SETTINGS) {
        action->getSettingsDialog()->show();
    } else if (action->getType() == SerialPortAction::DISCONNECT) {
        closeSerialPort(action);
    } else if (action->getType() == SerialPortAction::ADD) {
        addNewPort(action);
    } else if (action->getType() == SerialPortAction::DELETE) {
        deletePort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionQuit,         SIGNAL(triggered()), this,        SLOT(close()));
    //connect(ui->actionClear,      SIGNAL(triggered()), console,   SLOT(clear()));
    connect(ui->actionAbout,        SIGNAL(triggered()), this,        SLOT(about()));
    connect(ui->actionAboutQt,      SIGNAL(triggered()), qApp,        SLOT(aboutQt()));
}
