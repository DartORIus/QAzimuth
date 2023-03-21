
#include "Course_QW.h"
#include <QPainter>
#include <math.h>

Course_QW::Course_QW(QWidget *parent)
    : QWidget(parent), Course(0), X_Size(200), Y_Size(200)
{
    setFixedSize(X_Size, X_Size);
}

void Course_QW::paintEvent(QPaintEvent *event)
{
    QPainter pain;
        pain.begin(this);
            pain.fillRect(event->rect(), QBrush(QColor(0, 154, 225)));
        pain.end();

    qreal Translate_X = X_Size/2, Translate_Y = X_Size/2;
    qreal X_Left = -Translate_X+10, Y_Down = -Translate_Y+10;
    qreal X_Lenght = -X_Left*2, Y_Lenght = -Y_Down*2;

    QPainter title;

    title.begin(this);

    QFont Title_Font;
    Title_Font.setPixelSize(15);
    title.setFont(Title_Font);
    title.translate(Translate_X, Translate_Y);
    title.setPen(QPen(QBrush(Qt::black), 4));
    title.drawText(QRect(45, -Translate_Y, 50, 20), Qt::AlignCenter, tr("Course") );

    title.end();

    QPainter painter;

    painter.begin(this);

        painter.translate(Translate_X, Translate_Y);

        painter.setBrush(QBrush(QColor(12, 108, 134)));

        painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 2));

        painter.drawEllipse(X_Left, Y_Down, X_Lenght, Y_Lenght);

        QFont Font;
        Font.setPixelSize(14);
        painter.setFont(Font);

        for(int i = 0; i<72; i++)
        {
            painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 1));
            painter.drawLine(0, Y_Down, 0, Y_Down+5);
            painter.rotate(5);
        }

        for(int i = 0; i<12; i++)
        {
            painter.setPen(QPen(QBrush(Qt::white), 1));
            painter.drawText(QRect(-13, Y_Down+8, 26, 12), Qt::AlignCenter,
                             QString::number(i * 30) );
            painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 2));
            painter.drawLine(0, Y_Down-5,  0, Y_Down+5);
            painter.drawLine(0, Y_Down+20, 0, Y_Down+25);
            painter.rotate(30);
        }

    painter.end();

    QPainter Strelka;
    QFont CourseFont;
    CourseFont.setPixelSize(18);

    Strelka.begin(this);

        Strelka.translate(Translate_X, Translate_Y);

        qreal Round = 20;

        QBrush brush(Qt::yellow);

        if(Status != 'B' && Status != 'A')
            brush.setColor(Qt::red);

        Strelka.setPen(QPen(brush, 1));
        Strelka.setFont(CourseFont);
        Strelka.drawText(QRect(-Round, -Round, Round*2, Round*2), Qt::AlignCenter, QString::number(Course) );

        Strelka.rotate(Course);
        Strelka.setPen(QPen(brush, 4));
        Strelka.drawLine(0,   Y_Down+25,  0, -Round);
        Strelka.drawLine(0,   Round,         0, -Y_Down-25);
        Strelka.drawLine(-10, Y_Down+40,  0, Y_Down+25);
        Strelka.drawLine(0,   Y_Down+25, 10, Y_Down+40);

        Strelka.drawEllipse(-Round, -Round, Round*2, Round*2);

    Strelka.end();

}

void Course_QW::animate(const struct POHPR &POHPR)
{
    Course = POHPR.course;
    Status = POHPR.status;
    repaint();
}
