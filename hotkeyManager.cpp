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

#include "hotkeyManager.h"
#include "hotkeyInfo.h"
#include "mainWindow.h"
#include "webView.h"
#include "settings.h"
#include "config.h"

#include <QtGui>

#ifdef Q_WS_WIN
#include <qt_windows.h>
#endif

HotkeyManager::HotkeyManager(QObject *parent)
    : QObject(parent)
{

}

HotkeyManager::~HotkeyManager()
{
    removeAll();
}

void HotkeyManager::add(const QList<QUrl>& programUrls)
{
    foreach (const QUrl &programUrl, programUrls) {
        QString path = QDir::toNativeSeparators(programUrl.path());

#ifdef Q_WS_WIN
        if (QString(path.at(0)) == "\\" || QString(path.at(0)) == "/")
            path = path.mid(1, path.length());

        // [MEMO] LNK 확장자를 lnk로 변환 처리 - 아이콘 정보를 가져오지 못하는 현상 수정
        if (QFileInfo(path).suffix() == "LNK")
            path = path.replace(path.lastIndexOf("LNK"), 3, "lnk");
#endif

        QFileInfo fileInfo(path);
        if (fileInfo.isSymLink() && fileInfo.isExecutable())
            path = fileInfo.symLinkTarget();

        QString name = fileInfo.completeBaseName();
        if (contains(path)) {
            MainWindow::instance()->webView()->sendMessage("popup",
                                                           "\'" + name + "\' " +
                                                           trUtf8("은(는) 이미 등록되어 있습니다."));

            continue;
        }

        QString iconUrl = fileInfo.isSymLink() ? iconPath(fileInfo.symLinkTarget()) : iconPath(path);
        bool isExecutable = fileInfo.isFile() && fileInfo.isExecutable() ? true : false;

        // UI 업데이트
        MainWindow::instance()->webView()->sendMessage("add", iconUrl, name);

        m_hotkeys.append(new HotkeyInfo(name, path, iconUrl, isExecutable));
        writeIni();

#if DEBUG
        qDebug() << "[HotkeyManager::add]" << m_hotkeys.length() << name << iconUrl << isExecutable;
#endif
    }
}

void HotkeyManager::remove(int index)
{
    removeIcon(index);
    unregisterHotkeyAll();
    delete m_hotkeys.takeAt(index);

    registerHotkeyAll();
    writeIni();

#if DEBUG
    qDebug() << "[HotkeyManager::remove]" << m_hotkeys.length();
#endif
}

void HotkeyManager::removeAll()
{
    unregisterHotkeyAll();

    while (!m_hotkeys.isEmpty())
         delete m_hotkeys.takeFirst();
}

int HotkeyManager::length() const
{
    return m_hotkeys.length();
}

QString HotkeyManager::name(int index) const
{
    return m_hotkeys.at(index)->name();
}

QString HotkeyManager::path(int index) const
{
    return m_hotkeys.at(index)->path();
}

bool HotkeyManager::isExecutable(int index) const
{
    return m_hotkeys.at(index)->isExecutable();
}

QString HotkeyManager::iconPath(int index) const
{
    return m_hotkeys.at(index)->iconPath();
}

QString HotkeyManager::key(int index) const
{
    return m_hotkeys.at(index)->key();
}

void HotkeyManager::setKey(int index, const QString& key)
{
    m_hotkeys.at(index)->setKey(key);
}

QStringList HotkeyManager::modifiers(int index) const
{
    return m_hotkeys.at(index)->modifiers();
}

void HotkeyManager::setModifiers(int index, const QStringList& modifiers)
{
    m_hotkeys.at(index)->setModifiers(modifiers);
}

QString HotkeyManager::hotkey(int index)
{
    QString hotkey;
    QStringList modifiers = m_hotkeys.at(index)->modifiers();

    foreach (const QString& modifier, modifiers)
        hotkey += modifier + " + ";

    if (key(index).length() > 0)
        hotkey += key(index);
    else
        hotkey = hotkey.mid(0, hotkey.length() - QString(" + ").length());

#if DEBUG
    qDebug() << "[HotkeyManager::hotkey]" << hotkey;
#endif
    return hotkey;
}

