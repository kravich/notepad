#ifndef _NNGRUN_H_
#define _NNGRUN_H_

#include "include/npsettings.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QStyledItemDelegate>
#include <QTableWidget>

namespace NngRun
{
class RunPreferences : public QDialog
{
    Q_OBJECT

private:
    NngSettings &m_settings;
    QTableWidget *m_commands;

public:
    RunPreferences(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~RunPreferences();

private slots:
    void slotRemove();
    void slotOk();
    void slotInitCell(int row, int column);
};

class RunDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RunDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QIcon openIcon;
    QIcon rmIcon;

signals:
    void needsRemoval();
};

class RunDialog : public QDialog
{
    Q_OBJECT
private:
    NngSettings &m_settings;
    QLabel *m_status;
    QLineEdit *m_command;
    bool m_saved;

public:
    RunDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~RunDialog();
    QString getCommandInput();
    bool saved();
    static QStringList parseCommandString(QString cmd);

private slots:
    void slotSave();
    void slotHideStatus();
};

}; //namespace NngRun
#endif
