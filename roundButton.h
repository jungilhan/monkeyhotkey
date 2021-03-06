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

#ifndef ROUNDBUTTON_H
#define ROUNDBUTTON_H

#include <QWidget>

class QPushButton;

class RoundButton : public QWidget
{
    Q_OBJECT
public:
    explicit RoundButton(const QString& image, QString backgroundColor, QWidget* parent = 0);
    ~RoundButton();
    
signals:
    void clicked();

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);

private:
    void init();

private:
    QString m_image;
    QString m_backgroundColor;
    QPushButton* m_container;
    QPushButton* m_button;
};

#endif // ROUNDBUTTON_H
