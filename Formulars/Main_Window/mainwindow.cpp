#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QLabel>
#include <QDir>
#include <QTranslator>
#include <QAction>
#include <QCoreApplication>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "../NMEA_DIALOG/dialog.h"
#include "../../parse_nmea.h"
#include "../show_parse.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    Settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
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

    QAction* OpenFile_Action = new QAction(tr("&Open File"), this);
    ui->menuFile->addAction(OpenFile_Action);
    ui->menuFile->addSeparator();
    OpenFile_Action->setShortcut(QKeySequence::Open);
    connect(OpenFile_Action, SIGNAL(triggered()), dialog, SLOT(Open_File_Slot()));

    QAction* SaveFile_Action = new QAction(tr("&Save"), this);
    ui->menuFile->addAction(SaveFile_Action);
    ui->menuFile->addSeparator();
    SaveFile_Action->setShortcut(QKeySequence::Save);
    connect(SaveFile_Action, SIGNAL(triggered()), dialog, SLOT(Save_File_Slot()));

    QAction* Exit_Action = new QAction(tr("E&xit"), this);
    ui->menuFile->addAction(Exit_Action);
    Exit_Action->setShortcut(QKeySequence::Quit);
    connect(Exit_Action, SIGNAL(triggered()), this, SLOT(close()));

    portsNumber = Settings.value("serialPorts/number", 0).toInt();
    addPorts(portsNumber);
    initAddDeleteActions();
    ui->menuTools->addSeparator();
    QAction* Show_Hide_Toolbar_Action = new QAction("Show/Hide Tool Bar", this);
    ui->menuTools->addAction(Show_Hide_Toolbar_Action);
    connect(Show_Hide_Toolbar_Action, &QAction::triggered, this, [this](){
                ui->mainToolBar->setVisible(!(ui->mainToolBar->isVisible())); } );

    initActionsConnections();

    connect(dialog,             SIGNAL(Write_NMEA_Data_SIGNAL(const QString &, int)),
            this,               SLOT(write_NMEA_Data_SLOT(const QString &, int)));

//    connect(this,               SIGNAL(Show_NMEA_Text(const QByteArray &)),
//            dialog,             SLOT(Show_NMEA_Text(const QByteArray &)));

    connect(dialog,             SIGNAL(Parse_NMEA_Signal(const QString &)),
            parse_nmea,         SLOT(Parse(const QString &)));

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

