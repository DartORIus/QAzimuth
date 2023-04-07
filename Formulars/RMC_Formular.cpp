#include "RMC_Formular.h"
#include <QSettings>
#include <QCoreApplication>

RMC_Formular::RMC_Formular()
{
    readSettings();
    Graphic::Add_Graphic(0, 360, "Course");

    Graphic::Add_Label("Time");
    Graphic::Add_Label("Date");
    Graphic::Add_Label("Speed");
}

RMC_Formular::~RMC_Formular()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("/Windows/RMC_Formular/geometry", saveGeometry());
}

void RMC_Formular::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    restoreGeometry(settings.value("/Windows/RMC_Formular/geometry").toByteArray());
}

void RMC_Formular::Parse_GPRMC_Signal(const struct GPRMC &RMC)
{
    Graphic::Build_Graphic(RMC.Course, RMC.Status, 0);

    Graphic::Write_Data(int(RMC.Time),       0);
    Graphic::Write_Data(int(RMC.Date),       1);
    Graphic::Write_Data(int(RMC.Speed),      2);
}
