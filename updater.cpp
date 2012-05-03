/*
    Copyright (C) 2012  jungil han <jungil.han@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include "updater.h"
#include "config.h"
#include "mainWindow.h"
#include "progressDialog.h"

#include <QtCore>
#include <QtGui>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif

#define XML_URL "http://monkeyhotkey.googlecode.com/files/update.xml"

Updater::Updater(QObject *parent)
    : QObject(parent)
    , m_xmlDownloader(new Http(this))
    , m_binaryDownloader(new Http(this))
    , m_progressDialog(0)
    , m_silentMode(false)
{
    init();
}

Updater::~Updater()
{
    if (m_xmlDownloader->currentId())
        m_xmlDownloader->abort();

    if (m_binaryDownloader->currentId())
        m_binaryDownloader->abort();

    delete m_xmlDownloader;
    delete m_binaryDownloader;

    delete m_progressDialog;

}

void Updater::init()
{
    connect(m_xmlDownloader, SIGNAL(readFinished(QIODevice*,bool)), this, SLOT(downloadXmlFinished(QIODevice*,bool)));
    connect(m_binaryDownloader, SIGNAL(dataReadProgress(int,int)), this, SLOT(downloadBinaryProgress(int,int)));
    connect(m_binaryDownloader, SIGNAL(readFinished(QIODevice*,bool)), this, SLOT(downloadBinaryFinished(QIODevice*,bool)));
}

void Updater::start(bool silentMode)
{
    m_silentMode = silentMode;
    startDownloadXml();
}

void Updater::startDownloadXml()
{
    m_xmlDownloader->connectUrl(XML_URL);
}

void Updater::startDownloadBinary()
{
    m_binaryDownloader->connectUrl(m_downloadUrl);

    if (!m_progressDialog) {
        m_progressDialog = new progressDialog(MainWindow::instance());
        connect(m_progressDialog, SIGNAL(accepted()), this, SLOT(patchRequested()));
        connect(m_progressDialog, SIGNAL(rejected()), this, SLOT(patchCancled()));
    }

    m_progressDialog->setWindowTitle(trUtf8("업데이트"));
    m_progressDialog->setLabelText(trUtf8("패치를 내려받는 중입니다."));
    m_progressDialog->setAutoClose(5000);
    m_progressDialog->show();
}

void Updater::downloadXmlFinished(QIODevice* data, bool error)
{
    if (error) {
        if (!m_silentMode)
            QMessageBox::critical(MainWindow::instance(), trUtf8("업데이트 에러"), QString("Error Code: %1").arg(m_binaryDownloader->lastResponse().statusCode()) + "\n" + m_binaryDownloader->errorString());

#if DEBUG
        qDebug() << "Update Error" << m_binaryDownloader->lastResponse().statusCode() << m_binaryDownloader->errorString();
#endif
        return;
    }

    data->open(QIODevice::ReadOnly);
    parseXml(data->readAll());
    data->close();

    if (isAvailable()) {
        int result = QMessageBox::Cancel;

#if defined(Q_OS_WIN32)
        if (!m_silentMode)
            result = QMessageBox::information(MainWindow::instance(), trUtf8("업데이트 확인"), trUtf8("새로운 버전 '<b>") + m_versionStr + trUtf8("' </b>이 사용 가능합니다.<br/>내려받으시겠습니까?"),
                                              QMessageBox::Ok, QMessageBox::Cancel);
        else
            emit updateAvailable(m_versionStr);
#endif
        switch (result) {
        case QMessageBox::Ok:
#if defined(Q_OS_WIN32)
            startDownloadBinary();
#endif
            break;
        case QMessageBox::Cancel:
            break;
        }

    } else {
        if (!m_silentMode)
            QMessageBox::information(MainWindow::instance(), trUtf8("업데이트 확인"), trUtf8("이용 가능한 업데이트가 없습니다.<br/>'몽키 핫키'의 최신 버전을 사용하고 있습니다."));
    }
}

void Updater::downloadBinaryFinished(QIODevice* data, bool error)
{
    if (error) {
        if (m_progressDialog->isVisible())
            m_progressDialog->hide();

        QMessageBox::critical(MainWindow::instance(), trUtf8("업데이트 에러"), QString("Error Code: %1").arg(m_binaryDownloader->lastResponse().statusCode()) + "\n" + m_binaryDownloader->errorString());
        return;
    }

    data->open(QIODevice::ReadOnly);

    m_patchFile = QDir::tempPath() + "/" + m_name;

    QFile file(m_patchFile);
    file.open(QIODevice::WriteOnly);
    file.write(data->readAll());
    file.close();

    data->close();

    // [MEMO] 포터블 버전은 설치 디렉토리를 알 수 없으므로 현재 바이너리 위치를 레지스트리에 기록
    QSettings registry("HKEY_CURRENT_USER\\Software\\Monkey Hotkey", QSettings::NativeFormat);
    if (!registry.contains("InstallPath") ||
        registry.value("InstallPath", "").toString() != QDir::toNativeSeparators(QCoreApplication::applicationDirPath())) {
           QString value = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
           registry.setValue("InstallPath", value);
    }

    // [MEMO] 패치 적용.
    m_progressDialog->setLabelText(trUtf8("5초 후 재시작됩니다."));
}

void Updater::downloadBinaryProgress(int done, int total)
{
    Q_ASSERT(m_progressDialog);

    int percent = done * 100 / total;
    m_progressDialog->setValue(percent);
}

void Updater::patchRequested()
{
    startPatch();
}

void Updater::patchCancled()
{
    if (m_binaryDownloader->currentId())
        m_binaryDownloader->abort();
}

void Updater::startPatch()
{
#ifdef Q_OS_WIN
    m_patchFile.insert(0, "\"");
    m_patchFile += "\"";
    m_patchFile.replace("/", "\\");

    int result = (int)::ShellExecuteW(0, L"open", m_patchFile.toStdWString().c_str(), 0, 0, SW_SHOWNORMAL);
    if (result == SE_ERR_ACCESSDENIED) {
        // Requesting elevation
        result = (int)::ShellExecuteW(0, L"runas", m_patchFile.toStdWString().c_str(), 0, 0, SW_SHOWNORMAL);
    }
#endif

    qApp->quit();
}

void Updater::parseXml(const QByteArray& data)
{
    QXmlStreamReader xmlReader(data);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name() == "name") {
            m_name = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_name;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "version") {
            m_version = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_version;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "versionStr") {
            m_versionStr = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_versionStr;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "url") {
            m_downloadUrl = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_downloadUrl;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "webPage") {
            m_webPage = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_webPage;
#endif
        }
    }

    if (xmlReader.hasError())
        qDebug() << xmlReader.errorString();
}

bool Updater::isAvailable()
{
    int latestVersion = m_version.mid(QString("0x").length()).toInt();
    int currentVersion = QString(VERSION).mid(QString("0x").length()).toInt();

#if DEBUG
    qDebug() << latestVersion;
    qDebug() << currentVersion;
#endif

    return latestVersion > currentVersion ? true : false;
}
