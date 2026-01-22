#include "include/Extensions/Stubs/notepadstub.h"

#include "include/Extensions/Stubs/windowstub.h"
#include "include/Extensions/runtimesupport.h"
#include "include/globals.h"
#include "include/notepad.h"

#include <QApplication>

namespace Extensions
{
namespace Stubs
{

NotepadStub::NotepadStub(RuntimeSupport *rts) :
    Stub(rts)
{
    connect(&Notepad::getInstance(), &Notepad::newWindow, this, &NotepadStub::on_newWindow);
}

NotepadStub::~NotepadStub()
{
}

void NotepadStub::on_newWindow(MainWindow *window)
{
    RuntimeSupport *rts = runtimeSupport();
    QSharedPointer<Extensions::Stubs::WindowStub> windowStub =
        QSharedPointer<Extensions::Stubs::WindowStub>(
            new Extensions::Stubs::WindowStub(window, rts));

    QJsonArray args;
    args.append(rts->getJSONStub(rts->presentObject(windowStub), windowStub->stubName()));

    rts->emitEvent(this, "newWindow", args);
}

NP_DEFINE_EXTENSION_METHOD(NotepadStub, commandLineArguments, )
{
    QJsonArray arr = QJsonArray::fromStringList(QApplication::arguments());
    return StubReturnValue(arr);
}

NP_DEFINE_EXTENSION_METHOD(NotepadStub, version, )
{
    return StubReturnValue(QJsonValue(::Notepad::version));
}

NP_DEFINE_EXTENSION_METHOD(NotepadStub, print, args)
{
    QString output = "";

    for (int i = 0; i < args.count(); i++)
    {
        QJsonValue val = args.at(i);

        if (i != 0)
        {
            output += " ";
        }

        if (val.isString())
        {
            output += val.toString();
        }
        else if (val.isDouble())
        {
            output += QString::number(val.toDouble());
        }
        else if (val.isBool())
        {
            output += val.toBool() ? "true" : "false";
        }
        else if (val.isNull())
        {
            output += "null";
        }
        else if (val.isUndefined())
        {
            output += "undefined";
        }
        else if (val.isArray())
        {
            output += "[Array]";
        }
        else if (val.isObject())
        {
            output += "[Object]";
        }
    }

    println(output);
    return StubReturnValue();
}

NP_DEFINE_EXTENSION_METHOD(NotepadStub, windows, )
{
    QList<MainWindow *> windows = MainWindow::instances();
    RuntimeSupport *rts = runtimeSupport();

    QJsonArray jsonWindows;

    for (int i = 0; i < windows.length(); i++)
    {
        QSharedPointer<Extensions::Stubs::WindowStub> windowStub =
            QSharedPointer<Extensions::Stubs::WindowStub>(
                new Extensions::Stubs::WindowStub(windows[i], rts));

        QJsonObject stub = rts->getJSONStub(rts->presentObject(windowStub), windowStub->stubName());
        jsonWindows.append(stub);
    }

    return StubReturnValue(jsonWindows);
}

        // TODO Implement messagebox
} //namespace Stubs
} //namespace Extensions
