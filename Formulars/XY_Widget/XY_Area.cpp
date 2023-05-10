#include <QByteArray>
#include "XY_Area.h"
#include "ui_XY_Area.h"

XY_Area::XY_Area(QSettings& S, QWidget *parent) :
    QWidget(parent), ui(new Ui::XY_Area), Settings{S}
{
    ui->setupUi(this);
    readSettings();
    setWindowTitle("XY_Area");

    XV_Wid = new XY_Widget(QString("X_V"));
    XG_Wid = new XY_Widget(QString("X_G"));
    YV_Wid = new XY_Widget(QString("Y_V"));
    YG_Wid = new XY_Widget(QString("Y_G"));
    ZV_Wid = new XY_Widget(QString("Z_V"));
    ZG_Wid = new XY_Widget(QString("Z_G"));

    ui->mdiArea->addSubWindow(XV_Wid);
    ui->mdiArea->addSubWindow(XG_Wid);
    ui->mdiArea->addSubWindow(YV_Wid);
    ui->mdiArea->addSubWindow(YG_Wid);
    ui->mdiArea->addSubWindow(ZV_Wid);
    ui->mdiArea->addSubWindow(ZG_Wid);
}

XY_Area::~XY_Area()
{
    Settings.setValue("/Windows/XY_Formular/geometry", saveGeometry());
    delete ui;
}

void XY_Area::readSettings()
{
    const QByteArray geometry = Settings.value("/Windows/XY_Formular/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void XY_Area::Clear()
{
    XV_Wid->Clear();
    XG_Wid->Clear();
    YV_Wid->Clear();
    YG_Wid->Clear();
    ZV_Wid->Clear();
    ZG_Wid->Clear();
}

void XY_Area::resizeEvent(QResizeEvent *)
{
    ui->mdiArea->resize(this->width()-5, this->height()-5);
}
