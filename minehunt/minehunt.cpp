/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <stdlib.h>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QDomDocument>
#include <boost/numeric/conversion/converter.hpp>

#include "minehunt.h"

void tilesPropAppend(QDeclarativeListProperty<TileData>* prop, TileData* value)
{
    Q_UNUSED(prop);
    Q_UNUSED(value);
    return; //Append not supported
}

int tilesPropCount(QDeclarativeListProperty<TileData>* prop)
{
    return static_cast<QList<TileData*>*>(prop->data)->count();
}

TileData* tilesPropAt(QDeclarativeListProperty<TileData>* prop, int index)
{
    return static_cast<QList<TileData*>*>(prop->data)->at(index);
}

QDeclarativeListProperty<TileData> MinehuntGame::tiles(){
    return QDeclarativeListProperty<TileData>(this, &_tiles, &tilesPropAppend,
            &tilesPropCount, &tilesPropAt, 0);
}

MinehuntGame::MinehuntGame()
: numCols(24), numRows(15), playing(true), won(false)
{
    setObjectName("mainObject");
    srand(QTime(0,0,0).secsTo(QTime::currentTime()));

    qsrand(QDateTime::currentDateTime().toTime_t());

#ifdef Q_OS_WIN32
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
#endif

    parserClassRoom();

    //initialize array
    for(int ii = 0; ii < numRows * numCols; ++ii) {
        _tiles << new TileData;
    }
    reset();
}

MinehuntGame::~MinehuntGame()
{
#ifdef Q_OS_WIN32
    CryptReleaseContext(hCryptProv, 0);
#endif
}

void MinehuntGame::setBoard()
{
    foreach(TileData* t, _tiles){
        t->setHasMine(false);
        t->setHint(-1);
    }
    //place mines
    int mines = nMines;
    remaining = numRows*numCols-mines;
    while ( mines ) {
        int col = int((double(rand()) / double(RAND_MAX)) * numCols);
        int row = int((double(rand()) / double(RAND_MAX)) * numRows);

        TileData* t = tile( row, col );

        if (t && !t->hasMine()) {
            t->setHasMine( true );
            mines--;
        }
    }

    //set hints
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++) {
            TileData* t = tile(r, c);
            if (t && !t->hasMine()) {
                int hint = getHint(r,c);
                t->setHint(hint);
            }
        }

    setPlaying(true);
}

void MinehuntGame::reset()
{
    foreach(TileData* t, _tiles){
        t->unflip();
        t->setHasFlag(false);
    }
    nMines = m_sids.size();
    nFlags = 0;
    emit numFlagsChanged();
    setPlaying(false);
    QTimer::singleShot(600,this, SLOT(setBoard()));

    qDebug()<<"Rest Pool: "<<m_seqIdPool;
    qDebug()<<"Current Sample: "<<m_seqIdSample;
    m_seqIdPool.append(m_seqIdSample);
    m_seqIdSample.clear();
    emit numMinesChanged();
}

int MinehuntGame::getHint(int row, int col)
{
    int hint = 0;
    for (int c = col-1; c <= col+1; c++)
        for (int r = row-1; r <= row+1; r++) {
            TileData* t = tile(r, c);
            if (t && t->hasMine())
                hint++;
        }
    return hint;
}

bool MinehuntGame::flip(int row, int col)
{
    if(!playing)
        return false;

    TileData *t = tile(row, col);
    if (!t || t->hasFlag())
        return false;

    if(t->flipped()){
        int flags = 0;
        for (int c = col-1; c <= col+1; c++)
            for (int r = row-1; r <= row+1; r++) {
                TileData *nearT = tile(r, c);
                if(!nearT || nearT == t)
                    continue;
                if(nearT->hasFlag())
                    flags++;
                if(nearT->flipped() && nearT->hasMine())
                    flags++;
            }
        if(!t->hint() || t->hint() != flags)
            return false;
        for (int c = col-1; c <= col+1; c++)
            for (int r = row-1; r <= row+1; r++) {
                TileData *nearT = tile(r, c);
                if (nearT && !nearT->flipped() && !nearT->hasFlag()) {
                    flip( r, c );
                }
            }
        return true;
    }

    t->flip();

    if (t->hint() == 0) {
        for (int c = col-1; c <= col+1; c++)
            for (int r = row-1; r <= row+1; r++) {
                TileData* t = tile(r, c);
                if (t && !t->flipped()) {
                    flip( r, c );
                }
            }
    }

    if(t->hasMine()){
        popOne();
        return true;
    }

    remaining--;
    if(!remaining){
        won = true;
        hasWonChanged();
        // setPlaying(false);
    }
    return true;
}

bool MinehuntGame::flag(int row, int col)
{
    TileData *t = tile(row, col);
    if(!t)
        return false;

    t->setHasFlag(!t->hasFlag());
    nFlags += (t->hasFlag()?1:-1);
    emit numFlagsChanged();
    return true;
}

void MinehuntGame::parserClassRoom()
{
    QString classfilename = QFileDialog::getOpenFileName(NULL,
	    tr("ѡ��༶�ļ�"), QString(), "XML files (*.xml)");
    if (classfilename.isNull())
	classfilename = "SampleClassroom.xml";
    QFile* classfile = new QFile(classfilename, this);
    QDomDocument classdocument;
    if (classdocument.setContent(classfile))
    {
	QDomElement theelem = classdocument.documentElement();
	m_classname = theelem.attribute("name");
        QDomNodeList memlistelm = classdocument.elementsByTagName("member");
	int i;
        for (i = 0; i < memlistelm.count(); i++)
        {
	    theelem = memlistelm.item(i).toElement();
	    m_sids.append(theelem.attribute("id"));
	    m_names.append(theelem.text());
            m_seqIdPool.append(i);
        }
    }
    delete classfile;
}

void MinehuntGame::popOne()
{
    using namespace boost::numeric;
    typedef converter<int,double,conversion_traits<int,double>,
            def_overflow_handler, RoundEven<double>> HZround;
#ifdef Q_OS_WIN32
    //--------------------------------------------------------------------
    // Declare and initialize variables.

    BYTE         pbData[2];
    
    CryptGenRandom(hCryptProv, 2, pbData);
    quint16 randint = *((quint16 *) pbData) & RAND_MAX;
    int theone;
    theone = HZround::convert((m_seqIdPool.size() - 1.) * randint / RAND_MAX);
#else
    theone = HZround::convert((m_seqIdPool.size() - 1.) * qrand() / RAND_MAX);
#endif
    if (theone < m_seqIdPool.size())
    {
	qDebug()<<"Lucky: "<<m_sids.at(m_seqIdPool.at(theone))
            <<m_names.at(m_seqIdPool.at(theone));
    }
    if (m_seqIdPool.size() > 0)
    {
	m_seqIdSample.append(m_seqIdPool.at(theone));
	m_seqIdPool.removeAt(theone);
        emit numMinesChanged();
    }
}
