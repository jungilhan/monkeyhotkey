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

#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>

class NativeAdapter;
class HotkeyManager;
class Updater;

class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit WebView(QWidget* parent = 0);
    ~WebView();

public:
    void sendMessage(const QString& message, const QString& arg1 = QString(),
                     const QString& arg2 = QString(), const QString& arg3 = QString());

    HotkeyManager* hotkeyManager() {
        return m_hotkeyManager;
    }

protected:
    virtual void dropEvent(QDropEvent* event);

private slots:
    void onLoadStarted();
    void onExecuteRequested(int index);
    void onRestoreRequested();
    void onRestoreFinished(HotkeyManager* hotkeyManager);
    void onRemovedRequested(const QStringList& indexList);
    void onHotkeyChanged(int index, const QString& key, const QStringList& modifiers);
    void onOpenUrlRequested(const QUrl& url);
    void onProgramUpdateRequested();
    void onScrollTopRequested();
    void onSetupAutorunChanged(bool enable);
    void onSetupTopmostChanged(bool enable);

private:
    void init();

private:
    NativeAdapter* m_nativeAdapter;
    HotkeyManager* m_hotkeyManager;
    Updater* m_updater;
};

#endif // WEBVIEW_H
