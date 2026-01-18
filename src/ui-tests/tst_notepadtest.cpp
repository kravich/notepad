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
    void editorPathIsHtml();
};

NotepadTest::NotepadTest()
{
    //QApplication a();
}

void NotepadTest::editorPathIsHtml()
{
    QVERIFY(Notepad::editorPath().endsWith(".html"));
}

QTEST_GUILESS_MAIN(NotepadTest)

#include "tst_notepadtest.moc"
