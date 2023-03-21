
#ifndef MOUSE_H
#define MOUSE_H

#include <QWidget>
#include <QPaintEvent>
#include "lib/NMEA_Struct.h"

class Course_QW : public QWidget
{
    Q_OBJECT
public:
    Course_QW(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
public slots:
    void animate(const struct POHPR &);
private:
    qint16 Course;
    char   Status;
    int X_Size;
    int Y_Size;
};

#endif
