#ifndef EDITORSTUB_H
#define EDITORSTUB_H

#include "include/EditorNS/editor.h"
#include "include/Extensions/Stubs/stub.h"

#include <QObject>

namespace Extensions
{
namespace Stubs
{

class EditorStub : public Stub
{
public:
    EditorStub(const QWeakPointer<QObject> &object, RuntimeSupport *rts);
    ~EditorStub();

    NP_STUB_NAME("Editor")

private:
    NP_DECLARE_EXTENSION_METHOD(setValue)
    NP_DECLARE_EXTENSION_METHOD(value)
    NP_DECLARE_EXTENSION_METHOD(setSelectionsText)

    EditorNS::Editor *editor();
};

} //namespace Stubs
} //namespace Extensions

#endif // EDITORSTUB_H
