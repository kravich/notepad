#ifndef CUSTOMSCINTILLA_H
#define CUSTOMSCINTILLA_H

#include <Qsci/qsciscintilla.h>

#include <QDropEvent>
#include <QFocusEvent>
#include <QContextMenuEvent>

namespace EditorNS
{

class CustomScintilla : public QsciScintilla
{
    Q_OBJECT
public:
    explicit CustomScintilla(QWidget *parent = nullptr);

    /*
     * Wrappers for messages missing in QScintilla
     */

    /* Information */
    int positionFromPoint(const QPoint &pos);
    int pointXFromPosition(int position);
    int pointYFromPosition(int position);

    /* Scrolling and automatic scrolling */
    int getXOffset();
    void setXOffset(int xOffset);

    /* Keyboard commands */
    void lineDelete();
    void lineDuplicate();
    void moveSelectedLinesUp();
    void moveSelectedLinesDown();

    /* Zooming */
    int getZoom();

signals:
    void zoomChanged(int zoomFactor);

    void urlsDropped(QList<QUrl> urls);
    void gotFocus();

protected:
    void keyPressEvent(QKeyEvent *ev) override;
    void dropEvent(QDropEvent *ev) override;
    void focusInEvent(QFocusEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *ev) override;
};

} // namespace EditorNS

#endif // CUSTOMSCINTILLA_H
