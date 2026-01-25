#include "include/EditorNS/editor.h"

#include "include/notepad.h"
#include "include/npsettings.h"

#include "include/Search/searchstring.h"

#include <QDir>
#include <QEventLoop>
#include <QMessageBox>
#include <QRegExp>
#include <QRegularExpression>
#include <QTimer>
#include <QVBoxLayout>

namespace EditorNS
{

const int lineNumbersMarginIdx = 0;
const int foldingMarginIdx = 1;

const int foldingMarginWidthPx = 16;

QQueue<QSharedPointer<Editor>> Editor::m_editorBuffer = QQueue<QSharedPointer<Editor>>();

Editor::Editor(QWidget *parent) :
    QWidget(parent)
{
    QString themeName = NpSettings::getInstance().Appearance.getColorScheme();

    fullConstructor(themeFromName(themeName));
}

Editor::Editor(const Theme &theme, QWidget *parent) :
    QWidget(parent)
{
    fullConstructor(theme);
}

void Editor::fullConstructor(const Theme &theme)
{
    m_jsToCppProxy = new JsToCppProxy(this);
    connect(m_jsToCppProxy,
            &JsToCppProxy::messageReceived,
            this,
            &Editor::on_proxyMessageReceived);

    m_scintilla = new CustomScintilla(this);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_scintilla, 1);
    setLayout(m_layout);

    connect(m_scintilla, &CustomScintilla::zoomChanged, this, &Editor::zoomChanged);
    connect(m_scintilla, &CustomScintilla::zoomChanged, this, &Editor::refreshMargins);

    connect(m_scintilla, &CustomScintilla::cursorPositionChanged, this, &Editor::requestDocumentInfo);
    connect(m_scintilla, &CustomScintilla::selectionChanged, this, &Editor::requestDocumentInfo);
    connect(m_scintilla, &CustomScintilla::textChanged, this, &Editor::requestDocumentInfo);

    connect(this, &Editor::documentInfoRequested, this, &Editor::cursorActivity);   // To trigger cursorActivity() also

    connect(m_scintilla, &CustomScintilla::textChanged, this, &Editor::contentChanged);
    connect(m_scintilla, &CustomScintilla::modificationChanged, this, [&]() { emit cleanChanged(isClean()); });
    connect(m_scintilla, &CustomScintilla::linesChanged, this, &Editor::refreshMargins);

    connect(m_scintilla, &CustomScintilla::urlsDropped, this, &Editor::urlsDropped);
    connect(m_scintilla, &CustomScintilla::gotFocus, this, &Editor::gotFocus);
    setLanguage(nullptr);
    setTheme(theme);
}

QSharedPointer<Editor> Editor::getNewEditor(QWidget *parent)
{
    QSharedPointer<Editor> out;

    if (m_editorBuffer.length() == 0)
    {
        addEditorToBuffer(1);
        out = QSharedPointer<Editor>::create();
    }
    else if (m_editorBuffer.length() == 1)
    {
        addEditorToBuffer(1);
        out = m_editorBuffer.dequeue();
    }
    else
    {
        out = m_editorBuffer.dequeue();
    }

    out->setParent(parent);
    return out;
}

void Editor::addEditorToBuffer(const int howMany)
{
    for (int i = 0; i < howMany; i++)
        m_editorBuffer.enqueue(QSharedPointer<Editor>::create());
}

void Editor::insertContextMenuAction(QAction *before, QAction *action)
{
    m_scintilla->insertAction(before, action);
}

void Editor::invalidateEditorBuffer()
{
    m_editorBuffer.clear();
}

