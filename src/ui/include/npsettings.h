#ifndef NPSETTINGS_H
#define NPSETTINGS_H

#include <QAction>
#include <QList>
#include <QSettings>
#include <QString>

/*
 * The use of NP_SETTING creates several functions:
 * getXXX(), setXXX(value), resetXXX(), hasXXX()
 *
 * hasXXX() returns whether the setting exists or not.
 *
 * NP_SETTING_WITH_KEY creates the same functions, but inserts an additional key param.
 * This is useful when there are a variable number of settings.
 *
 * BEGIN_CATEGORY and END_CATEGORY create a nested class that is used to group settings
 * together. Because of a quick of QSettings, BEGIN_GENERAL_CATEGORY has to be used if the
 * keys are supposed to be written to the [General] section of the .ini file.
 *
 * The BEGIN_CATEGORY macro automagically generates two member variables:
 * [const QString _m_category] is the name of the category given to the macro.
 * [QSettings _m_settings] is the QSettings objects from NpSettings.
 * It is a good idea to prefix all private members with _ when creating custom categories.
 * This way they will be moved to the very bottom of the auto-complete list.
 *
 * */

#define NP_SETTING(Name, Type, Default) \
    Type get##Name() const { return _m_settings.value(_m_category+#Name,Default).value<Type>(); } \
    void set##Name(const Type& newValue) { _m_settings.setValue(_m_category+#Name, newValue); } \
    Type reset##Name() { _m_settings.setValue(_m_category+#Name,Default); return Default; } \
    bool has##Name() const { return _m_settings.contains(_m_category+#Name); }

#define NP_SETTING_WITH_KEY(Name, Type, Default) \
    Type get##Name(const QString& key) const { return _m_settings.value(_m_category+key+"/"#Name,Default).value<Type>(); } \
    void set##Name(const QString& key, const Type& newValue) { _m_settings.setValue(_m_category+key+"/"#Name, newValue); } \
    Type reset##Name(const QString& key) { _m_settings.setValue(_m_category+key+"/"#Name,Default); return Default; } \
    bool has##Name(const QString& key) const { return _m_settings.contains(_m_category+key+"/"#Name); }

#define BEGIN_CATEGORY(Name) \
    class _Category##Name { \
    private: \
    QSettings& _m_settings; \
    const QString _m_category = #Name"/"; \
    public: \
    _Category##Name(QSettings& settings) : _m_settings(settings) {} \

#define BEGIN_GENERAL_CATEGORY(Name) \
    class _Category##Name { \
    private: \
    QSettings& _m_settings; \
    const QString _m_category; \
    public: \
    _Category##Name(QSettings& settings) : _m_settings(settings) {} \

#define END_CATEGORY(Name) \
    }; _Category##Name Name = _Category##Name(_m_settings);


class NpSettings {

public:
    BEGIN_GENERAL_CATEGORY(General)
        /* clang-format off */
        NP_SETTING(Localization,                   QString,    "")
        NP_SETTING(WarnForDifferentIndentation,    bool,       true)
        NP_SETTING(ExitOnLastTabClose,             bool,       false)

        NP_SETTING(CollectStatistics,              bool,       false)
        NP_SETTING(LastStatisticTransmissionTime,  qint64,     0)
        NP_SETTING(StatisticsDialogShown,          int,        0)

        NP_SETTING(WordWrap,                       bool,       false)
        NP_SETTING(Zoom,                           qreal,      1.0)

        NP_SETTING(ShowAllSymbols,                 bool,       false)
        NP_SETTING(TabsVisible,                    bool,       false)
        NP_SETTING(SpacesVisisble,                 bool,       false)
        NP_SETTING(ShowEOL,                        bool,       false)

        NP_SETTING(RememberTabsOnExit,             bool,       true)
        NP_SETTING(AutosaveInterval,               int,        15)      // In seconds
        NP_SETTING(LastSelectedDir,                QString,    ".")
        NP_SETTING(LastSelectedSessionDir,         QString,    QString())
        NP_SETTING(RecentDocuments,                QList<QVariant>, QList<QVariant>())
        NP_SETTING(WarnIfFileLargerThan,           int,        1)

        NP_SETTING(NotepadVersion,               QString,    QString())
        NP_SETTING(SmartIndentation,               bool,       true)
        NP_SETTING(MathRendering,                  bool,       true)
        NP_SETTING(UseNativeFilePicker,            bool,       true)
        /* clang-format on */
    END_CATEGORY(General)

    BEGIN_CATEGORY(Appearance)
        /* clang-format off */
        NP_SETTING(ColorScheme,        QString,    "")
        NP_SETTING(OverrideFontFamily, QString,    "")
        NP_SETTING(OverrideFontSize,   int,        0)
        NP_SETTING(OverrideLineHeight, double,     0)
        NP_SETTING(ShowLineNumbers, bool,       true)
        /* clang-format on */
    END_CATEGORY(Appearance)

