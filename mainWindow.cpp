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

#include "mainWindow.h"
#include "config.h"
#include "header.h"
#include "footer.h"
#include "webView.h"
#include "settings.h"
#include "aboutDialog.h"
#include "hotkeyManager.h"
#include "updater.h"

#include <QtGui>
#include <QtWebKit>

#ifdef Q_WS_WIN
#include <qt_windows.h>
#endif

MainWindow::MainWindow(QWidget* parent)
    : QDialog(parent)
    , m_header(0)
    , m_footer(0)
    , m_splash(0)
    , m_webView(0)
    , m_layout(0)
    , m_trayMenu(0)
    , m_systemTrayIcon(0)
    , m_aboutDialg(0)
    , m_updater(0)

{
    init();
}

MainWindow::~MainWindow()
{
#if DEBUG
    qDebug() << "~MainWindow()";
#endif

    delete m_header;
    delete m_footer;
    delete m_splash;
    delete m_webView;
    delete m_layout;
    delete m_trayMenu;
    delete m_systemTrayIcon;
    delete m_aboutDialg;
    delete m_updater;
}

MainWindow* MainWindow::instance()
{
    static QWidget* mainWindow = 0;

    if (!mainWindow) {
        foreach (QWidget* widget, QApplication::allWidgets()) {
            if (widget && widget->objectName() == "MainWindow") {
                mainWindow = widget;
                break;
            }
        }
    }

    Q_ASSERT(mainWindow);
    return static_cast<MainWindow*>(mainWindow);
}

