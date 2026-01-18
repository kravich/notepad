#ifndef MENUITEMSTUB_H
#define MENUITEMSTUB_H

#include "include/Extensions/Stubs/stub.h"

#include <QAction>
#include <QObject>

namespace Extensions {
    namespace Stubs {

        class MenuItemStub : public Stub
        {
            Q_OBJECT
            void on_triggered(bool checked = false);

        public:
            MenuItemStub(QAction *object, RuntimeSupport *rts);
            ~MenuItemStub();

            NP_STUB_NAME("MenuItem")

        private:
            NP_DECLARE_EXTENSION_METHOD(setShortcut)
        };

    }
}

#endif // MENUITEMSTUB_H
