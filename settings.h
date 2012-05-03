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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();
    
public:
    void beginGroup(const QString& prefix);
    void endGroup();

    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value);

    int	beginReadArray(const QString& prefix);
    void beginWriteArray(const QString& prefix, int size = -1);
    void setArrayIndex(int i);
    void endArray();

    void removeGroup(const QString& prefix);
    bool contains(const QString& key) const;

private:
    QSettings* m_settings;
};

#endif // SETTINGS_H
