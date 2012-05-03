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

#ifndef HTTP_H
#define HTTP_H

#include <QtNetwork/QHttp>

class QUrl;
class QByteArray;

class Http : public QHttp
{
    Q_OBJECT
public:
    explicit Http(QObject* parent = 0);
    ~Http();

signals:
    void readFinished(QIODevice*, bool);

public:
    int connectUrl(const QString& url, QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp);

private slots:
    void readResponseHeader(const QHttpResponseHeader& responseHeader);
    void httpRequestFinished(int requestId, bool error);

private:
    int m_id;
    QUrl* m_url;
    QIODevice* m_data;
};

#endif // HTTP_H
