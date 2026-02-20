#include "include/notepad.h"
#include "notepad.cpp"
#include "npsettings.cpp"

#include <QString>
#include <QtTest>

class NotepadTest : public QObject
{
    Q_OBJECT

public:
    NotepadTest();

private Q_SLOTS:
};

NotepadTest::NotepadTest()
{
    //QApplication a();
}

QTEST_GUILESS_MAIN(NotepadTest)

#include "tst_notepadtest.moc"
