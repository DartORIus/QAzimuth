#include <QByteArray>
#include "RMC_Formular.h"

RMC_Formular::RMC_Formular(QSettings& S)
    : Settings{S}
{
    readSettings();
    Graphic::Add_Graphic(0, 360, "Course");

    Graphic::Add_Label("Time");
    Graphic::Add_Label("Date");
    Graphic::Add_Label("Speed");
}

RMC_Formular::~RMC_Formular()
{
    Settings.setValue("/Windows/RMC_Formular/geometry", saveGeometry());
}

void RMC_Formular::readSettings()
{
    const QByteArray geometry = Settings.value("/Windows/RMC_Formular/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void RMC_Formular::Parse_GPRMC_Signal(const struct GPRMC &RMC)
{
    Graphic::Build_Graphic(RMC.Course, RMC.Status, 0);

    Graphic::Write_Data(int(RMC.Time),       0);
    Graphic::Write_Data(int(RMC.Date),       1);
    Graphic::Write_Data(int(RMC.Speed),      2);
}
