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

#include "aboutDialog.h"

#include <QtGui>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
    , m_logoBackground(0)
    , m_logo(0)
    , m_title(0)
    , m_content(0)
    , m_thanksAreaTitle(0)
    , m_thanksArea(0)
    , m_closeButton(0)
{
    init();
}

AboutDialog::~AboutDialog()
{
    delete m_logoBackground;
    delete m_logo;
    delete m_title;
    delete m_content;
    delete m_thanksAreaTitle;
    delete m_thanksArea;
    delete m_closeButton;
}

void AboutDialog::init()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    resize(520, 480);
    setFixedSize(520, 480);
    setWindowTitle("About - Monkey Hotkey");

    m_logoBackground = new QLabel(this);
    m_logoBackground->setGeometry(10, 10, 148, 460);
    m_logoBackground->setStyleSheet("QLabel { border: 1px solid #8f8f91; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1 stop: 0 #d6d6d6, stop: 1 #737373); }");

    m_logo = new QLabel(this);
    m_logo->setGeometry(20, 20, 128, 128);
    m_logo->setStyleSheet("border: none;");
    m_logo->setPixmap(QPixmap(":monkeyhotkey128x128"));

    m_title = new QLabel(this);
    m_title->move(168, 10);
    m_title->setFont(QFont(QApplication::font().family(), 12, QFont::Bold));
    m_title->setText(trUtf8("Monkey Hotkey 2.0.0 r211"));

    m_content = new QLabel(this);
    m_content->setGeometry(168, 40, 342, 220);
    m_content->setFont(QFont(QApplication::font()));
    m_content->setTextFormat(Qt::RichText);
    m_content->setWordWrap(true);
    m_content->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_content->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_content->setText(trUtf8("Monkey Hotkey is Free Software and may be distributed according to the terms of the GNU General Public License.<br><br>\
                              Copyright (C) 2009 - 2012 jungil han<br>\
                              Maintainer jungil.han@gmail.com<br><br>\
                              안녕하세요. 몽키 핫키 개발자입니다. 소스코드를 자유롭게 사용하도록 공개해 주신 그리고 영감을 주는 많은 개발자분들에게 감사드립니다. 소소한 프로그램이지만 유용하게 사용하신다면 더할 나위없이 기쁠 것 같습니다. 버그를 발견하시거나 개선했으면 하는 부분이 있으면 언제든지 문의해주세요 :D"));

    m_thanksAreaTitle = new QLabel(this);
    m_thanksAreaTitle->move(168, 210);
    m_thanksAreaTitle->setFont(QFont(QApplication::font()));
    m_thanksAreaTitle->setText(trUtf8("도움주신 분들"));

    m_thanksArea = new QTextEdit(this);
    m_thanksArea->setGeometry(168, 235, 342, 200);
    m_thanksArea->setReadOnly(true);
    m_thanksArea->setText(trUtf8("김지운 <spongetaco@gmail.com>\n김경민 <v8877@naver.com>"));

    m_closeButton = new QPushButton(trUtf8("확인"), this);
    m_closeButton->setGeometry(430, 445, 80, 25);
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(hide()));
}

void AboutDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::postEvent(this, new QEvent(QEvent::Close));
    } else {
        event->accept();
    }
}

void AboutDialog::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}