void Editor::on_proxyMessageReceived(QString msg, QVariant data)
{
    QTimer::singleShot(0, [msg, data, this] {
        emit messageReceived(msg, data);

        if (msg.startsWith("[ASYNC_REPLY]"))
        {
            QRegExp rgx("\\[ID=(\\d+)\\]$");

            if (rgx.indexIn(msg) == -1)
                return;

            if (rgx.captureCount() != 1)
                return;

            unsigned int id = rgx.capturedTexts()[1].toInt();

                // Look into the list of callbacks
            for (auto it = this->asyncReplies.begin(); it != this->asyncReplies.end(); ++it)
            {
                if (it->id == id)
                {
                    AsyncReply r = *it;
                    if (r.value)
                    {
                        r.value->set_value(data);
                    }
                    this->asyncReplies.erase(it);

                    if (r.callback != nullptr)
                    {
                        QTimer::singleShot(0, [r, data] { r.callback(data); });
                    }

                    emit asyncReplyReceived(r.id, r.message, data);

                    break;
                }
            }
        }
    });
}

void Editor::setFocus()
{
    m_scintilla->setFocus();
}

void Editor::clearFocus()
{
    m_scintilla->clearFocus();
}

    /**
 * Automatically converts local relative file names to absolute ones.
 */
void Editor::setFilePath(const QUrl &filename)
{
    QUrl old = m_filePath;
    QUrl newUrl = filename;

    if (newUrl.isLocalFile())
        newUrl = QUrl::fromLocalFile(QFileInfo(filename.toLocalFile()).absoluteFilePath());

    m_filePath = newUrl;
    emit fileNameChanged(old, newUrl);
}

    /**
 * Always returns an absolute url.
 */
QUrl Editor::filePath() const
{
    return m_filePath;
}

QString Editor::tabName() const
{
    return m_tabName;
}

void Editor::setTabName(const QString &name)
{
    m_tabName = name;
}

void Editor::refreshMargins()
{
    m_scintilla->setMarginLineNumbers(lineNumbersMarginIdx, m_lineNumbersVisible);
    m_scintilla->setFolding(m_lineNumbersVisible ? CustomScintilla::BoxedTreeFoldStyle : CustomScintilla::NoFoldStyle, foldingMarginIdx); // FIXME: Make folding a setting separate from line numbers

    if (m_lineNumbersVisible)
    {
        QString lineNumbersMarginWidthString = QString(" ") + QString::number(m_scintilla->lines());

        m_scintilla->setMarginWidth(lineNumbersMarginIdx, lineNumbersMarginWidthString);
        m_scintilla->setMarginWidth(foldingMarginIdx, foldingMarginWidthPx);
    }
    else
    {
        m_scintilla->setMarginWidth(lineNumbersMarginIdx, 0);
        m_scintilla->setMarginWidth(foldingMarginIdx, 0);
    }
}

bool Editor::isClean()
{
    if (m_forceModified)
    {
        return false;
    }

    return !m_scintilla->isModified();
}

void Editor::markClean()
{
    m_forceModified = false;
    m_scintilla->setModified(false);
}

void Editor::markDirty()
{
    m_forceModified = true;
    emit cleanChanged(isClean());
}

int Editor::getHistoryGeneration()
{
    return asyncSendMessageWithResult("C_FUN_GET_HISTORY_GENERATION").get().toInt();
}

void Editor::setLanguage(const Language *lang)
{
    if (lang == nullptr)
    {
        lang = LanguageService::getInstance().lookupById("plaintext");
    }

    m_currentLanguage = lang;
    setIndentationMode(lang);   // Refresh indentation settings

    asyncSendMessageWithResult("C_CMD_SET_LANGUAGE", lang->mime.isEmpty() ? lang->mode : lang->mime);

    emit currentLanguageChanged(m_currentLanguage->id, m_currentLanguage->name);
}

void Editor::setLanguage(const QString &language)
{
    auto &cache = LanguageService::getInstance();
    auto lang = cache.lookupById(language);
    if (lang != nullptr)
    {
        setLanguage(lang);
    }
}

void Editor::setLanguageFromFilePath(const QString &filePath)
{
    auto name = QFileInfo(filePath).fileName();

    auto &cache = LanguageService::getInstance();
    auto lang = cache.lookupByFileName(name);
    if (lang != nullptr)
    {
        setLanguage(lang);
        return;
    }
    lang = cache.lookupByExtension(name);
    if (lang != nullptr)
    {
        setLanguage(lang);
    }
}

