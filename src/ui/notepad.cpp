#include "include/notepad.h"

#include "include/Extensions/extensionsloader.h"
#include "include/Extensions/runtimesupport.h"
#include "include/npsettings.h"

#include <QCheckBox>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

const QString Notepad::version = POINTVERSION;
const QString Notepad::contributorsUrl = "https://github.com/notepad/notepad/graphs/contributors";
const QString Notepad::website = "https://notepad.com";

QString Notepad::copyright()
{
    return QString::fromUtf8("Copyright © 2010-%1, Daniele Di Sarli").arg(COPYRIGHT_YEAR);
}

QString Notepad::appDataPath(QString fileName)
{
#ifdef Q_OS_MACX
    QString def = QString("%1/../Resources/").arg(qApp->applicationDirPath());
#else
    QString def = QString("%1/../appdata/").arg(qApp->applicationDirPath());
#endif

    if (!QDir(def).exists())
        def = QString("%1/../share/%2/").arg(qApp->applicationDirPath()).arg(qApp->applicationName().toLower());

    if (!fileName.isNull())
    {
        def.append(fileName);
    }

    return def;
}

QString Notepad::editorPath()
{
    return appDataPath("editor/index.html");
}

QString Notepad::extensionToolsPath()
{
    return appDataPath("extension_tools");
}

QString Notepad::nodejsPath()
{
    NpSettings &s = NpSettings::getInstance();
    return s.Extensions.getRuntimeNodeJS();
}

QString Notepad::npmPath()
{
    NpSettings &s = NpSettings::getInstance();
    return s.Extensions.getRuntimeNpm();
}

QString Notepad::fileNameFromUrl(const QUrl &url)
{
    return QFileInfo(url.toDisplayString(
                         QUrl::RemoveScheme |
                         QUrl::RemovePassword |
                         QUrl::RemoveUserInfo |
                         QUrl::RemovePort |
                         QUrl::RemoveAuthority |
                         QUrl::RemoveQuery |
                         QUrl::RemoveFragment |
                         QUrl::PreferLocalFile))
        .fileName();
}

QSharedPointer<QCommandLineParser> Notepad::getCommandLineArgumentsParser(const QStringList &arguments)
{
    QSharedPointer<QCommandLineParser> parser =
        QSharedPointer<QCommandLineParser>(new QCommandLineParser());

    parser->setApplicationDescription("Text editor for developers");
    parser->addHelpOption();
    parser->addVersionOption();

    QCommandLineOption newWindowOption("new-window",
                                       QObject::tr("Open a new window in an existing instance of %1.")
                                           .arg(QCoreApplication::applicationName()));
    parser->addOption(newWindowOption);

    QCommandLineOption setLine({"l", "line"},
                               QObject::tr("Open file at specified line."),
                               "line",
                               "0");
    parser->addOption(setLine);

    QCommandLineOption setCol({"c", "column"},
                              QObject::tr("Open file at specified column."),
                              "column",
                              "0");
    parser->addOption(setCol);

    QCommandLineOption allowRootOption("allow-root", QObject::tr("Allows Notepad to be run as root."));
    parser->addOption(allowRootOption);

    QCommandLineOption printDebugOption("print-debug-info", QObject::tr("Print system information for debugging."));
    parser->addOption(printDebugOption);

    parser->addPositionalArgument("urls",
                                  QObject::tr("Files to open."),
                                  "[urls...]");

    parser->process(arguments);

    return parser;
}

QString Notepad::extensionsPath()
{
    QSettings settings;

    QFileInfo f = QFileInfo(settings.fileName());
    return f.absoluteDir().absoluteFilePath("extensions");
}

QList<QString> Notepad::translations()
{
    QList<QString> out;

    QDir dir(":/translations");
    QStringList fileNames = dir.entryList(QStringList("notepad_*.qm"));

    // FIXME this can be removed if we create a .qm file for English too, which should exist for consistency purposes
    out.append("en");

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString langCode;
        langCode = fileNames[i]; // "notepad_de.qm"
        langCode.truncate(langCode.lastIndexOf('.')); // "notepad_de"
        langCode.remove(0, langCode.indexOf('_') + 1); // "de"

        out.append(langCode);
    }

    return out;
}

void Notepad::printEnvironmentInfo()
{
    qDebug() << QString("Notepad: %1").arg(POINTVERSION).toStdString().c_str();
#ifdef BUILD_SNAP
    qDebug() << "Snap build: yes";
#else
    qDebug() << "Snap build: no";
#endif
    qDebug() << QString("Qt: %1 - %2").arg(qVersion(), QSysInfo::buildAbi()).toStdString().c_str();
    qDebug() << QString("OS: %1 (%2 %3)")
                    .arg(QSysInfo::prettyProductName(), QSysInfo::productType(), QSysInfo::productVersion())
                    .toStdString()
                    .c_str();
    qDebug() << QString("CPU: %1").arg(QSysInfo::currentCpuArchitecture()).toStdString().c_str();
    qDebug() << QString("Kernel: %1 - %2").arg(QSysInfo::kernelType(), QSysInfo::kernelVersion()).toStdString().c_str();
}
