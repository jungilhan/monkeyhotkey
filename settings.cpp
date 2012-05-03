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

#include "settings.h"
#include "config.h"

#include <QtGui>

Settings::Settings(QObject *parent)
    : QObject(parent)
{
#if DEBUG
    qDebug() << QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "\\monkeyhotkey.ini");
#endif

    m_settings = new QSettings(
                QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "\\monkeyhotkey.ini"),
                QSettings::IniFormat);
}

Settings::~Settings()
{
    delete m_settings;
}

void Settings::beginGroup(const QString& prefix)
{
    m_settings->beginGroup(prefix);
}

void Settings::endGroup()
{
    m_settings->endGroup();
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value)
{
    m_settings->setValue(key, value);
}

int	Settings::beginReadArray(const QString& prefix)
{
    return m_settings->beginReadArray(prefix);
}

void Settings::beginWriteArray(const QString& prefix, int size)
{
    m_settings->beginWriteArray(prefix, size);
}

void Settings::setArrayIndex(int i)
{
    m_settings->setArrayIndex(i);
}

void Settings::endArray()
{
    m_settings->endArray();
}

void Settings::removeGroup(const QString& prefix)
{
    m_settings->beginGroup(prefix);
    m_settings->remove(QString());
    m_settings->endGroup();
}

bool Settings::contains(const QString& key) const
{
    return m_settings->contains(key);
}
