#include <QDoubleValidator>
#include <QComboBox>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <QRadioButton>
#include <QDesktopServices>

#include "drawgps.h"
#include "../Koord/Coord_QW.h"
#include "lib/NMEA_Struct.h"

DrawGPS::DrawGPS(QSettings& S, QWidget* parent)
    : QWidget{parent}, Settings{S}
{
    setWindowTitle(tr("GPS Drawing"));
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(createDynamicCoordGroup());
    vbox->addWidget(createBenchmarkCoordGroup());
    vbox->addWidget(createResultsGroup());
    setLayout(vbox);

    Read_Setting();
}

DrawGPS::~DrawGPS()
{
    Settings.setValue("/Windows/DrawGPS/geometry", saveGeometry());
}

void DrawGPS::closeEvent(QCloseEvent* event)
{
    if (coord) {
        coord->close();
    }
}

void DrawGPS::Read_Setting()
{
    const QByteArray geometry = Settings.value("/Windows/DrawGPS/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        this->resize(411, 400);
    }
}

QGroupBox* DrawGPS::createDynamicCoordGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("GPS Coordinates"));

    QLabel* dynamicGPSLabel = new QLabel("File with GPS coordinates:");
    dynamicGPSFileNameLabel = new QLabel("File is not loaded");

    QPushButton* dynamicGPSButton = new QPushButton("Load File");
    connect(dynamicGPSButton, SIGNAL(pressed()), this, SLOT(slotOpenDynamicFile()));
    dynamicGPSButton->setFixedSize(148,28);

    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout;
    hbox->addWidget(dynamicGPSLabel);
    hbox->addWidget(dynamicGPSFileNameLabel);
    vbox->addLayout(hbox);
    vbox->addWidget(dynamicGPSButton);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox* DrawGPS::createBenchmarkCoordGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("Benchmark Coordinates"));

    latitude_lineEdit = new QLineEdit();
    latitude_lineEdit->setPlaceholderText("12.3456789");
    latitude_lineEdit->setValidator( new QDoubleValidator(-90, 90, 10) );
    QLabel* latitudeLabel = new QLabel("&Latitude:");
    latitudeLabel->setBuddy(latitude_lineEdit);

    longitude_lineEdit = new QLineEdit();
    longitude_lineEdit->setPlaceholderText("12.3456789");
    longitude_lineEdit->setValidator( new QDoubleValidator(-180, 180, 10) );
    QLabel* longitudeLabel = new QLabel("L&ongitude:");
    longitudeLabel->setBuddy(longitude_lineEdit);

    QPushButton* benchmarkFileButton = new QPushButton("Calculate from file");
    connect(benchmarkFileButton, SIGNAL(pressed()), this, SLOT(slotOpenBenchmarkFile()));

    QLabel* staticGPSLabel = new QLabel("File for correcting coordinates:");
    staticGPSFileNameLabel = new QLabel("File is not loaded");
    QPushButton* staticGPSButton = new QPushButton("Load File");
    connect(staticGPSButton, SIGNAL(pressed()), this, SLOT(slotOpenStaticFile()));

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(latitudeLabel, 0, 0);
    grid->addWidget(latitude_lineEdit, 1, 0);
    grid->addWidget(longitudeLabel, 0, 1);
    grid->addWidget(longitude_lineEdit, 1, 1);
    grid->addWidget(benchmarkFileButton, 2, 0, 1, 2);
    grid->addWidget(staticGPSLabel, 0, 2);
    grid->addWidget(staticGPSFileNameLabel, 1, 2);
    grid->addWidget(staticGPSButton, 2, 2);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    groupBox->setLayout(grid);

    return groupBox;
}

QGroupBox* DrawGPS::createResultsGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("Calculations"));

//    QLabel* areaLabel = new QLabel("Area:");
//    areaLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
//    QLabel* perimeterLabel = new QLabel("Perimeter:");

//    QComboBox *areaUnitsPopup = new QComboBox();
//    areaUnitsPopup->addItem("First Item");
//    areaUnitsPopup->addItem("Second Item");
//    areaUnitsPopup->addItem("Third Item");
//    areaUnitsPopup->addItem("Fourth Item");

