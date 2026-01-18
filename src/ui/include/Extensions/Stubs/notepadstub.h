#ifndef EXTENSIONS_STUBS_NOTEPAD_H
#define EXTENSIONS_STUBS_NOTEPAD_H

#include "include/Extensions/Stubs/stub.h"
#include "include/mainwindow.h"

namespace Extensions
{
    namespace Stubs
    {

        class NotepadStub : public Stub
        {
            Q_OBJECT
        public:
            NotepadStub(RuntimeSupport *rts);
            ~NotepadStub();

            NP_STUB_NAME("Notepad")

        private:
            NP_DECLARE_EXTENSION_METHOD(commandLineArguments)
            NP_DECLARE_EXTENSION_METHOD(version)
            NP_DECLARE_EXTENSION_METHOD(print)
            NP_DECLARE_EXTENSION_METHOD(windows)

            void on_newWindow(MainWindow *window);
        };

    } //namespace Stubs
} //namespace Extensions

#endif // EXTENSIONS_STUBS_NOTEPAD_H
