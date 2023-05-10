#include "POUGT_Formular.h"
#include <QGridLayout>
#include <QFont>
#include <math.h>
#include <QByteArray>

POUGT_Formular::POUGT_Formular(QSettings& S)
    : Settings{S}
{
    readSettings();

    Graphic::Add_Graphic(-10, 10, "Line");
    Graphic::Add_Graphic(-50, 50, "Diff");

   // Graphic::Add_Label("Time");
    Graphic::Add_Time_Label();
    Graphic::Add_Label("Satellite");
}

POUGT_Formular::~POUGT_Formular()
{
    Settings.setValue("/Windows/POUGT_Formular/geometry", saveGeometry());
}

void POUGT_Formular::readSettings()
{
    const QByteArray geometry = Settings.value("/Windows/POUGT_Formular/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void POUGT_Formular::Parse_POUGT_Slot(const struct POUGT &POUGT)
{
    Graphic::Build_Graphic(POUGT.base_line, POUGT.status, 0);
    Graphic::Build_Graphic(POUGT.diff,      POUGT.status, 1);

    Graphic::Write_Time(POUGT.time);
    Graphic::Write_Data(int(POUGT.satellite),  0);
}


void POUGT_Formular::Find_Slot()
{
    Find_Text = "$POUGT, ";
}