//    QComboBox *perimeterUnitsPopup = new QComboBox();
//    perimeterUnitsPopup->addItem("First Item");
//    perimeterUnitsPopup->addItem("Second Item");
//    perimeterUnitsPopup->addItem("Third Item");
//    perimeterUnitsPopup->addItem("Fourth Item");

    QLabel* formatLabel = new QLabel("Format:");
    format_bgroup = new QButtonGroup(this);
    QRadioButton* latFirstButton = new QRadioButton("Latitude first", this);
    QRadioButton* longFirstButton = new QRadioButton("Longitude first", this);
    format_bgroup->addButton(latFirstButton, 1);
    format_bgroup->addButton(longFirstButton, 2);
    longFirstButton->setChecked(true);

    QLabel* geometryLabel = new QLabel("Geometry type:");
    geometry_bgroup = new QButtonGroup(this);
    QRadioButton* lineButton = new QRadioButton("Line", this);
    QRadioButton* polygonButton = new QRadioButton("Polygon", this);
    geometry_bgroup->addButton(lineButton, 1);
    geometry_bgroup->addButton(polygonButton, 2);
    lineButton->setChecked(true);

    QPushButton* saveButton = new QPushButton("Save as...");
    connect(saveButton, SIGNAL(pressed()), this, SLOT(slotCalculateGPS()));
    QPushButton* mapButton = new QPushButton("Show map");
    connect(mapButton, SIGNAL(pressed()), this, SLOT(slotShowMap()));

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(formatLabel, 0, 0);
    grid->addWidget(latFirstButton, 1, 0);
    grid->addWidget(longFirstButton, 2, 0);
    grid->addWidget(geometryLabel, 0, 1);
    grid->addWidget(lineButton, 1, 1);
    grid->addWidget(polygonButton, 2, 1);
    grid->addWidget(saveButton, 3, 0);
    grid->addWidget(mapButton, 3, 1);
    groupBox->setLayout(grid);

    return groupBox;
}

void DrawGPS::slotOpenBenchmarkFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "NME (*.nme);;All Files (*)");
    if (fileName != "") {
        QFile Open_File;
        Open_File.setFileName(fileName);
        if (!Open_File.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "Warning", "Error");
        } else {
            coord = new Coord_QW(Settings, this, Qt::Window);
            coord->setAttribute(Qt::WA_DeleteOnClose);
            coord->setWindowTitle("Coord");
            coord->setVisible(true);

            QByteArray data = Open_File.readAll();
            QString Text_NMEA = QString::fromLocal8Bit(data);
            QStringList Text_NMEA_List = Text_NMEA.split("\n");

            struct NMEA_Data NMEA_D, NMEA_Data_Settings;
            memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));
            NMEA_Data_Settings.GPGGA.time = 1;
            BmUartProtoNmea ctx;
            memset(&ctx, 0, sizeof(BmUartProtoNmea));

            for(int i = 0; i < Text_NMEA_List.size(); i++) {
                if(Text_NMEA_List[i].size() > 0) {
                    QString Z = *(Text_NMEA_List[i].end() - 1);

                    if(Z != "\r") {
                        Text_NMEA_List[i] += "\r";
                    }

                    Text_NMEA = Text_NMEA_List[i] + "\n";

                    NMEA_D = NMEA_Data_Settings;
                    char data2[4096];
                    memset(data2, 0, 4096);
                    strcpy(data2, Text_NMEA.toLocal8Bit().data());
                    nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
                    if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
                        coord->Parse_GPGGA_Slot(NMEA_D.GPGGA);
                    }
                }
            }
            Open_File.close();
            m_benchmark_latitude = coord->getAverageLat();
            m_benchmark_longitude = coord->getAverageLong();
            latitude_lineEdit->setText(QString::number(m_benchmark_latitude, 'g', 9));
            longitude_lineEdit->setText(QString::number(m_benchmark_longitude, 'g', 9));
        }
    }
}

void DrawGPS::slotOpenDynamicFile()
{
    openFile(dynamicGPSFileNameLabel, &m_dynamic_data);
}

void DrawGPS::slotOpenStaticFile()
{
   openFile(staticGPSFileNameLabel, &m_static_data);
}

void DrawGPS::openFile(QLabel* plabel, QMap<int, QList<double> >* pmap)
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "NME (*.nme);;All Files (*)");
   if (fileName != "") {
        QFile Open_File;
        Open_File.setFileName(fileName);
        if (!Open_File.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "Warning", "Error");
        } else {
            plabel->setText(QFileInfo(fileName).fileName());

            QByteArray data = Open_File.readAll();
            QString Text_NMEA = QString::fromLocal8Bit(data);
            QStringList Text_NMEA_List = Text_NMEA.split("\n");

            struct NMEA_Data NMEA_D, NMEA_Data_Settings;
            memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));
            NMEA_Data_Settings.GPGGA.time = 1;
            BmUartProtoNmea ctx;
            memset(&ctx, 0, sizeof(BmUartProtoNmea));

            for(int i = 0; i < Text_NMEA_List.size(); i++) {
                if(Text_NMEA_List[i].size() > 0) {
                    QString Z = *(Text_NMEA_List[i].end() - 1);

                    if(Z != "\r") {
                        Text_NMEA_List[i] += "\r";
                    }

                    Text_NMEA = Text_NMEA_List[i] + "\n";

                    NMEA_D = NMEA_Data_Settings;
                    char data2[4096];
                    memset(data2, 0, 4096);
                    strcpy(data2, Text_NMEA.toLocal8Bit().data());
                    nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
                    if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
                        (*pmap)[static_cast<int>(NMEA_D.GPGGA.time)] = QList<double>{NMEA_D.GPGGA.latitude, NMEA_D.GPGGA.longitude};
                    }
                }
            }
            Open_File.close();
        }
   } else {
        plabel->setText("File is not loaded");
        pmap->clear();
   }
}

