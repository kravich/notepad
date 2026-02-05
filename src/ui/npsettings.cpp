#include "include/npsettings.h"

#include "include/notepad.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

NpSettings &NpSettings::getInstance()
{
    static NpSettings settings;
    return settings;
}
