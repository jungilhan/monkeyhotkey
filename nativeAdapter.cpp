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

#include "nativeAdapter.h"
#include "hotkeyManager.h"
#include "webView.h"
#include "config.h"

NativeAdapter::NativeAdapter(QObject* parent)
    : QObject(parent)
{

}

void NativeAdapter::handleMessage(const QString& message,
                                  const QString& arg1, const QString& arg2)
{
#if DEBUG
    qDebug() << "[NativeAdapter::handleMessage] " << message << arg1 << arg2;
#endif

    if (message == "execute") {
        emit executeRequested(arg1.toInt());

    } else if (message == "remove") {
        QStringList indexList = arg1.split(",");
        emit removeRequested(indexList);

    } else if (message == "hotkey") {
        QString key;
        QStringList modifiers;

        foreach (const QString& _key, arg2.split(" + ")) {
            if (_key == "Ctrl" || _key == "Alt" || _key == "Shift")
                modifiers << _key.trimmed();
            else
                key = _key.trimmed();
        }

        emit hotkeyChanged(arg1.toInt(), key, modifiers);

    } else if (message == "restore") {
        emit restoreRequested();

    } else if (message == "openUrl") {
        QString url = arg1;
        if (url.contains("http://", Qt::CaseInsensitive) == false)
            url = "http://" + url;

        emit openUrlRequested(QUrl(url));

    } else if (message == "update") {
        emit programUpdateRequested();

    } else if (message == "setup") {
        if (arg1 == "autorun") {
            emit setupAutorunChanged(arg2 == "on" ? true : false );
        } else if (arg1 == "topmost") {
            emit setupTopmostChanged(arg2 == "on" ? true : false );
        }
    } else if (message == "scrollTop") {
        emit scrollTopRequested();
    }

}
