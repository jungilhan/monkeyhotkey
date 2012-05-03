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

#ifndef HOTKEYINFO_H
#define HOTKEYINFO_H

#include <QtCore>

class HotkeyInfo : public QObject
{
    Q_OBJECT
public:
    explicit HotkeyInfo(const QString& name,
                        const QString& path,
                        const QString& iconPath,
                        bool isExecutable = false,
                        QObject *parent = 0);

    explicit HotkeyInfo(const QString& name,
                        const QString& path,
                        const QString& iconPath,
                        bool isExecutable,
                        const QString& key,
                        const QStringList& modifiers,
                        QObject *parent = 0);

public:
    QString name() const;
    QString path() const;
    QString iconPath() const;
    bool isExecutable() const;

    QString key() const;
    void setKey(const QString& key);

    QStringList modifiers() const;
    void setModifiers(const QStringList& modifiers);

    bool isRegistered();
    void setRegistered(bool on);

private:
    QString m_name;
    QString m_path;
    QString m_iconPath;
    bool m_isExecutable;
    QString m_key;
    QStringList m_modifiers;
    bool m_isRegistered;
};

#endif // HOTKEYINFO_H