//    connect(parse_nmea,         SIGNAL(Parse_POHPR_Signal(const POHPR &)),
//            this,               SLOT(ZDA_Slot(const POHPR &)));

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
    for (unsigned int i = 0; i < serialPorts.size(); ++i) {
        delete(serialPorts[i]);
    }
    for (unsigned int i = 0; i < serialPortActions.size(); ++i) {
        QVector<SerialPortAction*> asd = serialPortActions.at(i);
        for (int j = 0; j < asd.size(); ++j) {
            delete serialPortActions[i][j];
        }
    }
    delete paddAction;
    delete pDeleteAction;
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
    const QByteArray geometry = Settings.value("/MainWindow/geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
        (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    dialog      ->setHidden(Settings.value("/NMEA_DIalog/hidden").toBool());
    coord       ->setVisible(Settings.value("/Coord/hidden").toBool());
    CHT         ->setVisible(Settings.value("/CHT/hidden").toBool());
    pohpr       ->setVisible(Settings.value("/POHPR/hidden").toBool());
    POUGT_F     ->setVisible(Settings.value("/POUGT_Formular/hidden").toBool());
    RMC_F       ->setVisible(Settings.value("/RMC_Formular/hidden").toBool());
    Settings.endGroup();
    ui->mainToolBar->setVisible(Settings.value("/Toolbar/visible", true).toBool());
}

void MainWindow::WriteSettings()
{
    Settings.beginGroup("/Date");
        QString  date(__DATE__);
        date += QString(__TIME__);
        Settings.setValue("Date", date);
    Settings.endGroup();

    Settings.beginGroup("/Windows");
        Settings.setValue("/MainWindow/geometry",   saveGeometry());
        Settings.setValue("/NMEA_DIalog/hidden",    dialog->isHidden());
        Settings.setValue("/Coord/hidden",          coord->isVisible());
        Settings.setValue("/CHT/hidden",            CHT->isVisible());
        Settings.setValue("/POHPR/hidden",          pohpr->isVisible());
        Settings.setValue("/POUGT_Formular/hidden", POUGT_F->isVisible());
        Settings.setValue("/RMC_Formular/hidden",   RMC_F->isVisible());
    Settings.endGroup();
    Settings.setValue("serialPorts/number", portsNumber);
    Settings.setValue("/Toolbar/visible", ui->mainToolBar->isVisible());
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

void MainWindow::write_NMEA_Data_SLOT(const QString &str, int index)
{
    QByteArray K = str.toLocal8Bit();
    serialPorts[index]->getPort()->write(K);
}

void MainWindow::readDatafromPort(QSerialPort* port, int index)
{
    QByteArray data = port->readAll();
    dialog->Show_NMEA_Text(data, index+1);
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

//void MainWindow::ZDA_Slot(const struct POHPR &)
//{
//}

void MainWindow::addPorts(int number)
{
    int portsExist = serialPorts.size();
    for (int i = portsExist; i < number + portsExist; ++i) {
        serialPorts.push_back(new SettingsDialog(&Settings, i+1));

        QSerialPort* port = serialPorts[i]->getPort();
        connect(port, &QSerialPort::errorOccurred, this, [i, port, this](){
                    if (port->error() == QSerialPort::ResourceError)
                        handlePortError(i); } );

        connect(port, &QSerialPort::readyRead, this, [i, port, this](){ readDatafromPort(port, i); } );

        serialPortActions.push_back({
            new SerialPortAction(ICON[SerialPortAction::DISCONNECT], "Connect Serial Port", SerialPortAction::CONNECT, serialPorts[i], this),
            new SerialPortAction(ICON[SerialPortAction::SETTINGS], "Open Port Settings", SerialPortAction::SETTINGS, serialPorts[i], this)
        });
        for (int j = 0; j < serialPortActions[i].size(); ++j) {
            ui->mainToolBar->addAction(serialPortActions[i][j]);
            connect(serialPortActions[i][j], SIGNAL(triggered()), serialPortActions[i][j], SLOT(wasTriggered_slot()));
            connect(serialPortActions[i][j], SIGNAL(wasTriggered_signal(SerialPortAction*)), this, SLOT(actTriggered(SerialPortAction*)));
        }
        ui->mainToolBar->addSeparator();

        dialog->addTab("Port_"+QString::number(i+1));
    }
}

/* Adds new port group to the toolbar */
void MainWindow::addNewPort()
{
    ui->mainToolBar->removeAction(paddAction);
    ui->mainToolBar->removeAction(pDeleteAction);

    portsNumber += 1;
    addPorts(1);
    ui->mainToolBar->addAction(pDeleteAction);
    ui->mainToolBar->addAction(paddAction);
    pDeleteAction->setEnabled(true);
}

/* Creates actions "Delete port" and "Add port" */
void MainWindow::initAddDeleteActions()
{
    pDeleteAction = new SerialPortAction(ICON[SerialPortAction::DELETE], "Delete Port", SerialPortAction::DELETE, nullptr, this);
    connect(pDeleteAction, SIGNAL(triggered()), pDeleteAction, SLOT(wasTriggered_slot()));
    connect(pDeleteAction, SIGNAL(wasTriggered_signal(SerialPortAction*)), this, SLOT(actTriggered(SerialPortAction*)));
    pDeleteAction->setEnabled(false);
    if (portsNumber > 0) {
        ui->mainToolBar->addAction(pDeleteAction);
        pDeleteAction->setEnabled(true);
    }

    paddAction = new SerialPortAction(ICON[SerialPortAction::ADD], "Add New Serial Port", SerialPortAction::ADD, nullptr, this);
    connect(paddAction, SIGNAL(triggered()), paddAction, SLOT(wasTriggered_slot()));
    connect(paddAction, SIGNAL(wasTriggered_signal(SerialPortAction*)), this, SLOT(actTriggered(SerialPortAction*)));

    ui->mainToolBar->addAction(paddAction);
    ui->menuTools->addAction(paddAction);
    ui->menuTools->addAction(pDeleteAction);
}

void MainWindow::openSerialPort(SerialPortAction* action)
{
    QSerialPort::SerialPortError portError = action->getSettingsDialog()->openSerialPort();
    if (portError == QSerialPort::NoError) {
        SettingsDialog::portSettings p = action->getSettingsDialog()->settings();
        dialog->setEnabled(true);
        dialog->setLocalEchoEnabled(p.localEchoEnabled);
        action->setType(SerialPortAction::DISCONNECT);
        action->setIcon(ICON[SerialPortAction::CONNECT]);

        int index = 0;
        SettingsDialog* port = action->getSettingsDialog();
        while (port != serialPorts.at(index)) {
            ++index;
        }
        if (index ==  portsNumber - 1) {
            pDeleteAction->setEnabled(false);
        }
        QList<SerialPortAction*> asd = serialPortActions.at(index);
        for (int i = 0; i < asd.size(); ++i) {
            if (asd[i]->getType() == SerialPortAction::SETTINGS) {
                asd[i]->setEnabled(false);
            }
        }
        ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                   .arg(p.name).arg(p.baudRate).arg(p.dataBits)
                                   .arg(p.parity).arg(p.stopBits).arg(p.flowControl));
        emit dialog->portStateChanged_Signal(index + 1); // <- dialog has 1 additional tab 'main'
    } else if (portError == QSerialPort::OpenError) {
        QMessageBox::critical(this, tr("Error"), action->getSettingsDialog()->getPort()->errorString());
        ui->statusBar->showMessage(tr("Open error"));
    } else {
        QMessageBox::critical(this, tr("Error"), action->getSettingsDialog()->getPort()->errorString());
        ui->statusBar->showMessage(tr("Configure error"));
    }
}

/* Handle "ResourceError" of the port */
void MainWindow::handlePortError(int index)
{
    closeSerialPort(serialPortActions[index][0]);
    QMessageBox::critical(this, tr("Critical Error"), serialPorts[0]->getPort()->errorString());
}

void MainWindow::closeSerialPort(SerialPortAction* action)
{
    action->getSettingsDialog()->getPort()->close();
    action->setType(SerialPortAction::CONNECT);
    action->setIcon(ICON[SerialPortAction::DISCONNECT]);

    int index = 0;
    SettingsDialog* port = action->getSettingsDialog();
    while (port != serialPorts[index]) {
        ++index;
    }
    if (index ==  portsNumber - 1) {
        pDeleteAction->setEnabled(true);
    }
    QList<SerialPortAction*> act = serialPortActions[index];
    for (int i = 0; i < act.size(); ++i) {
        if (act[i]->getType() == SerialPortAction::SETTINGS) {
            act[i]->setEnabled(true);
        }
    }

    ui->statusBar->showMessage(tr("Disconnected"));
    //pohpr->Push_Read_Buton_Slot(false);
    emit dialog->portStateChanged_Signal(index + 1);
}

void MainWindow::deletePort()
{
    ui->mainToolBar->removeAction(paddAction);
    ui->mainToolBar->removeAction(pDeleteAction);
    ui->mainToolBar->removeAction(ui->mainToolBar->actions().last()); //separator
    pDeleteAction->setEnabled(false);

    QList<SerialPortAction*> portActions = serialPortActions.last();
    for (int i = 0; i < portActions.size(); ++i) {
        ui->mainToolBar->removeAction(portActions[i]);
        delete portActions[i];
    }

    delete serialPorts.last();
    serialPorts.pop_back();
    serialPortActions.pop_back();
    portsNumber -= 1;
    dialog->deleteTab();
    if (portsNumber > 0) {
        pDeleteAction->setEnabled(true);
        ui->mainToolBar->addAction(pDeleteAction);
        portActions = serialPortActions.last();
        for (int i = 0; i < portActions.size(); ++i) {
            if (portActions[i]->getType() == SerialPortAction::DISCONNECT) {
                pDeleteAction->setEnabled(false);
                break;
            }
        }
    }
    ui->mainToolBar->addAction(paddAction);
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
        addNewPort();
    } else if (action->getType() == SerialPortAction::DELETE) {
        deletePort();
    }
}

void MainWindow::initActionsConnections()
{
//    connect(ui->Exit_Action,         SIGNAL(triggered()), this,        SLOT(close()));
    //connect(ui->actionClear,      SIGNAL(triggered()), console,   SLOT(clear()));
    connect(ui->actionAbout,        SIGNAL(triggered()), this,        SLOT(about()));
    connect(ui->actionAboutQt,      SIGNAL(triggered()), qApp,        SLOT(aboutQt()));
}