void DrawGPS::slotCalculateGPS()
{
    if (!m_dynamic_data.isEmpty()) {
        if (!m_static_data.isEmpty()) {
            if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
                calcCoordinates();
                saveToFile();
            } else {
                int n = QMessageBox::warning(0, "Warning", "Benchmark coodinates were not provided."
                            "\nDo you want to generate GeoJSON without adjusting the coordinates?",
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if (n == QMessageBox::Yes) {
                    saveToFile();
                }
            }
        } else if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
            int n = QMessageBox::warning(0, "Warning",
                                        "A File to refine the coordinates was not provided."
                            "\nDo you want to generate GeoJSON without adjusting the coordinates?",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (n == QMessageBox::Yes) {
                saveToFile();
            }
        } else {
            QMessageBox::information(0, "Information", "GeoJSON will be generated without "
                                                            "adjusting the coordinates");
            saveToFile();
        }
    } else {
        QMessageBox::information(0, "Information", "Load a file for generating GeoJson from it.");
    }
}

void DrawGPS::calcCoordinates()
{
    m_benchmark_latitude = latitude_lineEdit->text().toDouble();
    m_benchmark_longitude = longitude_lineEdit->text().toDouble();
    for (auto i = m_dynamic_data.begin(), end = m_dynamic_data.end(); i != end; ++i) {
        if (m_static_data.contains(i.key())) {
            i.value()[0] -= m_benchmark_latitude - m_static_data.value(i.key())[0];
            i.value()[1] -= m_benchmark_longitude - m_static_data.value(i.key())[1];
        } else {
            m_dynamic_data.remove(i.key());
        }
    }
}

void DrawGPS::saveToFile()
{
    QString date = QDate::currentDate().toString("yyyy.MM.dd");

    #ifdef linux
    QString time = QTime::currentTime().toString("hh:mm");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + "_" + time + ".geojson",
                                                    "GeoJSON File(*.json)");
    #else
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + ".geojson",
                                                    "GeoJSON File(*.geojson)");
    #endif

    if (fileName != "") {
        if (QFileInfo(fileName).suffix().isEmpty()) {
            fileName.append(".geojson");
        }
        QFile file;
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(0, "Warning", "Error");
        } else {
            QTextStream stream(&file);
            stream.setRealNumberPrecision(13);
            stream << "{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n";
            stream << "    {\n      \"type\": \"Feature\",\n      \"properties\": {},\n";
            stream << "      \"geometry\": {\n        \"type\": \"";
            switch (geometry_bgroup->checkedId()) {
                case 1:
                    stream << "LineString";
                    break;

                case 2:
                    stream << "Polygon";
                    break;
            }
            stream << "\",\n        \"coordinates\": [";
            if (geometry_bgroup->checkedId() == 2) {
                stream << "\n          [";
            }

            int index_1 = (0x01 & format_bgroup->checkedId())^0x01;
            int index_2 = 0x01 & format_bgroup->checkedId();
            bool comma = false;
            int count = 0;
            for (auto i = m_dynamic_data.cbegin(), end = m_dynamic_data.cend(); i != end; ++i) {
                if (comma) {
                    stream << ",";
                }
                if (count++ % 4 == 0) {
                    stream << "\n          ";
                }
                stream << "[" << i.value()[index_1] << "," << i.value()[index_2] << "]";
                comma = true;
            }
            if (geometry_bgroup->checkedId() == 2) {
                if (m_dynamic_data.first() != m_dynamic_data.last()) {
                    stream << ",[" << m_dynamic_data.first()[index_1] << "," << m_dynamic_data.first()[index_2] << "]";
                }
                stream << "\n          ]";
            }
            stream << "\n        ]\n      }\n    }\n  ]\n}\n";

            file.close();
        }
    }
}

void DrawGPS::slotShowMap()
{
    QString mapUrl = "http://geojson.io";
    if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
        mapUrl += "/#map=5/" + latitude_lineEdit->text() + "/" + longitude_lineEdit->text();
    }
    QDesktopServices::openUrl(QUrl(mapUrl));
}