    BEGIN_CATEGORY(Search)
        /* clang-format off */
        NP_SETTING(SearchAsIType,  bool,           true)
        NP_SETTING(SaveHistory,    bool,           true)
        NP_SETTING(SearchHistory,  QStringList,    QStringList())
        NP_SETTING(ReplaceHistory, QStringList,    QStringList())
        NP_SETTING(FileHistory,    QStringList,    QStringList())
        NP_SETTING(FilterHistory,  QStringList,    QStringList())
        /* clang-format on */
    END_CATEGORY(Search)

    BEGIN_CATEGORY(Extensions)
        /* clang-format off */
        NP_SETTING(RuntimeNodeJS,  QString, QString())
        NP_SETTING(RuntimeNpm,     QString, QString())
        /* clang-format on */
    END_CATEGORY(Extensions)

    BEGIN_CATEGORY(Languages)
        /* clang-format off */
        NP_SETTING_WITH_KEY(IndentWithSpaces,      bool,   false)
        NP_SETTING_WITH_KEY(TabSize,               int,    4)
        NP_SETTING_WITH_KEY(UseDefaultSettings,    bool,   true)
        /* clang-format on */
    END_CATEGORY(Languages)

    BEGIN_CATEGORY(MainWindow)
        /* clang-format off */
        NP_SETTING(Geometry,       QByteArray, QByteArray())
        NP_SETTING(WindowState,    QByteArray, QByteArray())
        NP_SETTING(MenuBarVisible, bool,       true)
        NP_SETTING(ToolBarItems,   QString,    QString())
        /* clang-format on */
    END_CATEGORY(MainWindow)


    //A few of the more involved settings can't be handled like above.
    BEGIN_CATEGORY(Shortcuts)

        /**
         * @brief Since default shortcuts aren't stored in the settings, we have to set them manually.
         * @param actions List of actions that have or could have shortcuts.
         */
        void initShortcuts(const QList<QAction*>& actions){
            for (QAction* a : actions){
                if (a->objectName().isEmpty())
                    continue;

                const QString key = "Shortcuts/" + a->objectName();

                //Only update the current shortcut if it's actually set in the settings.
                QKeySequence shortcut = _m_settings.contains(key) ?
                                            _m_settings.value(key).toString() : a->shortcut();

                _m_shortcuts.insert( a->objectName(), _ActionItem{a->shortcut(), shortcut} );
            }
        }

        QKeySequence getDefaultShortcut(const QString& actionName) const {
            auto it = _m_shortcuts.find(actionName);

            if (it == _m_shortcuts.end())
                return QKeySequence();

            return it.value().defaultSequence;
        }

        QKeySequence getShortcut(const QString& actionName) const {
            auto it = _m_shortcuts.find(actionName);

            if (it == _m_shortcuts.end())
                return QKeySequence();

            return it.value().sequence;
        }

        void setShortcut(const QString& actionName, const QKeySequence& sequence){
            auto it = _m_shortcuts.find(actionName);

            if (it == _m_shortcuts.end())
                return;

            it.value().sequence = sequence;
            _m_settings.setValue("Shortcuts/" + actionName, sequence.toString());
        }

    private:
        struct _ActionItem {
            QKeySequence defaultSequence;
            QKeySequence sequence;
        };
        QMap<QString, _ActionItem> _m_shortcuts;

    END_CATEGORY(Shortcuts)
    BEGIN_CATEGORY(Run)
        void resetCommands()
        {
            _m_settings.beginGroup("Run");
            _m_settings.remove("");
            _m_settings.endGroup();
        }

        QMap <QString, QString> getCommands()
        {
            QMap <QString, QString> ret;
            _m_settings.beginGroup("Run");
            QStringList groups = _m_settings.childGroups();
            for (int i = 0; i < groups.size(); ++i) {
                _m_settings.beginGroup(groups.at(i));
                const QString& name = _m_settings.value("name","").toString();
                const QString& cmd = _m_settings.value("command","").toString();
                ret.insert(name, cmd);
                _m_settings.endGroup();
            }
            _m_settings.endGroup();
            return ret;
        }

        void setCommand(const QString &cmdName, const QString &cmdRun)
        {
            _m_settings.beginGroup("Run");
            const int pos = _m_settings.childGroups().size()+1;
            QString group = "c" + QString::number(pos);
            _m_settings.setValue(group + "/name", cmdName);
            _m_settings.setValue(group + "/command", cmdRun);
            _m_settings.endGroup();
        }

    END_CATEGORY(Run)

    /**
     * @brief Some keys have changed since v0.53. To maintain compatibility, this function
     *        parses through the QSettings file and fixes these entries.
     *        Should be called once at program initiation, before getInstance() is called.
     */
    static void ensureBackwardsCompatibility();

    static NpSettings& getInstance();

private:
    QSettings _m_settings;

    NpSettings(){}
    NpSettings& operator=(NpSettings&) = delete;
};

#endif // NPSETTINGS_H
