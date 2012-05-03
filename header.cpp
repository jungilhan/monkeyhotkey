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

#include "header.h"
#include "config.h"
#include "roundButton.h"

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QApplication>

Header::Header(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
    , m_label(0)
    , m_aboutButton(0)
    , m_closeButton(0)
{
    init();
}

Header::~Header()
{
    delete m_label;
    delete m_aboutButton;
    delete m_closeButton;
}

void Header::init()
{
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setFont(QFont(QApplication::font().family(), 14, QFont::Normal));
    //m_label->setText(trUtf8("몽키 핫키 2"));
    m_label->setPixmap(QPixmap(":title"));
    m_label->resize(FRAME_WIDTH, HEADER_HEIGHT);
    m_label->installEventFilter(this);
    m_label->setStyleSheet("color: white; \
                            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #969696, stop: 0.08 #5f5f5f, stop: 0.49999 #515151, stop: 0.5 #323232, stop: 0.9 #1c1c1c, stop: 1 #000000)");

    m_aboutButton = new RoundButton(":about18x18", "#ADC6A6", this);
    m_aboutButton->move(10, 8);
    connect(m_aboutButton, SIGNAL(clicked()), this, SIGNAL(aboutClicked()));

    m_closeButton = new RoundButton(":close18x18", "white", this);
    m_closeButton->move(FRAME_WIDTH - 26 - 10, 8);
    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(closeClicked()));
}

void Header::resizeEvent(QResizeEvent* event)
{
    m_closeButton->move(event->size().width() - 26 - 10, 8);
    m_label->resize(event->size().width(), HEADER_HEIGHT);
}

bool Header::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_label) {
        if (event->type() == QEvent::MouseMove) {
            if (((QMouseEvent*)event)->buttons() & Qt::LeftButton) {
                parentWidget()->move(((QMouseEvent*)event)->globalPos() - m_dragPos);
                return true;
            } else {
                return false;
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            if (((QMouseEvent*)event)->button() == Qt::LeftButton) {
                m_dragPos = ((QMouseEvent*)event)->globalPos() - parentWidget()->frameGeometry().topLeft();
                return true;
            } else {
                return false;
            }
        } else if (event->type() == QEvent::Enter) {
            QApplication::setOverrideCursor(Qt::SizeAllCursor);
            return false;
        } else if (event->type() == QEvent::Leave) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    }

    return QFrame::eventFilter(watched, event);
}
