#include "include/nngsettings.h"

#include "include/notepadng.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

NngSettings &NngSettings::getInstance()
{
    static NngSettings settings;
    return settings;
}
