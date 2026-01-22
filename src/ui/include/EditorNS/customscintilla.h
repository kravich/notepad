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

signals:
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
