#include "include/notepadng.h"
#include "notepadng.cpp"
#include "npsettings.cpp"

#include <QString>
#include <QtTest>

class NotepadngTest : public QObject
{
    Q_OBJECT

public:
    NotepadngTest();

private Q_SLOTS:
};

NotepadngTest::NotepadngTest()
{
    //QApplication a();
}

QTEST_GUILESS_MAIN(NotepadngTest)

#include "tst_notepadngtest.moc"
