#include "include/EditorNS/nulllexer.h"

namespace EditorNS
{

NullLexer::NullLexer(QObject *parent):
    QsciLexerCustom(parent)
{
}

const char *NullLexer::language() const
{
    return "null";
}

QString NullLexer::description(int style) const
{
    if (style == 0)
    {
        return "Null style";
    }

    return "";
}

void NullLexer::styleText(int start, int end)
{
}

} // namespace EditorNS
