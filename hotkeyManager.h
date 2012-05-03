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

#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include <QtCore>

class HotkeyInfo;

class HotkeyManager : public QObject
{
    Q_OBJECT
public:
    explicit HotkeyManager(QObject* parent = 0);
    ~HotkeyManager();

signals:
    void restoreFinished(HotkeyManager* hotkeyManager);

public:
    void add(const QList<QUrl>& programUrls);
    void remove(int index);
    void removeAll();

    int length() const;

    QString name(int index) const;
    QString path(int index) const;
    QString iconPath(int index) const;
    bool isExecutable(int index) const;

    QString key(int index) const;
    void setKey(int index, const QString& key);

    QStringList modifiers(int index) const;
    void setModifiers(int index, const QStringList& modifiers);

    QString hotkey(int index);

    void writeIni();
    void restore();

    void registerHotkey(int index, const QString& key, const QStringList& modifiers);
    void unregisterHotkey(int index);

    void registerHotkeyAll();
    void unregisterHotkeyAll();

private:
    QString iconPath(const QFileInfo& fileInfo);
    void removeIcon(int index);
    void readIni();

    int keyCodeFromString(const QString& key);
    int modifierCodeFromString(const QString& key);

    bool contains(const QString& path);

private:
    QList<HotkeyInfo*> m_hotkeys;
};

#endif // HOTKEYMANAGER_H
