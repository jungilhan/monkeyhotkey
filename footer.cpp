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

#include "footer.h"
#include "config.h"

#include <QHBoxLayout>
#include <QPushButton>

Footer::Footer(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
    , m_layout(0)
    , m_deleteButton(0)
    , m_setupButton(0)
    , m_infoButton(0)
{
    init();
}

Footer::~Footer()
{
    delete m_deleteButton;
    delete m_setupButton;
    delete m_infoButton;
    delete m_layout;
}

void Footer::init()
{
    QString style =  "QPushButton:hover {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #A6A6A6, stop: 0.04 #6f6f6f, stop: 0.05 #616161, stop: 1 #3A3A3A);} \
                      QPushButton {color: white; \
                                   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #969696, stop: 0.04 #5f5f5f, stop: 0.05 #515151, stop: 1 #2A2A2A); \
                                   border-width: 1px; \
                                   border-color: #2C2C2C; \
                                   border-style: hidden solid;}";

    m_deleteButton = new QPushButton(trUtf8("삭제"), this);
    m_deleteButton->setIcon(QIcon(":trash24x24"));
    m_deleteButton->setMinimumHeight(FOOTER_HEIGHT);
    m_deleteButton->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setStyleSheet(style);
    connect(m_deleteButton, SIGNAL(clicked()), this, SIGNAL(deleteClicked()));

    m_setupButton = new QPushButton(trUtf8("설정"), this);
    m_setupButton->setIcon(QIcon(":setup24x24"));
    m_setupButton->setMinimumHeight(FOOTER_HEIGHT);
    m_setupButton->setFocusPolicy(Qt::NoFocus);
    m_setupButton->setStyleSheet(style);
    connect(m_setupButton, SIGNAL(clicked()), this, SIGNAL(setupClicked()));

    m_infoButton = new QPushButton(trUtf8("정보"), this);
    m_infoButton->setIcon(QIcon(":info24x24"));
    m_infoButton->setMinimumHeight(FOOTER_HEIGHT);
    m_infoButton->setFocusPolicy(Qt::NoFocus);
    m_infoButton->setStyleSheet(style);
    connect(m_infoButton, SIGNAL(clicked()), this, SIGNAL(infoClicked()));

    m_layout = new QHBoxLayout();
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_deleteButton);
    m_layout->addWidget(m_setupButton);
    m_layout->addWidget(m_infoButton);
    setLayout(m_layout);
}
