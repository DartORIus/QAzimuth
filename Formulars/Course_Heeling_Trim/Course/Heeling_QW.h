#ifndef HEELING_QW_H
#define HEELING_QW_H

#include <QWidget>
#include <QPaintEvent>
#include "lib/NMEA_Struct.h"

class Roll_QW : public QWidget
{
    Q_OBJECT
public:
    Roll_QW(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
public slots:
    void animate(const struct POHPR &);
private:
    qint16 Roll;
    char   Status;
    int X_Size, Y_Size;
};

#endif // HEELING_QW_H
