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

#include "roundButton.h"

#include <QtGui>
#include <QtCore>

RoundButton::RoundButton(const QString& image, QString backgroundColor, QWidget* parent)
    : QWidget(parent)
    , m_image(image)
    , m_backgroundColor(backgroundColor)
    , m_container(0)
    , m_button(0)
{
    init();
}

RoundButton::~RoundButton()
{
    delete m_button;
    delete m_container;
}

void RoundButton::init()
{
    m_container = new QPushButton(this);
    m_container->setFocusPolicy(Qt::NoFocus);
    m_container->resize(26, 26);
    m_container->setStyleSheet(QString("QPushButton:pressed { background-color: white; } \
                                        QPushButton:hover { background-color: #d9d9d9; } \
                                        QPushButton {border-radius: 13px; \
                                                     background-color: %1; }").arg(m_backgroundColor));
    connect(m_container, SIGNAL(clicked()), this, SIGNAL(clicked()));

    m_button = new QPushButton(m_container);
    m_button->setFocusPolicy(Qt::NoFocus);
    m_button->setStyleSheet(QString("QPushButton:pressed { background-color: #898989; } \
                                     QPushButton:hover { background-color: #696969; } \
                                     QPushButton { margin-left: 4px; \
                                                   margin-top: 4px; \
                                                   width: 18px; \
                                                   height: 18px; \
                                                   border-radius: 9px; \
                                                   background-color: #898989; \
                                                   background-image: url(%1); }").arg(m_image));
    connect(m_button, SIGNAL(clicked()), this, SIGNAL(clicked()));
}

void RoundButton::enterEvent(QEvent* event)
{
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
    QWidget::enterEvent(event);
}

void RoundButton::leaveEvent(QEvent* event)
{
    QApplication::restoreOverrideCursor();
    QWidget::leaveEvent(event);
}
