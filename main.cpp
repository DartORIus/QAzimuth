
#include "drawgps.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("GPS-calculator");
    QCoreApplication::setApplicationName("GPS-calculator");
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    DrawGPS w(settings);
    w.show();
    return a.exec();
}
