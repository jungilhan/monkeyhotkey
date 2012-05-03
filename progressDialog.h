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

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

class progressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit progressDialog(QWidget *parent = 0);
    ~progressDialog();

public:
    void setLabelText(const QString& text);
    void setValue(int value);
    void setAutoClose(int msec);

protected:
    virtual void hideEvent(QHideEvent* event);

private:
    void init();
    void startAutoCloseTimer();

private slots:
    void onTimer();

private:
    QLabel* m_contentsLabel;
    QProgressBar* m_progressBar;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    QTimer* m_timer;
    int m_autoCloseMsec;
    int m_currentMsec;
};

#endif // PROGRESSDIALOG_H