void MainWindow::init()
{
#ifdef Q_WS_WIN
    if (QApplication::argc() > 1 && QApplication::arguments().at(1) == "/autorun")
        showMinimized();
#endif

    setObjectName("MainWindow");
    setWindowTitle(trUtf8("몽키 핫키 2"));
    setWindowIcon(QIcon(":logo32x32"));
    setWindowFlags(Qt::CustomizeWindowHint);
    resize(FRAME_WIDTH, FRAME_HEIGHT);
    setMinimumSize(FRAME_WIDTH, FRAME_HEIGHT);

    QApplication::setApplicationName("Monkey Hotkey");
    QApplication::setOrganizationName("Monkey Labs");
    QApplication::setApplicationVersion(VERSION_STR);

#ifdef Q_WS_WIN
    initNativeWindowStyle();
#endif

    m_splash = new QWebView(this);
    m_splash->load(QUrl("file:///" + APPLICATION_DIR_PATH + "/ui/splash.html"));
    m_splash->show();

    readSettings();

    m_trayMenu = new QMenu();
    m_trayMenu->addAction(QIcon(":tray_info16x16"), trUtf8("몽키 핫키에 대해서"), this, SLOT(onAboutClicked()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(QIcon(":tray_home16x16"), trUtf8("프로젝트 홈페이지로 이동하기"), this, SLOT(onGoWebPage()));
    m_trayMenu->addAction(trUtf8("코드 저장소로 이동하기"), this, SLOT(onGoRepositoryPage()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(trUtf8("열기"), this, SLOT(onShow()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(QIcon(":tray_shutdown16x16"), trUtf8("끝내기"), this, SLOT(onTrayQuitClicked()));

    m_systemTrayIcon = new QSystemTrayIcon(QIcon(":about18x18"), this);
    m_systemTrayIcon->setToolTip(trUtf8("몽키 핫키 2"));
    m_systemTrayIcon->setContextMenu(m_trayMenu);
    m_systemTrayIcon->show();
    m_systemTrayIcon->connect(m_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    m_header = new Header(this);
    m_header->setMinimumSize(FRAME_WIDTH - 4, HEADER_HEIGHT);
    m_header->setMaximumHeight(HEADER_HEIGHT);
    m_header->connect(m_header, SIGNAL(aboutClicked()), this, SLOT(onAboutClicked()));
    m_header->connect(m_header, SIGNAL(closeClicked()), this, SLOT(onCloseClicked()));

    m_footer = new Footer(this);
    m_footer->setMinimumSize(FRAME_WIDTH, FOOTER_HEIGHT);
    m_footer->setMaximumHeight(FOOTER_HEIGHT);
    m_footer->hide();
    m_footer->connect(m_footer, SIGNAL(deleteClicked()), this, SLOT(onDeleteClicked()));
    m_footer->connect(m_footer, SIGNAL(setupClicked()), this, SLOT(onSetupClicked()));
    m_footer->connect(m_footer, SIGNAL(infoClicked()), this, SLOT(onInfoClicked()));

    m_webView = new WebView(this);
    m_webView->load(QUrl("file:///" + APPLICATION_DIR_PATH + "/ui/index.html"));
    m_webView->hide();
    m_webView->connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(this, SIGNAL(hotkeyPressed(int)), m_webView, SLOT(onExecuteRequested(int)));

    m_layout = new QVBoxLayout();
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_header);
    m_layout->addWidget(m_webView);
    m_layout->addWidget(m_splash);
    m_layout->addWidget(m_footer);
    setLayout(m_layout);
}

#ifdef Q_WS_WIN
void MainWindow::initNativeWindowStyle()
{
    // [MEMO] Qt의 Qt::FramelessWindowHint 및 Qt::CustomizeWindowHint 스타일을 사용하면
    //        윈도우즈 Task바의 시스템 컨텍스트 메뉴가 나오지 않으므로 Win API로 직접 설정.
    DWORD flags = GetWindowLong(winId(), GWL_STYLE);
    flags |= (WS_SYSMENU | WS_MINIMIZEBOX);
    SetWindowLong(winId(), GWL_STYLE, flags);
}
#endif

WebView* MainWindow::webView()
{
    return m_webView;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        m_webView->sendMessage("goHome");
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    showMinimized();
    hide();
    writeSettings();
}

void MainWindow::onLoadFinished(bool ok)
{
    QTimer::singleShot(1500, this, SLOT(onSingleShotTimer()));
}

void MainWindow::onSingleShotTimer()
{
    QPropertyAnimation* animation = new QPropertyAnimation(m_splash, "pos", this);
    animation->setDuration(300);
    animation->setStartValue(QPoint(m_splash->pos()));
    animation->setEndValue(QPoint(m_splash->pos().x(), m_splash->pos().y() + m_splash->height()));
    animation->connect(animation, SIGNAL(finished()), this, SLOT(onAnimationFinished()));
    animation->start();

    layout()->removeWidget(m_splash);
    m_webView->lower();
    m_webView->show();
    m_footer->lower();
    m_footer->show();
}

void MainWindow::onAnimationFinished()
{
    m_splash->hide();

    if (!m_updater) {
        m_updater = new Updater(this);
        m_updater->connect(m_updater, SIGNAL(updateAvailable(QString)), this, SLOT(onUpdateAvailable(QString)));
    }

    m_updater->start(true);
}

void MainWindow::onAboutClicked()
{
    if (!m_aboutDialg)
        m_aboutDialg = new AboutDialog(this);

    if (m_aboutDialg->isVisible())
        m_aboutDialg->activateWindow();
    else
        m_aboutDialg->show();
}

void MainWindow::onCloseClicked()
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::Close));
}

void MainWindow::onDeleteClicked()
{
    m_webView->sendMessage("showDeleteActivity");
}

void MainWindow::onSetupClicked()
{
    m_webView->sendMessage("showSetupActivity");
}

void MainWindow::onInfoClicked()
{
    m_webView->sendMessage("showInfoActivity");
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        onShow();
    }
}

void MainWindow::onGoWebPage()
{
    QDesktopServices::openUrl(QUrl("http://jungilhan.github.com/monkeyhotkey/index.html"));
}

void MainWindow::onGoRepositoryPage()
{
    QDesktopServices::openUrl(QUrl("http://code.google.com/p/monkeyhotkey/"));
}

void MainWindow::onShow()
{
    if (isMinimized())
        showNormal();

    activateWindow();
}

void MainWindow::onTrayQuitClicked()
{
    writeSettings();
    qApp->quit();
}

void MainWindow::onUpdateAvailable(const QString& version)
{
    m_webView->sendMessage("popup", trUtf8("몽키 핫키의 ") + "'" + version + "'" + trUtf8(" 버전이 사용 가능합니다.") + trUtf8(" '정보' 탭에서 업데이트하세요."));

#if DEBUG
    qDebug() << trUtf8("몽키 핫키의") << "'" << version << "'" << trUtf8(" 버전이 사용 가능합니다.");
#endif
}

void MainWindow::readSettings()
{
    Settings settings;
    settings.beginGroup("MainWindow");

    if (settings.contains("Size"))
        resize(settings.value("Size").toSize());
    if (settings.contains("Position"))
        move(settings.value("Position").toPoint());
    if (isTopmostEnabled())
        enableTopmost(true);

    settings.endGroup();
}

void MainWindow::writeSettings()
{
    Settings settings;
    settings.beginGroup("MainWindow");

    settings.setValue("Size", size());
    settings.setValue("Position", pos());

    settings.endGroup();
}

bool MainWindow::isAutorunEnabled()
{
    Settings settings;
    settings.beginGroup("MainWindow");
    bool enable = settings.value("Autorun", false).toBool();
    settings.endGroup();

    return enable;
}

bool MainWindow::isTopmostEnabled()
{
    Settings settings;
    settings.beginGroup("MainWindow");
    bool enable = settings.value("Topmost", false).toBool();
    settings.endGroup();

    return enable;
}

void MainWindow::enableAutorun(bool on, bool saveIni)
{
#ifdef Q_WS_WIN
    QSettings registry("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (on) {
        QString value = "\"" + QDir::toNativeSeparators(QCoreApplication::applicationFilePath()) + "\"";
        value += " /autorun";
        registry.setValue("monkeyhotkey", value);
    } else {
        registry.remove("monkeyhotkey");
    }
#endif

    if (saveIni) {
        Settings settings;
        settings.beginGroup("MainWindow");
        settings.setValue("Autorun", on);
        settings.endGroup();
    }
}

void MainWindow::enableTopmost(bool on, bool saveIni)
{
    Qt::WindowFlags flags = windowFlags();
    if (on)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;

#ifdef Q_WS_WIN
    // [MEMO] Windows에서는 setWindowFlags 설정 시 winId가 업데이트 됨.
    //        따라서 모든 핫키를 삭제 후 재등록하는 과정이 필요.
    if (m_webView)
        m_webView->hotkeyManager()->unregisterHotkeyAll();
#endif

    setWindowFlags(flags);

#ifdef Q_WS_WIN
    if (m_webView)
        m_webView->hotkeyManager()->registerHotkeyAll();

    initNativeWindowStyle();
#endif

    if (!isVisible())
        show();

    if (saveIni) {
        Settings settings;
        settings.beginGroup("MainWindow");
        settings.setValue("Topmost", on);
        settings.endGroup();
    }    
}

#ifdef Q_WS_WIN
bool MainWindow::winEvent(MSG* message, long* result)
{
    switch (message->message) {
    case WM_HOTKEY:
#if DEBUG
        qDebug() << "WM_HOTKEY" << message->wParam;
#endif
        if (message->wParam >= NUMPAD_START_ID)
            emit hotkeyPressed(message->wParam - NUMPAD_START_ID);
        else
            emit hotkeyPressed(message->wParam);
        break;
    case WM_CLOSE:
        // [MEMO] Alt + F4에 대한 처리 재정의.
        QCoreApplication::postEvent(this, new QEvent(QEvent::Close));
        return true;
    }

    return QDialog::winEvent(message, result);
}
#endif
