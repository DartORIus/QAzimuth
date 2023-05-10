#ifndef XY_AREA_H
#define XY_AREA_H

#include <QWidget>
#include <QSettings>
#include "XY_Widget.h"

namespace Ui {
class XY_Area;
}

class XY_Area : public QWidget
{
    Q_OBJECT

public:
    explicit XY_Area(QSettings&, QWidget *parent = 0);
    ~XY_Area();

private:
    void readSettings();
    Ui::XY_Area *ui;
    XY_Widget *XV_Wid, *XG_Wid, *YV_Wid, *YG_Wid, *ZV_Wid, *ZG_Wid;
    QSettings& Settings;

public slots:
    void Clear();

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // XY_AREA_H
