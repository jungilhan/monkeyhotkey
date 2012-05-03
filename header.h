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

#ifndef HEADER_H
#define HEADER_H

#include <QFrame>

class QLabel;
class RoundButton;

class Header : public QFrame
{
    Q_OBJECT
public:
    explicit Header(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~Header();

signals:
    void aboutClicked();
    void closeClicked();

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual bool eventFilter(QObject* watched, QEvent* event);
private:
    void init();

private:
    QLabel* m_label;
    RoundButton* m_aboutButton;
    RoundButton* m_closeButton;
    QPoint m_dragPos;
};

#endif // HEADER_H
