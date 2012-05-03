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

#include "webView.h"
#include "mainWindow.h"
#include "nativeAdapter.h"
#include "hotkeyManager.h"
#include "updater.h"
#include "config.h"

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

WebView::WebView(QWidget* parent)
    : QWebView(parent)
    , m_nativeAdapter(new NativeAdapter(this))
    , m_hotkeyManager(new HotkeyManager(this))
    , m_updater(0)
{
    init();
}

WebView::~WebView()
{
    delete m_nativeAdapter;
    delete m_hotkeyManager;
    delete m_updater;
}

void WebView::init()
{
    setContextMenuPolicy(Qt::NoContextMenu);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    connect(this, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    connect(m_hotkeyManager, SIGNAL(restoreFinished(HotkeyManager*)), this, SLOT(onRestoreFinished(HotkeyManager*)));
    connect(m_nativeAdapter, SIGNAL(executeRequested(int)), this, SLOT(onExecuteRequested(int)));
    connect(m_nativeAdapter, SIGNAL(restoreRequested()), this, SLOT(onRestoreRequested()));
    connect(m_nativeAdapter, SIGNAL(removeRequested(QStringList)), this, SLOT(onRemovedRequested(QStringList)));
    connect(m_nativeAdapter, SIGNAL(hotkeyChanged(int,QString,QStringList)), this, SLOT(onHotkeyChanged(int,QString,QStringList)));
    connect(m_nativeAdapter, SIGNAL(openUrlRequested(QUrl)), this, SLOT(onOpenUrlRequested(QUrl)));
    connect(m_nativeAdapter, SIGNAL(programUpdateRequested()), this, SLOT(onProgramUpdateRequested()));
    connect(m_nativeAdapter, SIGNAL(scrollTopRequested()), this, SLOT(onScrollTopRequested()));
    connect(m_nativeAdapter, SIGNAL(setupAutorunChanged(bool)), this, SLOT(onSetupAutorunChanged(bool)));
    connect(m_nativeAdapter, SIGNAL(setupTopmostChanged(bool)), this, SLOT(onSetupTopmostChanged(bool)));
}

void WebView::sendMessage(const QString& message, const QString& arg1,
                          const QString& arg2, const QString& arg3)
{
    QString js = QString("handleMessage(\"%1\", \"%2\", \"%3\", \"%4\")").arg(message).arg(arg1).arg(arg2).arg(arg3);
    page()->mainFrame()->evaluateJavaScript(js);
}

void WebView::dropEvent(QDropEvent* event)
{
    QList<QUrl> programUrls = event->mimeData()->urls();
    m_hotkeyManager->add(programUrls);
}

void WebView::onLoadStarted()
{
    page()->mainFrame()->addToJavaScriptWindowObject("nativeAdapter", m_nativeAdapter);
}

void WebView::onExecuteRequested(int index)
{
    QString programPath = m_hotkeyManager->path(index);

    if (m_hotkeyManager->isExecutable(index)) {
        bool status = QProcess::startDetached("\"" + programPath + "\"");
        if (!status)
            QDesktopServices::openUrl(QUrl("file:///" + programPath));
    } else {
        QDesktopServices::openUrl(QUrl("file:///" + programPath));
    }
}

void WebView::onRestoreRequested()
{
    m_hotkeyManager->restore();
}

void WebView::onRestoreFinished(HotkeyManager* hotkeyManager)
{
    for (int index = 0; index < hotkeyManager->length(); index++)
        sendMessage("restore", hotkeyManager->iconPath(index), hotkeyManager->name(index), hotkeyManager->hotkey(index));

    QString isAutorunEnabled = MainWindow::instance()->isAutorunEnabled() ? "on" : "off";
    QString isTopmostEnabled = MainWindow::instance()->isTopmostEnabled() ? "on" : "off";
    sendMessage("setup", isAutorunEnabled, isTopmostEnabled);
}

void WebView::onRemovedRequested(const QStringList& indexList)
{
    // [MEMO] 순차적으로 삭제시 리스트의 인덱스가 변화하므로 역순으로 삭제
    for (int index = indexList.length() - 1; index >= 0; index--)
        m_hotkeyManager->remove(indexList.at(index).toInt());
}

void WebView::onHotkeyChanged(int index, const QString& key, const QStringList& modifiers)
{
    m_hotkeyManager->setKey(index, key);
    m_hotkeyManager->setModifiers(index, modifiers);

#if DEBUG
    qDebug() << "WebView::onHotkeyChanged" << key << modifiers;
#endif

    if (!key.isEmpty() && !modifiers.isEmpty())
        m_hotkeyManager->registerHotkey(index, key, modifiers);
    else
        m_hotkeyManager->unregisterHotkey(index);

    // [TODO] 최적화 필요.
    m_hotkeyManager->writeIni();
}

void WebView::onSetupAutorunChanged(bool enable)
{
    MainWindow::instance()->enableAutorun(enable, true);
}

void WebView::onSetupTopmostChanged(bool enable)
{
    MainWindow::instance()->enableTopmost(enable, true);
}

void WebView::onOpenUrlRequested(const QUrl& url)
{
    QDesktopServices::openUrl(url);
}

void WebView::onProgramUpdateRequested()
{
    if (!m_updater)
        m_updater = new Updater(this);

    m_updater->start();
}

void WebView::onScrollTopRequested()
{
    page()->mainFrame()->setScrollPosition(QPoint(0, 0));
}
