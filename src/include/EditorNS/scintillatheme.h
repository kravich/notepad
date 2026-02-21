#ifndef SCINTILLATHEME_H
#define SCINTILLATHEME_H

#include <QList>
#include <QMap>
#include <QString>
#include <QRgb>

namespace EditorNS
{

const QString globalStyleDefault = "Default Style";
const QString globalStyleCaretColour = "Caret colour";
const QString globalStyleCurrentLineBackgroundColour = "Current line background colour";
const QString globalStyleLineNumberMargin = "Line number margin";
const QString globalStyleFoldMargin = "Fold margin";
const QString globalStyleBraceHightlightStyle = "Brace highlight style";
const QString globalStyleBadBraceColour = "Bad brace colour";
const QString globalStyleSelectedTextColour = "Selected text colour";
const QString globalStyleIndentGuidelineStyle = "Indent guideline style";

enum FontStyle
{
    FONT_STYLE_NONE = 0,
    FONT_STYLE_BOLD = 1,
    FONT_STYLE_ITALIC = 2,
    FONT_STYLE_UNDERLINE = 4,
};

struct Style
{
    QString name;
    int styleId = 0;
    QRgb fgColor = 0;
    QRgb bgColor = 0;
    int fontStyle = FONT_STYLE_NONE;
};

struct ScintillaTheme
{
    QMap<QString, Style> defaultStyles;
    QMap<QString, QList<Style>> lexerStyles;

    int LoadFromFile(const QString &themePath);
};

QString MapIdToNppLanguageName(const QString &id);

} // namespace EditorNS

#endif
