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

#ifndef NATIVEADAPTER_H
#define NATIVEADAPTER_H

#include <QtCore>

class NativeAdapter : public QObject
{
    Q_OBJECT
public:
    explicit NativeAdapter(QObject* parent = 0);

signals:
    void executeRequested(int index);
    void removeRequested(const QStringList& indexList);
    void hotkeyChanged(int index, const QString& key, const QStringList& modifiers);
    void restoreRequested();
    void openUrlRequested(const QUrl& url);
    void programUpdateRequested();
    void scrollTopRequested();
    void setupAutorunChanged(bool enable);
    void setupTopmostChanged(bool enable);

public slots:
    void handleMessage(const QString& message,
                       const QString& arg1, const QString& arg2);
};

#endif // NATIVEADAPTER_H
