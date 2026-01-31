#include "include/EditorNS/customscintilla.h"

#include <QMimeData>
#include <QMenu>

namespace EditorNS
{

CustomScintilla::CustomScintilla(QWidget *parent) :
    QsciScintilla(parent)
{
    // Connect handlers for notifications
    connect(this, &CustomScintilla::SCN_ZOOM, this, [this]() { emit zoomChanged(getZoom()); });

    // Always use some monospace font by default (if not overriden by lexer)
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    setFont(font);

    // Always do brace matching
    setBraceMatching(SloppyBraceMatch);
}

/* Information */

int CustomScintilla::positionFromPoint(const QPoint &pos)
{
    return SendScintilla(SCI_POSITIONFROMPOINT, pos.x(), pos.y());
}

int CustomScintilla::pointXFromPosition(int position)
{
    return SendScintilla(SCI_POINTXFROMPOSITION, 0, position);
}

/* Scrolling and automatic scrolling */

int CustomScintilla::getXOffset()
{
    return SendScintilla(SCI_GETXOFFSET);
}

void CustomScintilla::setXOffset(int xOffset)
{
    SendScintilla(SCI_SETXOFFSET, xOffset);
}

int CustomScintilla::pointYFromPosition(int position)
{
    return SendScintilla(SCI_POINTYFROMPOSITION, 0, position);
}

/* Keyboard commands */

void CustomScintilla::lineDelete()
{
    SendScintilla(SCI_LINEDELETE);
}

void CustomScintilla::lineDuplicate()
{
    SendScintilla(SCI_LINEDUPLICATE);
}

void CustomScintilla::moveSelectedLinesUp()
{
    SendScintilla(SCI_MOVESELECTEDLINESUP);
}

void CustomScintilla::moveSelectedLinesDown()
{
    SendScintilla(SCI_MOVESELECTEDLINESDOWN);
}

/* Zooming */

int CustomScintilla::getZoom()
{
    return SendScintilla(SCI_GETZOOM);
}

void CustomScintilla::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key())
    {
    case Qt::Key_Insert:
        ev->ignore();
        break;
    default:
        QsciScintilla::keyPressEvent(ev);
    }
}

void CustomScintilla::dropEvent(QDropEvent *ev)
{
    if (ev->mimeData()->hasUrls())
    {
        ev->ignore();
        emit urlsDropped(ev->mimeData()->urls());
    }
    else
    {
        QsciScintilla::dropEvent(ev);
    }
}

void CustomScintilla::focusInEvent(QFocusEvent *event)
{
    QsciScintilla::focusInEvent(event);
    emit gotFocus();
}

void CustomScintilla::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);

    QList<QAction*> contextActions = actions();

    if (contextActions.size() > 0)
    {
        for (QAction *action : contextActions)
        {
            menu->insertAction(nullptr, action);
        }
    }
    else
    {
        QAction *placeholderAction = new QAction(this);
        placeholderAction->setText("Placeholder");
        placeholderAction->setEnabled(false);
        menu->insertAction(nullptr, placeholderAction);
    }

    menu->popup(event->globalPos());
}

} // namespace EditorNS
