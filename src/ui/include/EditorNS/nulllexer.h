#ifndef NULL_LEXER_H
#define NULL_LEXER_H

#include "Qsci/qscilexercustom.h"

namespace EditorNS
{

class NullLexer: public QsciLexerCustom
{
public:
    //! Construct a NullLexer with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    NullLexer(QObject *parent = 0);

    //! Returns true if the language is case sensitive.  The default is true.
    const char* language() const override;

    //! Returns the descriptive name for style number \a style.  For a valid
    //! style number for this language a non-empty QString must be returned.
    //! If the style number is invalid then an empty QString must be returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const override;

    //! This is called when the section of text beginning at position \a start
    //! and up to position \a end needs to be styled.  \a start will always be
    //! at the start of a line.  The text is styled by calling startStyling()
    //! followed by one or more calls to setStyling().  It must be
    //! re-implemented by a sub-class.
    //!
    //! \sa setStyling(), startStyling(), QsciScintilla::bytes(),
    //! QsciScintilla::text()
    virtual void styleText(int start, int end) override;
};

} // namespace EditorNS

#endif
