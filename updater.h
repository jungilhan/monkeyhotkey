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

#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include "http.h"

class progressDialog;

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    ~Updater();

signals:
    void updateAvailable(const QString& version);

public:
    void start(bool silentMode = false);

private:
    void init();
    void startDownloadXml();
    void startDownloadBinary();
    void startPatch();
    void parseXml(const QByteArray& data);
    bool isAvailable();

private slots:
    void downloadXmlFinished(QIODevice* data, bool error);
    void downloadBinaryFinished(QIODevice* data, bool error);
    void downloadBinaryProgress(int done, int total);
    void patchRequested();
    void patchCancled();

private:
    Http* m_xmlDownloader;
    Http* m_binaryDownloader;
    progressDialog* m_progressDialog;

    QString m_name;
    QString m_version;
    QString m_versionStr;
    QString m_downloadUrl;
    QString m_webPage;
    QString m_patchFile;

    bool m_silentMode;
};

#endif // UPDATER_H
