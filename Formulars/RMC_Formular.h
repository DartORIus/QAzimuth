#ifndef RMC_FORMULAR_H
#define RMC_FORMULAR_H

#include <QSettings>
#include <Graphic/graphic.h>

class RMC_Formular : public Graphic
{
    Q_OBJECT
public:
    RMC_Formular(QSettings&);
    ~RMC_Formular();

public slots:
    void Parse_GPRMC_Signal(const struct GPRMC &);

private:
    void readSettings();
    QSettings& Settings;
};

#endif // RMC_FORMULAR_H
