#ifndef WINDOWSTUB_H
#define WINDOWSTUB_H

#include "include/Extensions/Stubs/stub.h"
#include "include/mainwindow.h"

#include <QObject>

namespace Extensions {
    namespace Stubs {

        class WindowStub : public Stub
        {
            Q_OBJECT
        public:
            WindowStub(MainWindow *object, RuntimeSupport *rts);
            ~WindowStub();

            NP_STUB_NAME("Window")
        private:
            NP_DECLARE_EXTENSION_METHOD(currentEditor)
            NP_DECLARE_EXTENSION_METHOD(addExtensionMenuItem)
        };

    }
}

#endif // WINDOWSTUB_H
