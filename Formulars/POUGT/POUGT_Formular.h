#ifndef POUGT_FORMULAR_H
#define POUGT_FORMULAR_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QSettings>
#include <Graphic/Graphic_Widget.h>

#include <lib/NMEA_Struct.h>
#include <Graphic/graphic.h>

class POUGT_Formular : public Graphic
{
    Q_OBJECT
public:
    explicit POUGT_Formular(QSettings&);
    ~POUGT_Formular();

private slots:
    void Parse_POUGT_Slot(const struct POUGT &);
    virtual void Find_Slot();

private:
    void readSettings();
    QString Find_Text;
    QSettings& Settings;
};

#endif // POUGT_FORMULAR_H
