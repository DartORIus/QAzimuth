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

    if(NMEA_D.GPGGA.longitude && NMEA_D.GPGGA.latitude)
        emit Parse_GPGGA_Signal(NMEA_D.GPGGA);

    if(NMEA_D.POHPR.time && NMEA_D.POHPR.status)
        emit Parse_POHPR_Signal(NMEA_D.POHPR);

    if(NMEA_D.RMC.Time && NMEA_D.RMC.Status)
        emit Parse_GPRMC_Signal(NMEA_D.RMC);
}

Parse_NMEA::~Parse_NMEA()
{
    Settings.beginGroup("/NMEA_sentence");
        Settings.setValue("/GPGGA",   NMEA_Data_Settings.GPGGA.time);
        Settings.setValue("/POHPR",   NMEA_Data_Settings.POHPR.time);
        Settings.setValue("/RMC",     NMEA_Data_Settings.RMC.Time);
    Settings.endGroup();
}

void Parse_NMEA::Read_Settings()
{
    Settings.beginGroup("/NMEA_sentence");
        NMEA_Data_Settings.GPGGA.time       = Settings.value("/GPGGA").toBool();
        NMEA_Data_Settings.POHPR.time       = Settings.value("/POHPR").toBool();
        NMEA_Data_Settings.RMC.Time         = Settings.value("/GPGGA").toBool();
    Settings.endGroup();

    Settings.beginGroup("/NMEA_sentence");
        NMEA_Actions[0] ->setChecked(Settings.value("/GPGGA").toBool());
        NMEA_Actions[1] ->setChecked(Settings.value("/POHPR").toBool());
        NMEA_Actions[2] ->setChecked(Settings.value("/RMC").toBool());
    Settings.endGroup();
}

void Parse_NMEA::NMEA_Select()
{
    menuNMEA->addAction("GPGGA");
    menuNMEA->addAction("POHPR");
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
        NMEA_Data_Settings.RMC.Time = 1;
    else
        NMEA_Data_Settings.RMC.Time = 0;
}
