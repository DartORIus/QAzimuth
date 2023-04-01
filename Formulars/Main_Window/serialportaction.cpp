#include "serialportaction.h"

SerialPortAction::SerialPortAction(const QIcon& icon, const QString& text, ACT_TYPES t, SettingsDialog* s, QObject* parent)
    :  QAction{icon, text, parent}, type{t}, port{s}
{

}

void SerialPortAction::slot1()
{
    emit wasTriggered(this);
}
