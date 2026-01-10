#include <QString>
#include <QtTest>
#include "include/notepad.h"
#include "nqqsettings.cpp"
#include "notepad.cpp"

class NotepadqqTest : public QObject
{
    Q_OBJECT

public:
    NotepadqqTest();

private Q_SLOTS:
    void editorPathIsHtml();
};

NotepadqqTest::NotepadqqTest()
{
    //QApplication a();
}

void NotepadqqTest::editorPathIsHtml()
{
    QVERIFY(Notepadqq::editorPath().endsWith(".html"));
}

QTEST_GUILESS_MAIN(NotepadqqTest)

#include "tst_notepadtest.moc"
