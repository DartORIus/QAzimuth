#include "parse_nmea.h"
#include <QStringList>
#include <stdio.h>

Parse_NMEA::Parse_NMEA(QMenu *menu, QObject *parent) :
    QObject(parent)
{
    menuNMEA = menu;
    NMEA_Select();

    memset(&ctx, 0, sizeof(BmUartProtoNmea));
    memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));

    Read_Settings();
}

void Parse_NMEA::Parse(const QString &Text_NMEA)
{
    NMEA_D = NMEA_Data_Settings;
    char data[4096];
    memset(data, 0, 4096);

    strcpy(data, Text_NMEA.toLocal8Bit().data());

    nmea_recv(&ctx, data, strlen(data), &NMEA_D);
    if(NMEA_D.GPGGA.longitude && NMEA_D.GPGGA.latitude) {
        emit Parse_GPGGA_Signal(NMEA_D.GPGGA);
    }

    if(NMEA_D.POHPR.time && NMEA_D.POHPR.status)
        emit Parse_POHPR_Signal(NMEA_D.POHPR);

    if(NMEA_D.POUGT.time && NMEA_D.POUGT.status)
        emit Parse_POUGT_Signal(NMEA_D.POUGT);

    if(NMEA_D.PONDM.Distance_1)
        emit Parse_PONDM_Signal(NMEA_D.PONDM);

    if(NMEA_D.PORZA.Boudrate)
        emit Parse_PORZA_Signal(NMEA_D.PORZA);

    if(NMEA_D.PNVGO.Signal)
        emit Parse_PNVGO_Signal(NMEA_D.PNVGO);

    if(NMEA_D.PNVGVER.Signal)
        emit Parse_PNVGVER_Signal(NMEA_D.PNVGVER);

    if(NMEA_D.PNVGS.Signal)
        emit Parse_PNVGS_Signal(NMEA_D.PNVGS);

    if(NMEA_D.PNVGR.Status)
        emit Parse_PNVGR_Signal(NMEA_D.PNVGR);

    if(NMEA_D.RMC.Time && NMEA_D.RMC.Status)
        emit Parse_GPRMC_Signal(NMEA_D.RMC);

    if(NMEA_D.PORZV.Time && NMEA_D.PORZV.Stat)
        emit Parse_PORZV_Signal(NMEA_D.PORZV);

    if(NMEA_D.GPZDA.k == 1)
        emit Parse_GPZDA_Signal(NMEA_D.GPZDA);
}

Parse_NMEA::~Parse_NMEA()
{
    Settings.beginGroup("/NMEA_sentence");
        Settings.setValue("/GPGGA",   NMEA_Data_Settings.GPGGA.time);
        Settings.setValue("/POHPR",   NMEA_Data_Settings.POHPR.time);
        Settings.setValue("/POUGT",   NMEA_Data_Settings.POUGT.satellite);
        Settings.setValue("/PONDM",   NMEA_Data_Settings.PONDM.Course);
        Settings.setValue("/PORZA",   NMEA_Data_Settings.PORZA.Number);
        Settings.setValue("/PNVGO",   NMEA_Data_Settings.PNVGO.Signal);
        Settings.setValue("/PNVGVER", NMEA_Data_Settings.PNVGVER.Signal);
        Settings.setValue("/PNVGS",   NMEA_Data_Settings.PNVGS.Signal);
        Settings.setValue("/PNVGR",   NMEA_Data_Settings.PNVGR.Status);
        Settings.setValue("/RMC",     NMEA_Data_Settings.RMC.Time);
    Settings.endGroup();
}