void Editor::setLanguageFromFilePath()
{
    setLanguageFromFilePath(filePath().toString());
}

void Editor::setIndentationMode(const Language *lang)
{
    const auto &s = NpSettings::getInstance().Languages;
    const bool useDefaults = s.getUseDefaultSettings(lang->id);
    const auto &langId = useDefaults ? "default" : lang->id;

    setIndentationMode(!s.getIndentWithSpaces(langId), s.getTabSize(langId));
}

bool Editor::searchAndSelect(bool inSelection,
                             const QString &string,
                             SearchHelpers::SearchMode searchMode,
                             bool forward,
                             const SearchHelpers::SearchOptions &searchOptions)
{
    QString expr = string;

    bool isRegex = (searchMode == SearchHelpers::SearchMode::Regex);
    bool isCaseSensitive = searchOptions.MatchCase;
    bool isWholeWord = searchOptions.MatchWholeWord;
    bool wrap = true;

    if (searchMode == SearchHelpers::SearchMode::SpecialChars)
    {
        // Implement searching for special characters through regex
        isRegex = true;
        expr = SearchString::format(expr, searchMode, searchOptions);   // FIXME: Ensure correctness of formatting
    }

    bool isFoundAndSelected = false;

    if (inSelection)
    {
        isFoundAndSelected = m_scintilla->findFirstInSelection(expr, isRegex, isCaseSensitive, isWholeWord, wrap, forward);
    }
    else
    {
        isFoundAndSelected = m_scintilla->findFirst(expr, isRegex, isCaseSensitive, isWholeWord, wrap, forward);
    }

    return isFoundAndSelected;
}

void Editor::setIndentationMode(const bool useTabs, const int size)
{
    m_scintilla->setIndentationsUseTabs(useTabs);
    m_scintilla->setTabWidth(size);
}

Editor::IndentationMode Editor::indentationMode()
{
    IndentationMode out;
    out.useTabs = m_scintilla->indentationsUseTabs();
    out.size = m_scintilla->tabWidth();
    return out;
}

void Editor::setCustomIndentationMode(const bool useTabs, const int size)
{
    m_customIndentationMode = true;
    setIndentationMode(useTabs, size);
}

void Editor::setCustomIndentationMode(const bool useTabs)
{
    m_customIndentationMode = true;
    setIndentationMode(useTabs, 0);
}

void Editor::clearCustomIndentationMode()
{
    m_customIndentationMode = false;
    setIndentationMode(getLanguage());
}

bool Editor::isUsingCustomIndentationMode() const
{
    return m_customIndentationMode;
}

void Editor::setSmartIndent(bool enabled)
{
    m_scintilla->setAutoIndent(enabled);
}

void Editor::setValue(const QString &value)
{
    auto lang = LanguageService::getInstance().lookupByContent(value);
    if (lang != nullptr)
    {
        setLanguage(lang);
    }
    m_scintilla->setText(value);
}

QString Editor::value()
{
    return m_scintilla->text();
}

bool Editor::fileOnDiskChanged() const
{
    return m_fileOnDiskChanged;
}

void Editor::setFileOnDiskChanged(bool fileOnDiskChanged)
{
    m_fileOnDiskChanged = fileOnDiskChanged;
}

void Editor::sendMessage(const QString msg, const QVariant data)
{
#ifdef QT_DEBUG
    qDebug() << "Legacy message " << msg << " sent.";
#endif

    emit m_jsToCppProxy->messageReceivedByJs(msg, data);
}

void Editor::sendMessage(const QString msg)
{
    sendMessage(msg, 0);
}

unsigned int messageIdentifier = 0;

std::shared_future<QVariant> Editor::asyncSendMessageWithResult(const QString msg, const QVariant data, std::function<void(QVariant)> callback)
{
    unsigned int currentMsgIdentifier = ++messageIdentifier;

    std::shared_ptr<std::promise<QVariant>> resultPromise = std::make_shared<std::promise<QVariant>>();

    AsyncReply asyncmsg;
    asyncmsg.id = currentMsgIdentifier;
    asyncmsg.message = msg;
    asyncmsg.value = resultPromise;
    asyncmsg.callback = callback;
    this->asyncReplies.push_back((asyncmsg));

    QString message_id = "[ASYNC_REQUEST]" + msg + "[ID=" + QString::number(currentMsgIdentifier) + "]";

    this->sendMessage(message_id, data);

    std::shared_future<QVariant> fut = resultPromise->get_future().share();

    while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    }

    return fut;
}