QString HotkeyManager::iconPath(const QFileInfo& fileInfo)
{
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(fileInfo);

    QString dataPath = QDir::toNativeSeparators(APPLICATION_DIR_PATH + "/ui/icons/");
    QDir dir(dataPath);
    if (!dir.exists())
        dir.mkpath(dataPath);

    // [MEMO] 동일한 이름의 아이콘 저장 방지를 위해 경로를 이용한 시그니처 생성
    QString signature = fileInfo.path();
    signature.replace(QRegExp("[^a-zA-Z0-9_]"), "");

    QPixmap grayscalePixmap(icon.pixmap(48, 48, QIcon::Disabled));
    QString iconName = signature + fileInfo.fileName() + ".png.gray";
    grayscalePixmap.save(dataPath + iconName, "PNG");

    QPixmap pixmap(icon.pixmap(48, 48));
    iconName = signature + fileInfo.fileName() + ".png";
    pixmap.save(dataPath + iconName, "PNG");

    return "icons/" + iconName;
}

void HotkeyManager::removeIcon(int index)
{
    QFile::remove(QDir::toNativeSeparators("ui/") + m_hotkeys.at(index)->iconPath());
    QFile::remove(QDir::toNativeSeparators("ui/") + m_hotkeys.at(index)->iconPath() + ".gray");
}

void HotkeyManager::writeIni()
{
    Settings settings;
    settings.removeGroup("Items");
    settings.beginWriteArray("Items");

    for (int index = 0; index < m_hotkeys.length(); index++) {
        settings.setArrayIndex(index);
        settings.setValue("Name", m_hotkeys.at(index)->name());
        settings.setValue("Path", m_hotkeys.at(index)->path());
        settings.setValue("Icon", m_hotkeys.at(index)->iconPath());
        settings.setValue("Executable", m_hotkeys.at(index)->isExecutable());
        settings.setValue("Key", m_hotkeys.at(index)->key());
        settings.setValue("Modifiers", m_hotkeys.at(index)->modifiers());
    }

    settings.endArray();
}

void HotkeyManager::restore()
{
    readIni();
    registerHotkeyAll();

    emit restoreFinished(this);
}

void HotkeyManager::readIni()
{
    removeAll();

    Settings settings;
    int size = settings.beginReadArray("Items");
    for (int index = 0; index < size; index++) {
        settings.setArrayIndex(index);
        QString name = settings.value("Name").toString();
        QString path = settings.value("Path").toString();
        QString iconPath = settings.value("Icon").toString();
        QString key = settings.value("Key", "").toString();
        QStringList modifiers = settings.value("Modifiers", "").toStringList();
        bool isExecutable = settings.value("Executable").toBool();

        m_hotkeys.append(new HotkeyInfo(name, path, iconPath, isExecutable, key, modifiers));

#if DEBUG
        qDebug() << "[HotkeyManager::readIni]" << name << path << iconPath << key << modifiers << isExecutable;
#endif
    }
}

int HotkeyManager::keyCodeFromString(const QString& key)
{
    int code = 0;

#ifdef Q_WS_WIN
    if (key == "0") code = 0x30;
    else if (key == "1") code = 0x31;
    else if (key == "2") code = 0x32;
    else if (key == "3") code = 0x33;
    else if (key == "4") code = 0x34;
    else if (key == "5") code = 0x35;
    else if (key == "6") code = 0x36;
    else if (key == "7") code = 0x37;
    else if (key == "8") code = 0x38;
    else if (key == "9") code = 0x39;
    else if (key == "A") code = 0x41;
    else if (key == "B") code = 0x42;
    else if (key == "C") code = 0x43;
    else if (key == "D") code = 0x44;
    else if (key == "E") code = 0x45;
    else if (key == "F") code = 0x46;
    else if (key == "G") code = 0x47;
    else if (key == "H") code = 0x48;
    else if (key == "I") code = 0x49;
    else if (key == "J") code = 0x4A;
    else if (key == "K") code = 0x4B;
    else if (key == "L") code = 0x4C;
    else if (key == "M") code = 0x4D;
    else if (key == "N") code = 0x4E;
    else if (key == "O") code = 0x4F;
    else if (key == "P") code = 0x50;
    else if (key == "Q") code = 0x51;
    else if (key == "R") code = 0x52;
    else if (key == "S") code = 0x53;
    else if (key == "T") code = 0x54;
    else if (key == "U") code = 0x55;
    else if (key == "V") code = 0x56;
    else if (key == "W") code = 0x57;
    else if (key == "X") code = 0x58;
    else if (key == "Y") code = 0x59;
    else if (key == "Z") code = 0x5A;
    else if (key == "F1") code = VK_F1;
    else if (key == "F2") code = VK_F2;
    else if (key == "F3") code = VK_F3;
    else if (key == "F4") code = VK_F4;
    else if (key == "F5") code = VK_F5;
    else if (key == "F6") code = VK_F6;
    else if (key == "F7") code = VK_F7;
    else if (key == "F8") code = VK_F8;
    else if (key == "F9") code = VK_F9;
    else if (key == "F10") code = VK_F10;
    else if (key == "F11") code = VK_F11;
    else if (key == "F12") code = VK_F12;
#endif

    return code;
}

