#include "Heeling_QW.h"

#include <QPainter>
#include <math.h>

Roll_QW::Roll_QW(QWidget *parent)
    : QWidget(parent),Roll(0), X_Size(200), Y_Size(125)
{
    setFixedSize(X_Size, Y_Size);
}

void Roll_QW::paintEvent(QPaintEvent *event)
{
    QPainter pain;
        pain.begin(this);
            pain.fillRect(event->rect(), QBrush(QColor(0, 154, 225)));
        pain.end();

    qreal Translate_X = X_Size/2, Translate_Y = X_Size/2;
    qreal X_Left = -Translate_X+10, Y_Down = -Translate_Y+10;
    qreal Round = 20;

    QPainter title;

    title.begin(this);

    QFont Title_Font;
    Title_Font.setPixelSize(15);
    title.setFont(Title_Font);
    title.translate(Translate_X, Translate_Y);
    title.setPen(QPen(QBrush(Qt::black), 4));
    title.drawText(QRect(45, -Translate_Y, 55, 20), Qt::AlignCenter, tr("Roll") );

    title.end();

    QPainter painter;

    painter.begin(this);

        painter.translate(Translate_X, Translate_Y);

        painter.setBrush(QBrush(QColor(12, 108, 134)));

        painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 2));

        painter.drawChord(X_Left, Y_Down, -X_Left*2, -Y_Down*2, 0, 180*16);

        QFont Font;
        Font.setPixelSize(14);
        painter.setFont(Font);

        painter.rotate(-90);
        for(int i = 0; i<36; i++)
        {
            painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 1));
            painter.drawLine(0, Y_Down, 0, Y_Down+5);
            painter.rotate(5);
        }

        painter.rotate(180);
        for(int i = 0; i<7; i++)
        {
            painter.setPen(QPen(QBrush(Qt::white), 1));
            painter.drawText(QRect(-13, Y_Down+8, 26, 12), Qt::AlignCenter,
                             QString::number(i * 30 - 90) );

            painter.setPen(QPen(QBrush(QColor(0, 240, 0)), 2));
            painter.drawLine(0, Y_Down-5,  0, Y_Down+5);
            painter.drawLine(0, Y_Down+20, 0, Y_Down+25);
            painter.rotate(30);
        }

    painter.end();

    QPainter Samolot;
    QFont CourseFont;
    CourseFont.setPixelSize(18);

        Samolot.begin(this);

            QBrush brush(Qt::yellow);

            if(Status != 'B' && Status != 'A')
                brush.setColor(Qt::red);

            Samolot.translate(Translate_X, Translate_Y);
            Samolot.rotate(Roll);

            Samolot.setPen(QPen(brush, 5));

            Samolot.drawLine(X_Left+25, 0, -Round, 0);
            Samolot.drawLine(0, -Round, 0, Y_Down+30);
            Samolot.drawLine(Round, 0, -X_Left-25, 0);

            Samolot.setBrush(QBrush(QColor(12, 108, 134)));
            Samolot.drawEllipse(-Round, -Round, Round*2, Round*2);

            Samolot.setFont(CourseFont);
            Samolot.drawText(QRect(-20, -20, 40, 40), Qt::AlignCenter,
                             QString::number(Roll) );
        Samolot.end();
}

void Roll_QW::animate(const struct POHPR &POHPR)
{
    Roll = POHPR.roll;
    Status  = POHPR.status;
    repaint();
}