std::shared_future<QVariant> Editor::asyncSendMessageWithResult(const QString msg, std::function<void(QVariant)> callback)
{
    return this->asyncSendMessageWithResult(msg, 0, callback);
}

void Editor::setZoomFactor(int factor)
{
    m_scintilla->zoomTo(factor);
}

int Editor::zoomFactor() const
{
    return m_scintilla->getZoom();
}

void Editor::setSelectionsText(const QStringList &texts, SelectMode mode)
{
    QVariantMap data{{"text", texts}};
    switch (mode)
    {
    case SelectMode::After:
        data.insert("select", "after");
        break;
    case SelectMode::Before:
        data.insert("select", "before");
        break;
    default:
        data.insert("select", "selected");
        break;
    }
    sendMessage("C_CMD_SET_SELECTIONS_TEXT", data);
}

void Editor::setSelectionsText(const QStringList &texts)
{
    setSelectionsText(texts, SelectMode::After);
}

void Editor::insertBanner(QWidget *banner)
{
    m_layout->insertWidget(0, banner);
}

void Editor::removeBanner(QWidget *banner)
{
    if (banner != m_scintilla && m_layout->indexOf(banner) >= 0)
    {
        m_layout->removeWidget(banner);
        emit bannerRemoved(banner);
    }
}

void Editor::removeBanner(QString objectName)
{
    for (auto &&banner : findChildren<QWidget *>(objectName))
    {
        removeBanner(banner);
    }
}

void Editor::setLineWrap(const bool wrap)
{
    m_scintilla->setWrapMode(wrap ? CustomScintilla::WrapWord : CustomScintilla::WrapNone);
}

void Editor::setEOLVisible(const bool showeol)
{
    m_scintilla->setEolVisibility(showeol);
}

void Editor::setWhitespaceVisible(const bool showspace)
{
    m_scintilla->setWhitespaceVisibility(showspace ? CustomScintilla::WsVisible : CustomScintilla::WsInvisible);
}

void Editor::setIndentGuideVisible(bool showindentguide)
{
    m_scintilla->setIndentationGuides(showindentguide);
}

void Editor::requestDocumentInfo()
{
    // Cursor
    int cursorLine = 0;
    int cursorIndex = 0;

    m_scintilla->getCursorPosition(&cursorLine, &cursorIndex);

    // Selections
    int selectedLines = 0;
    int selectedChars = 0;

    if (m_scintilla->hasSelectedText())
    {
        int selectedLineFrom = 0;
        int selectedIndexFrom = 0;
        int selectedLineTo = 0;
        int selectedIndexTo = 0;

        m_scintilla->getSelection(&selectedLineFrom, &selectedIndexFrom, &selectedLineTo, &selectedIndexTo);

        selectedLines = selectedLineTo - selectedLineFrom + 1;
        selectedChars = m_scintilla->selectedText().length();
    }

    // Content
    int linesCount = m_scintilla->lines();
    int charsCount = m_scintilla->text().length();

    QMap<QString, QVariant> data;

    QList<int> cursorList;
    cursorList.append(cursorLine);
    cursorList.append(cursorIndex);

    QList<int> selectionsList;
    selectionsList.append(selectedLines);
    selectionsList.append(selectedChars);

    QList<int> contentList;
    contentList.append(linesCount);
    contentList.append(charsCount);

    data["cursor"] = QVariant::fromValue(cursorList);
    data["selections"] = QVariant::fromValue(selectionsList);
    data["content"] = QVariant::fromValue(contentList);

    emit documentInfoRequested(data);
}

QPair<int, int> Editor::cursorPosition()
{
    int cursorLine = 0;
    int cursorIndex = 0;

    m_scintilla->getCursorPosition(&cursorLine, &cursorIndex);

    return {cursorLine, cursorIndex};
}

