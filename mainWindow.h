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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QDialog>
#include <QtGui/QSystemTrayIcon>

class Header;
class Footer;
class WebView;
class QWebView;
class QVBoxLayout;
class QMenu;
class AboutDialog;
class Updater;

class MainWindow : public QDialog
{
    Q_OBJECT
    
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

signals:
    void hotkeyPressed(int index);

public:
    static MainWindow* instance();

public:
    WebView* webView();

public:
    bool isAutorunEnabled();
    void enableAutorun(bool on, bool saveIni = false);

    bool isTopmostEnabled();
    void enableTopmost(bool on, bool saveIni = false);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void closeEvent(QCloseEvent* event);

#ifdef Q_WS_WIN
    bool winEvent(MSG* message, long* result);
#endif

private slots:
    void onLoadFinished(bool ok);
    void onSingleShotTimer();
    void onAnimationFinished();
    void onAboutClicked();
    void onCloseClicked();
    void onDeleteClicked();
    void onSetupClicked();
    void onInfoClicked();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onGoWebPage();
    void onGoRepositoryPage();
    void onShow();
    void onTrayQuitClicked();
    void onUpdateAvailable(const QString& version);

private:
    void init();
    void readSettings();
    void writeSettings();

#ifdef Q_WS_WIN
    void initNativeWindowStyle();
#endif

private:
    Header* m_header;
    Footer* m_footer;
    QWebView* m_splash;
    WebView* m_webView;
    QVBoxLayout* m_layout;
    QMenu* m_trayMenu;
    QSystemTrayIcon* m_systemTrayIcon;
    AboutDialog* m_aboutDialg;
    Updater* m_updater;
};

#endif // MAINWINDOW_H
