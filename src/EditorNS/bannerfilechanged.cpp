#include "EditorNS/bannerfilechanged.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

namespace EditorNS
{

BannerFileChanged::BannerFileChanged(QWidget *parent) :
    BannerBasicMessage(parent)
{
    setMessage(tr("This file has been changed outside of Notepadng."));

    QPushButton *btnReload = addButton(tr("Reload"));
    connect(btnReload, &QPushButton::clicked, this, &BannerFileChanged::reload);

    QPushButton *btnIgnore = addButton(tr("Ignore"));
    connect(btnIgnore, &QPushButton::clicked, this, &BannerFileChanged::ignore);
}

} //namespace EditorNS