void Editor::setCursorPosition(const int line, const int column)
{
    m_scintilla->setCursorPosition(line, column);
}

void Editor::setCursorPosition(const QPair<int, int> &position)
{
    setCursorPosition(position.first, position.second);
}

void Editor::setCursorPosition(const Cursor &cursor)
{
    setCursorPosition(cursor.line, cursor.column);
}

void Editor::setSelection(int fromLine, int fromCol, int toLine, int toCol)
{
    QVariantList arg{fromLine, fromCol, toLine, toCol};
    asyncSendMessageWithResult("C_CMD_SET_SELECTION", QVariant(arg));
}

QPair<int, int> Editor::scrollPosition()
{
    int line = m_scintilla->firstVisibleLine();
    return {0, line};   // FIXME: Support horizontal scrolling
}

void Editor::setScrollPosition(const int left, const int top)
{
    m_scintilla->setXOffset(0); // FIXME: Support horizontal scrolling
    m_scintilla->setFirstVisibleLine(top);
}

void Editor::setScrollPosition(const QPair<int, int> &position)
{
    setScrollPosition(position.first, position.second);
}

QString Editor::endOfLineSequence() const
{
    return m_endOfLineSequence;
}

void Editor::setEndOfLineSequence(const QString &newLineSequence)
{
    m_endOfLineSequence = newLineSequence;
}

void Editor::setFont(QString fontFamily, int fontSize, double lineHeight)
{
    QMap<QString, QVariant> tmap;
    tmap.insert("family", fontFamily == nullptr ? "" : fontFamily);
    tmap.insert("size", QString::number(fontSize));
    tmap.insert("lineHeight", QString::number(lineHeight, 'f', 2));
    asyncSendMessageWithResult("C_CMD_SET_FONT", tmap);
}

void Editor::setLineNumbersVisible(bool visible)
{
    m_lineNumbersVisible = visible;

    refreshMargins();
}

QTextCodec *Editor::codec() const
{
    return m_codec;
}

void Editor::setCodec(QTextCodec *codec)
{
    m_codec = codec;
}

bool Editor::bom() const
{
    return m_bom;
}

void Editor::setBom(bool bom)
{
    m_bom = bom;
}

Editor::Theme Editor::themeFromName(QString name)
{
    if (name == "default" || name.isEmpty())
        return Theme();

    QFileInfo editorPath(Notepad::editorPath());
    QDir bundledThemesDir(editorPath.absolutePath() + "/libs/codemirror/theme/");

    if (bundledThemesDir.exists(name + ".css"))
        return Theme(name, bundledThemesDir.filePath(name + ".css"));

    return Theme();
}

QList<Editor::Theme> Editor::themes()
{
    auto editorPath = QFileInfo(Notepad::editorPath());
    QDir bundledThemesDir(editorPath.absolutePath() + "/libs/codemirror/theme/", "*.css");

    QList<Theme> out;
    for (auto &&theme : bundledThemesDir.entryInfoList())
    {
        out.append(Theme(theme.completeBaseName(), theme.filePath()));
    }
    return out;
}

void Editor::setTheme(Theme theme)
{
    sendMessage("C_CMD_SET_THEME", QVariantMap{{"name", theme.name}, {"path", theme.path}});
}

QList<Editor::Selection> Editor::selections()
{
    QList<Selection> out;

    if (m_scintilla->hasSelectedText())
    {
        // FIMXE: Support several selections

        int selectedLineFrom = 0;
        int selectedIndexFrom = 0;
        int selectedLineTo = 0;
        int selectedIndexTo = 0;

        m_scintilla->getSelection(&selectedLineFrom, &selectedIndexFrom, &selectedLineTo, &selectedIndexTo);

        Editor::Selection selection;
        selection.from.line = selectedLineFrom;
        selection.from.column = selectedIndexFrom;
        selection.to.line = selectedLineTo;
        selection.to.column = selectedIndexTo;

        out.append(selection);
    }

    return out;
}

