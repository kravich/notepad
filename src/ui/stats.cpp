#include "include/stats.h"

#include "include/notepad.h"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QSysInfo>
#include <QTimer>
#include <QUrl>

bool Stats::m_longTimerRunning = false;
bool Stats::m_isFirstNotepadRun = false;

#define DIALOG_NEVER_SHOWN 0
#define DIALOG_ALREADY_SHOWN 1
#define DIALOG_FIRST_TIME_IGNORED 2

void Stats::init()
{
    NpSettings &settings = NpSettings::getInstance();

    Stats::askUserPermission();

    // Check whether the user wants us to collect stats. If not, return.
    if (!settings.General.getCollectStatistics())
    {
        return;
    }

    // Start a timer that will check very soon if we need to send stats.
    QTimer *t = new QTimer();
    t->setTimerType(Qt::VeryCoarseTimer);
    QObject::connect(t, &QTimer::timeout, [t]() {
        Stats::check();
        t->deleteLater();
    });

    // Start after 10 seconds: we don't want to take time to the startup sequence
    t->start(10000);

    // Also start another timer that will periodically check if a week has passed and
    // it's time to transmit new information.
    if (!m_longTimerRunning)
    {
        QTimer *tlong = new QTimer();
        tlong->setTimerType(Qt::VeryCoarseTimer);
        QObject::connect(tlong, &QTimer::timeout, [t]() {
            Stats::check();
        });

        tlong->start(12 * 60 * 60 * 1000); // Check every ~12 hours.

        m_longTimerRunning = true;
    }
}

void Stats::check()
{
    // Check whether the user wants us to collect stats. If not, return.
    NpSettings &settings = NpSettings::getInstance();
    if (!settings.General.getCollectStatistics())
    {
        return;
    }

    // Check if it is time to send the stats (i.e. a week has passed).
    // If not, return.
    if (!isTimeToSendStats())
    {
        return;
    }
    settings.General.setLastStatisticTransmissionTime(currentUnixTimestamp());

    QJsonObject data;
    data["version"] = QString(POINTVERSION);
#ifndef BUILD_SNAP
    data["qt_version"] = QString(qVersion());
#else
    data["qt_version"] = QString(qVersion()) + " (Snap)";
#endif

#if QT_VERSION >= 0x050400
    data["os"] = QSysInfo::productType();
    data["os_version"] = QSysInfo::productVersion();
#endif

    Stats::remoteApiSend(data);
}

void Stats::remoteApiSend(const QJsonObject &data)
{
    QUrl url("https://notepad.com/api/stat/post.php");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/javascript");

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QObject::connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *) {
        manager->deleteLater();
    });

    QJsonDocument doc;
    doc.setObject(data);

    manager->post(request, doc.toJson(QJsonDocument::Compact));
}

void Stats::askUserPermission()
{
    NpSettings &settings = NpSettings::getInstance();
    int dialogShown = settings.General.getStatisticsDialogShown();

    if (dialogShown == DIALOG_FIRST_TIME_IGNORED && !m_isFirstNotepadRun)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QCoreApplication::applicationName());
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("<h3>" + QObject::tr("Would you like to help?") + "</h3>");
        msgBox.setInformativeText(
            /* clang-format off */
            "<html><body>"
            "<p>" + QObject::tr("You can help to improve Notepad by allowing us to collect <b>anonymous statistics</b>.") + "</p>" +
            "<b>" + QObject::tr("What will we collect?") + "</b><br>" +
            QObject::tr(
                "We will collect information such as the version of Qt or the version of the OS<br>"
                "You don't have to trust us: Notepad is open source, so you can %1check by yourself%2 😊").
                      arg("<a href=\"https://github.com/notepad/notepad/blob/master/src/ui/stats.cpp\">").
                      arg("</a>") +
            "</body></html>"
            /* clang-format on */
        );

        QAbstractButton *ok = msgBox.addButton(QObject::tr("Okay, I agree"), QMessageBox::AcceptRole);
        msgBox.addButton(QObject::tr("No"), QMessageBox::RejectRole);

        msgBox.exec();

        settings.General.setStatisticsDialogShown(DIALOG_ALREADY_SHOWN);

        if (msgBox.clickedButton() == ok)
        {
            settings.General.setCollectStatistics(true);
        }
        else
        {
            settings.General.setCollectStatistics(false);
        }
    }
    else if (dialogShown == DIALOG_NEVER_SHOWN)
    {
        // Set m_isFirstNotepadRun to true, so that next executions of this method within
        // the current process won't show the dialog even if we're setting
        // statisticsDialogShown = DIALOG_FIRST_TIME_IGNORED.
        m_isFirstNotepadRun = true;
        settings.General.setStatisticsDialogShown(DIALOG_FIRST_TIME_IGNORED);
    }
}

bool Stats::isTimeToSendStats()
{
    NpSettings &settings = NpSettings::getInstance();
    return (currentUnixTimestamp() - settings.General.getLastStatisticTransmissionTime()) >= 7 * 24 * 60 * 60 * 1000;
}

qint64 Stats::currentUnixTimestamp()
{
#if QT_VERSION >= 0x050800
    return QDateTime::currentDateTime().toSecsSinceEpoch();
#else
    return QDateTime::currentDateTime().toTime_t();
#endif
}
