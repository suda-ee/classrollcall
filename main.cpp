/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the GICS library.
**
** Commercial Usage
** Licensees holding valid GICS Commercial licenses may use this file in
** accordance with the GICS Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
**
****************************************************************************/

/****************************************************************************
**
** Copyright (C) 2009 caiwenfeng at suda dot edu dot cn.
**
****************************************************************************/


#include <QApplication>
#include "mainwindow.hpp"
#include <QTextCodec>
//#include <QtPlugin>
#include <QPluginLoader>

//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Force the "raster" graphics system on Linux to get best performances
    #ifdef Q_OS_LINUX
        QApplication::setGraphicsSystem("raster");
    #endif

    //Q_IMPORT_PLUGIN(qcncodecs)
    //QPluginLoader cncodec(qApp->applicationDirPath() + "plugins/codecs/qcncodecs4.dll");
    //cncodec.load();
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

    QApplication application(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow window;
    window.showFullScreen();
    return application.exec();
}