QStringList Editor::selectedTexts()
{
    QString selectedText = m_scintilla->selectedText();

    QStringList selectedTextsList;

    if (selectedText.length() > 0)
    {
        // FIMXE: Support several selections
        selectedTextsList.append(selectedText);
    }

    return selectedTextsList;
}

void Editor::setOverwrite(bool overwrite)
{
    m_scintilla->setOverwriteMode(overwrite);
}

void Editor::print(std::shared_ptr<QPrinter> printer)
{
    fprintf(stderr, "FIXME: Implement Editor::print()\n");
}

QByteArray Editor::printToPdf(const QPageLayout &pageLayout)
{
    fprintf(stderr, "FIXME: Implement Editor::printToPdf()\n");
    return QByteArray();
}

int Editor::lineCount()
{
    return asyncSendMessageWithResult("C_FUN_GET_LINE_COUNT").get().toInt();
}

void Editor::selectAll()
{
    m_scintilla->selectAll();
}

void Editor::undo()
{
    m_scintilla->undo();
}

void Editor::redo()
{
    m_scintilla->redo();
}

void Editor::deleteCurrentLine()
{
    m_scintilla->lineDelete();
}

void Editor::duplicateCurrentLine()
{
    m_scintilla->lineDuplicate();
}

void Editor::moveCurrentLineUp()
{
    m_scintilla->moveSelectedLinesUp();
}

void Editor::moveCurrentLineDown()
{
    m_scintilla->moveSelectedLinesDown();
}

void Editor::trimTrailingWhitespaces()
{
    m_scintilla->beginUndoAction();
    replaceAllNoCheckpoint("\\s+$", SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), "");
    m_scintilla->endUndoAction();
}

void Editor::trimLeadingWhitespaces()
{
    m_scintilla->beginUndoAction();
    replaceAllNoCheckpoint("^\\s+", SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), "");
    m_scintilla->endUndoAction();
}

void Editor::trimLeadingAndTrailingWhitespaces()
{
    m_scintilla->beginUndoAction();
    replaceAllNoCheckpoint("^\\s+", SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), "");
    replaceAllNoCheckpoint("\\s+$", SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), "");
    m_scintilla->endUndoAction();
}

void Editor::convertEolToSpace()
{
    fprintf(stderr, "FIXME: Implement Editor::convertEolToSpace()\n");
}

void Editor::convertTabsToSpaces()
{
    IndentationMode indentation = indentationMode();

    QString spacesReplacement = QString(" ").repeated(indentation.size);

    m_scintilla->beginUndoAction();
    replaceAllNoCheckpoint("\\t", SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), spacesReplacement);
    m_scintilla->endUndoAction();
}

void Editor::convertAllSpacesToTabs()
{
    IndentationMode indentation = indentationMode();

    QString regex = QString(" ").repeated(indentation.size);

    m_scintilla->beginUndoAction();
    replaceAllNoCheckpoint(regex, SearchHelpers::SearchMode::Regex, SearchHelpers::SearchOptions(), "\t");
    m_scintilla->endUndoAction();
}

void Editor::convertLeadingSpacesToTabs()
{
    fprintf(stderr, "FIXME: Implement Editor::convertLeadingSpacesToTabs()\n");
}

void Editor::search(const QString &string,
                    SearchHelpers::SearchMode searchMode,
                    bool forward,
                    const SearchHelpers::SearchOptions &searchOptions)
{
    searchAndSelect(false, string, searchMode, forward, searchOptions);
}

void Editor::replace(const QString &string,
                     SearchHelpers::SearchMode searchMode,
                     bool forward,
                     const SearchHelpers::SearchOptions &searchOptions,
                     const QString &replacement)
{
    bool isFoundAndSelected = searchAndSelect(true, string, searchMode, forward, searchOptions);

    if (!isFoundAndSelected)
    {
        isFoundAndSelected = searchAndSelect(false, string, searchMode, forward, searchOptions);
    }

    if (isFoundAndSelected)
    {
        m_scintilla->replace(replacement);
    }
}

} //namespace EditorNS
