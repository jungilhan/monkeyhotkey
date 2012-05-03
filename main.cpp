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

#include <QtGui/QApplication>
#include "mainWindow.h"
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

    if (app.isRunning())
        return !app.sendMessage("");

    MainWindow w;
#ifdef Q_WS_WIN
    if (argc > 1 && QString(argv[1]) == "/autorun")
        w.close();
    else
#endif
    w.show();

    app.setActivationWindow(&w);
    return app.exec();
}
