#ifndef COURSE_HEELING_TRIM_H
#define COURSE_HEELING_TRIM_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "lib/NMEA_Struct.h"
#include "Course/Course_QW.h"
#include "Course/Heeling_QW.h"
#include "Course/Trim_QW.h"

class Course_Heeling_Trim : public QWidget
{
    Q_OBJECT
public:
    explicit Course_Heeling_Trim(QWidget *parent = 0);
    ~Course_Heeling_Trim();
    
signals:
    void Hide_Signal(const QSize &);
public slots:
    void Parse_POHPR_Slot(const struct POHPR &);

private:
    void readSettings();
    void hideEvent(QHideEvent *);
    Course_QW *course;
    Roll_QW *roll;
    Pitch_QW *pitch;
};

#endif // COURSE_HEELING_TRIM_H
