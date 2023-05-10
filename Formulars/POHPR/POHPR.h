#ifndef POHPR_W_H
#define POHPR_W_H

#include <QWidget>
#include <QPaintEvent>
#include <QVector>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QMenu>
#include <QAction>
#include <QSettings>

#include <lib/NMEA_Struct.h>
#include <Graphic/graphic.h>

class POHPR_W : public Graphic
{
    Q_OBJECT
public:
    explicit POHPR_W(QSettings&);
    ~POHPR_W();

private slots:
    void Parse_POHPR_Slot(const struct POHPR &);

private:
    void readSettings();
    QString Find_Text;
    QSettings& Settings;
};

#endif // POHPR_W_H
