#ifndef LEXERFACTORY_H
#define LEXERFACTORY_H

#include <Qsci/qscilexer.h>

namespace EditorNS
{

namespace LexerFactory
{

QsciLexer *CreateLexerForId(QObject *parent, const QString &id);

} // namespace LexerFactory

} // namespace EditorNS

#endif
