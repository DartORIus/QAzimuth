#ifndef TRIM_QW_H
#define TRIM_QW_H

#include <QWidget>
#include <QPaintEvent>
#include "lib/NMEA_Struct.h"

class Pitch_QW : public QWidget
{
public:
    Pitch_QW(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
public slots:
    void animate(const struct POHPR &);
private:
    qint16 Pitch;
    char   Status;
    int X_Size; int Y_Size;
};

#endif // TRIM_QW_H
