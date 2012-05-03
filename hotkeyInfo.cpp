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

#include "hotkeyInfo.h"

HotkeyInfo::HotkeyInfo(const QString& name,
                       const QString& path,
                       const QString& iconPath,
                       bool isExecutable,
                       QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_path(path)
    , m_iconPath(iconPath)
    , m_isExecutable(isExecutable)
    , m_isRegistered(false)
{

}

HotkeyInfo::HotkeyInfo(const QString& name,
                       const QString& path,
                       const QString& iconPath,
                       bool isExecutable,
                       const QString& key,
                       const QStringList& modifiers,
                       QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_path(path)
    , m_iconPath(iconPath)
    , m_isExecutable(isExecutable)
    , m_key(key)
    , m_modifiers(modifiers)
    , m_isRegistered(false)
{

}

QString HotkeyInfo::name() const
{
    return m_name;
}

QString HotkeyInfo::path() const
{
    return m_path;
}

QString HotkeyInfo::iconPath() const
{
    return m_iconPath;
}

bool HotkeyInfo::isExecutable() const
{
    return m_isExecutable;
}

QString HotkeyInfo::key() const
{
    return m_key;
}

void HotkeyInfo::setKey(const QString& key)
{
    m_key = key;
}

QStringList HotkeyInfo::modifiers() const
{
    return m_modifiers;
}

void HotkeyInfo::setModifiers(const QStringList& modifiers)
{
    m_modifiers = modifiers;
}

bool HotkeyInfo::isRegistered()
{
    return m_isRegistered;
}

void HotkeyInfo::setRegistered(bool on)
{
    m_isRegistered = on;
}
