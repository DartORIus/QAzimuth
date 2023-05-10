#ifndef DRAWGPS_H
#define DRAWGPS_H

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>
#include <QPointer>
#include <QButtonGroup>
#include "Formulars/Koord/Coord_QW.h"

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
    void openFile(QLabel*, QMap<int, QList<double> >*);

    QGroupBox* createBenchmarkCoordGroup();
    QLineEdit* longitude_lineEdit;
    QLineEdit* latitude_lineEdit;
    QGroupBox* createDynamicCoordGroup();
    QLabel* dynamicGPSFileNameLabel;
    QLabel* staticGPSFileNameLabel;
    QGroupBox* createResultsGroup();
    QButtonGroup* format_bgroup;
    QButtonGroup* geometry_bgroup;

    void closeEvent(QCloseEvent* event) override;

    QSettings& Settings;
    void Read_Setting();

private slots:
    void slotOpenBenchmarkFile();
    void slotOpenStaticFile();
    void slotOpenDynamicFile();
    void slotCalculateGPS();
    void slotShowMap();
};

#endif // DRAWGPS_H
