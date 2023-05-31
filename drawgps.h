#ifndef DRAWGPS_H
#define DRAWGPS_H

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>
#include <QPointer>
#include <QButtonGroup>
#include "Koord/Coord_QW.h"

class DrawGPS : public QWidget
{
    Q_OBJECT
public:
    explicit DrawGPS(QSettings&, QWidget* parent = nullptr);
    //    explicit DrawGPS(QSettings&, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DrawGPS();

private:
    double m_benchmark_longitude;
    double m_benchmark_latitude;
    QMap<int, QList<double> > m_dynamic_data;
    QMap<int, QList<double> > m_static_data;

    QPointer<Coord_QW> coord;
    void calcCoordinates();
    void saveToFile();
    void createData(const QString&, QMap<int, QList<double> >*);

    QGroupBox* createDynamicCoordGroup();
    QLabel* dynamicGPSFileNameLabel;
    QString dynamicFile;
    QButtonGroup* type_bgroup;
    QGroupBox* createBenchmarkCoordGroup();
    QLineEdit* longitude_lineEdit;
    QLineEdit* latitude_lineEdit;
    QLabel* staticGPSFileNameLabel;
    QString staticFile;
    QGroupBox* createResultsGroup();
    QButtonGroup* format_bgroup;
    QButtonGroup* geometry_bgroup;

    void closeEvent(QCloseEvent* event) override;

    QSettings& Settings;
    void readSetting();

//    QString genGGA(const QByteArray& time, const double& lat, const double& lng);

private slots:
    void slotOpenBenchmarkFile();
    void slotCalculateGPS();
    void slotShowMap();
//    void nmeaFromCSV();
};

#endif // DRAWGPS_H
