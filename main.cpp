
#include <QApplication>

#include "Formulars/Main_Window/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Navis-Ukraine/QAzimuth");
    QCoreApplication::setApplicationName("QAzimuth");
    MainWindow w;

    w.show();

    return a.exec();
}