void Parse_NMEA::Read_Settings()
{
    Settings.beginGroup("/NMEA_sentence");
        NMEA_Data_Settings.GPGGA.time       = Settings.value("/GPGGA").toBool();
        NMEA_Data_Settings.POHPR.time       = Settings.value("/POHPR").toBool();
        NMEA_Data_Settings.POUGT.satellite  = Settings.value("/POUGT").toBool();
        NMEA_Data_Settings.PONDM.Course     = Settings.value("/PONDM").toBool();
        NMEA_Data_Settings.PORZA.Number     = Settings.value("/PORZA").toBool();
        NMEA_Data_Settings.PNVGO.Signal     = Settings.value("/PNVGO").toBool();
        NMEA_Data_Settings.PNVGVER.Signal   = Settings.value("/PNVGVER").toBool();
        NMEA_Data_Settings.PNVGS.Signal     = Settings.value("/PNVGS").toBool();
        NMEA_Data_Settings.PNVGR.Status     = Settings.value("/RMC").toBool();
        NMEA_Data_Settings.RMC.Time         = Settings.value("/GPGGA").toBool();
    Settings.endGroup();

    Settings.beginGroup("/NMEA_sentence");
        NMEA_Actions[0] ->setChecked(Settings.value("/GPGGA").toBool());
        NMEA_Actions[1] ->setChecked(Settings.value("/POHPR").toBool());
        NMEA_Actions[2] ->setChecked(Settings.value("/POUGT").toBool());
        NMEA_Actions[3] ->setChecked(Settings.value("/PONDM").toBool());
        NMEA_Actions[4] ->setChecked(Settings.value("/PORZA").toBool());
        NMEA_Actions[5] ->setChecked(Settings.value("/PNVGO").toBool());
        NMEA_Actions[6] ->setChecked(Settings.value("/PNVGVER").toBool());
        NMEA_Actions[7] ->setChecked(Settings.value("/PNVGS").toBool());
        NMEA_Actions[8] ->setChecked(Settings.value("/RMC").toBool());
        NMEA_Actions[9] ->setChecked(Settings.value("/GPGGA").toBool());
    Settings.endGroup();
}

void Parse_NMEA::NMEA_Select()
{
    menuNMEA->addAction("GPGGA");
    menuNMEA->addAction("POHPR");
    menuNMEA->addAction("POUGT");
    menuNMEA->addAction("PONDM");
    menuNMEA->addAction("PORZA");
    menuNMEA->addAction("PNVGO");
    menuNMEA->addAction("PNVGVER");
    menuNMEA->addAction("PNVGS");
    menuNMEA->addAction("PNVGR");
    menuNMEA->addAction("RMC");

    NMEA_Actions = menuNMEA->actions();

    menuNMEA->addSeparator();
    menuNMEA->addAction("Select all");

    for(int i=0; i<NMEA_Actions.size(); i++)
        NMEA_Actions[i]->setCheckable(true);

    connect(menuNMEA, SIGNAL(triggered(QAction*)), this, SLOT(OnOFF_NMEA(QAction*)));
}

void Parse_NMEA::OnOFF_NMEA(QAction* action)
{
    if (action->text() == "Select all") {
        bool isAllSelected = true;
        for(int i=0; i<NMEA_Actions.size(); i++) {
            if (!NMEA_Actions[i]->isChecked()) {
                isAllSelected = false;
                break;
            }
        }
        if (isAllSelected) {
            for(int i=0; i<NMEA_Actions.size(); i++) {
                NMEA_Actions[i]->setChecked(false);
            }
        } else {
            for(int i=0; i<NMEA_Actions.size(); i++) {
                NMEA_Actions[i]->setChecked(true);
            }
        }
    }

    if(NMEA_Actions[0]->isChecked())
        NMEA_Data_Settings.GPGGA.time = 1;
    else
        NMEA_Data_Settings.GPGGA.time = 0;

    if(NMEA_Actions[1]->isChecked())
        NMEA_Data_Settings.POHPR.time = 1;
    else
        NMEA_Data_Settings.POHPR.time = 0;

    if(NMEA_Actions[2]->isChecked())
        NMEA_Data_Settings.POUGT.satellite = 1;
    else
        NMEA_Data_Settings.POUGT.satellite = 0;

    if(NMEA_Actions[3]->isChecked())
        NMEA_Data_Settings.PONDM.Course = 1;
    else
        NMEA_Data_Settings.PONDM.Course = 0;

    if(NMEA_Actions[4]->isChecked())
        NMEA_Data_Settings.PORZA.Number = 1;
    else
        NMEA_Data_Settings.PORZA.Number = 0;

    if(NMEA_Actions[5]->isChecked())
        NMEA_Data_Settings.PNVGO.Signal = 1;
    else
        NMEA_Data_Settings.PNVGO.Signal = 0;

    if(NMEA_Actions[6]->isChecked())
        NMEA_Data_Settings.PNVGVER.Signal = 1;
    else
        NMEA_Data_Settings.PNVGVER.Signal = 0;

    if(NMEA_Actions[7]->isChecked())
        NMEA_Data_Settings.PNVGS.Signal = 1;
    else
        NMEA_Data_Settings.PNVGS.Signal = 0;

    if(NMEA_Actions[8]->isChecked())
        NMEA_Data_Settings.PNVGR.Status = 1;
    else
        NMEA_Data_Settings.PNVGR.Status = 0;

    if(NMEA_Actions[9]->isChecked())
        NMEA_Data_Settings.RMC.Time = 1;
    else
        NMEA_Data_Settings.RMC.Time = 0;
}
