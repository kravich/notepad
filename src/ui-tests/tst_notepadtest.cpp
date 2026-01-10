#include <QString>
#include <QtTest>
#include "include/notepad.h"
#include "nqqsettings.cpp"
#include "notepad.cpp"

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
