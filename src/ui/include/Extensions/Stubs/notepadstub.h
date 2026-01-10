#ifndef EXTENSIONS_STUBS_NOTEPAD_H
#define EXTENSIONS_STUBS_NOTEPAD_H

#include "include/Extensions/Stubs/stub.h"
#include "include/mainwindow.h"

namespace Extensions {
    namespace Stubs {

        class NotepadStub : public Stub
        {
            Q_OBJECT
        public:
            NotepadStub(RuntimeSupport *rts);
            ~NotepadStub();

            NQQ_STUB_NAME("Notepad")

        private:
            NQQ_DECLARE_EXTENSION_METHOD(commandLineArguments)
            NQQ_DECLARE_EXTENSION_METHOD(version)
            NQQ_DECLARE_EXTENSION_METHOD(print)
            NQQ_DECLARE_EXTENSION_METHOD(windows)

            void on_newWindow(MainWindow *window);
        };

    }
}

#endif // EXTENSIONS_STUBS_NOTEPAD_H
