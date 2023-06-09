#include "course_heeling_trim.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QGraphicsWidget>
#include <math.h>

Course_Heeling_Trim::Course_Heeling_Trim(QWidget *parent) :
    QWidget(parent)
{
    this->setMaximumSize(215, 570);

    QVBoxLayout *QVB = new QVBoxLayout;

    course           = new Course_QW();

    roll             = new Roll_QW();

    pitch            = new Pitch_QW();

    QVB->addWidget(course);
    QVB->addWidget(roll);
    QVB->addWidget(pitch);
    setLayout(QVB);
}

void Course_Heeling_Trim::Parse_POHPR_Slot(const struct POHPR &POHPR)
{
    course->animate(POHPR);
    roll->animate(POHPR);
    pitch->animate(POHPR);
}

void Course_Heeling_Trim::hideEvent(QHideEvent *)
{
    QSize Size = this->size();
    emit Hide_Signal(Size);
}
