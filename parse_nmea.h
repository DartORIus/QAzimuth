#ifndef PARSE_NMEA_H
#define PARSE_NMEA_H

#include <QObject>
#include <QString>
#include <QMenu>
#include <QSettings>

#include "lib/NMEA_Struct.h"

class Parse_NMEA : public QObject
{
    Q_OBJECT
public:
    explicit Parse_NMEA(QMenu *, QObject *parent = 0);
    ~Parse_NMEA();
signals:
    void Parse_POHPR_Signal(const struct POHPR &);
    void Parse_GPGGA_Signal(const struct GPGGA &);
    void Parse_PONDM_Signal(const struct PONDM &);
    void Parse_PORZA_Signal(const struct PORZA &);
    void Parse_PNVGO_Signal(const struct PNVGO &);
    void Parse_PNVGVER_Signal(const struct PNVGVER &);
    void Parse_PNVGS_Signal(const struct PNVGS &);
    void Parse_PNVGR_Signal(const struct PNVGR &);
    void Parse_GPRMC_Signal(const struct GPRMC &);
    void Parse_POUGT_Signal(const struct POUGT &);
    void Parse_PORZV_Signal(const struct PORZV &);
    void Parse_GPZDA_Signal(const struct GPZDA &);
public slots:
    void Parse(const QString &);
    void OnOFF_NMEA(QAction*);
private:
    struct NMEA_Data NMEA_D, NMEA_Data_Settings;
    BmUartProtoNmea ctx;
    QSettings Settings;
    void Read_Settings();
    QMenu *menuNMEA;
    QList<QAction*> NMEA_Actions;
    void NMEA_Select();
};

#endif // PARSE_NMEA_H