int HotkeyManager::modifierCodeFromString(const QString& key)
{
    int code = 0;

#ifdef Q_WS_WIN
    if (key == "Ctrl")
       code = MOD_CONTROL;
    else if (key == "Alt")
       code = MOD_ALT;
    else if (key == "Shift")
       code = MOD_SHIFT;
#endif

    return code;
}

bool HotkeyManager::contains(const QString& path)
{
    for (int index = 0; index < m_hotkeys.length(); index++) {
        if (m_hotkeys.at(index)->path() == path)
            return true;
    }

    return false;
}

void HotkeyManager::registerHotkey(int index, const QString& key, const QStringList& modifiers)
{
    Q_ASSERT(!key.isEmpty());
    Q_ASSERT(!modifiers.isEmpty());

    int modifiersCode = 0;

    foreach (const QString &modifier, modifiers)
        modifiersCode |= modifierCodeFromString(modifier);

    if (m_hotkeys.at(index)->isRegistered()) {
#if DEBUG
        qDebug() << "Hotkey has already been registered: " << hotkey(index);
#endif
#ifdef Q_WS_WIN
        unregisterHotkey(index);
#endif
    }

#ifdef Q_WS_WIN
    int keyCode = keyCodeFromString(key);
    bool result = RegisterHotKey(MainWindow::instance()->winId(),
                                 index,
                                 modifiersCode,
                                 keyCode);

    // [Issue3] 숫자 등록 시 숫자 키패드(키보드 오른쪽에 위치한)도 함께 등록.
    if (keyCode >= 0x30 && keyCode <= 0x39) {
        int numpadKeyCode = keyCode + 0x30;

        RegisterHotKey(MainWindow::instance()->winId(),
                       index + NUMPAD_START_ID, // 기존 숫자 키의 ID와 중복되지 않는 ID 생성.
                       modifiersCode,
                       numpadKeyCode);
    }

#if DEBUG
    qDebug() << result << "[HotkeyManager::registerHotkey]" << index << key << modifiers;
#endif

    if (!result) {
        MainWindow::instance()->webView()->sendMessage("popup",
                                                       "\'" + name(index) + "\'" + trUtf8("이(가) 등록한 ") +
                                                       "\'" + hotkey(index) + "\' " +
                                                       trUtf8("은(는) 이미 다른 곳에서 사용 중입니다."));
    }
#endif

    m_hotkeys.at(index)->setRegistered(true);
}

void HotkeyManager::unregisterHotkey(int index)
{
#ifdef Q_WS_WIN
    UnregisterHotKey(MainWindow::instance()->winId(), index);

    // [Issue3] 숫자가 포함된 핫키 해제시 숫자 키패드(키보드 오른쪽에 위치한)도 해제
    int keyCode = keyCodeFromString(m_hotkeys.at(index)->key());
    if (keyCode >= 0x30 && keyCode <= 0x39)
        UnregisterHotKey(MainWindow::instance()->winId(), index + NUMPAD_START_ID);
#endif

    m_hotkeys.at(index)->setRegistered(false);

#if DEBUG
    qDebug() << "[HotkeyManager::unregisterHotkey]" << index;
#endif
}

void HotkeyManager::registerHotkeyAll()
{
    for (int index = 0; index < m_hotkeys.length(); index++) {
        QString key = m_hotkeys.at(index)->key();
        QStringList modifiers = m_hotkeys.at(index)->modifiers();

        if (!key.isEmpty() && !modifiers.isEmpty())
            registerHotkey(index, key, modifiers);
    }
}

void HotkeyManager::unregisterHotkeyAll()
{
    for (int index = 0; index < m_hotkeys.length(); index++)
        unregisterHotkey(index);
}
