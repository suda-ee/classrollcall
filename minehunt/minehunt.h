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


#include <qdeclarative.h>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <Wincrypt.h>
#endif

class TileData : public QObject
{
    Q_OBJECT
public:
    TileData() : _hasFlag(false), _hasMine(false), _hint(-1), _flipped(false) {}

    Q_PROPERTY(bool hasFlag READ hasFlag WRITE setHasFlag NOTIFY hasFlagChanged)
    bool hasFlag() const { return _hasFlag; }

    Q_PROPERTY(bool hasMine READ hasMine NOTIFY hasMineChanged)
    bool hasMine() const { return _hasMine; }

    Q_PROPERTY(int hint READ hint NOTIFY hintChanged)
    int hint() const { return _hint; }

    Q_PROPERTY(bool flipped READ flipped NOTIFY flippedChanged())
    bool flipped() const { return _flipped; }

    void setHasFlag(bool flag) {if(flag==_hasFlag) return; _hasFlag = flag; emit hasFlagChanged();}
    void setHasMine(bool mine) {if(mine==_hasMine) return; _hasMine = mine; emit hasMineChanged();}
    void setHint(int hint) { if(hint == _hint) return; _hint = hint; emit hintChanged(); }
    void flip() { if (_flipped) return; _flipped = true; emit flippedChanged(); }
    void unflip() { if(!_flipped) return; _flipped = false; emit flippedChanged(); }

signals:
    void flippedChanged();
    void hasFlagChanged();
    void hintChanged();
    void hasMineChanged();

private:
    bool _hasFlag;
    bool _hasMine;
    int _hint;
    bool _flipped;
};

class MinehuntGame : public QObject
{
    Q_OBJECT
public:
    MinehuntGame();
    ~MinehuntGame();

    Q_PROPERTY(QDeclarativeListProperty<TileData> tiles READ tiles CONSTANT)
    QDeclarativeListProperty<TileData> tiles();

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    bool isPlaying() {return playing;}

    Q_PROPERTY(bool hasWon READ hasWon NOTIFY hasWonChanged)
    bool hasWon() {return won;}

    Q_PROPERTY(int numMines READ numMines NOTIFY numMinesChanged)
    int numMines() const{return m_seqIdSample.size();}

    Q_PROPERTY(int numFlags READ numFlags NOTIFY numFlagsChanged)
    int numFlags() const{return nFlags;}

    Q_PROPERTY(QString txtLucky READ txtLucky NOTIFY txtLuckyChanged)
    QString txtLucky() const{return m_txtLucky;}

public slots:
    Q_INVOKABLE bool flip(int row, int col);
    Q_INVOKABLE bool flag(int row, int col);
    void setBoard();
    void reset();

signals:
    void isPlayingChanged();
    void hasWonChanged();
    void numMinesChanged();
    void numFlagsChanged();
    void txtLuckyChanged();

private:
    bool onBoard( int r, int c ) const { return r >= 0 && r < numRows && c >= 0 && c < numCols; }
    TileData *tile( int row, int col ) { return onBoard(row, col) ? _tiles[col+numCols*row] : 0; }
    int getHint(int row, int col);
    void setPlaying(bool b){if(b==playing) return; playing=b; emit isPlayingChanged();}

    void parserClassRoom();
    void popOne();

    QList<TileData *> _tiles;
    int numCols;
    int numRows;
    bool playing;
    bool won;
    int remaining;
    int nMines;
    int nFlags;

    QList<int> m_seqIdPool;  ///< @brief the set of seq ids
    QList<int> m_seqIdSample;  ///< @brief the sample set of seq ids
    QString m_classname;
    QList<QString> m_sids;
    QList<QString> m_names;
    QString m_txtLucky;
#ifdef Q_OS_WIN32
    //--------------------------------------------------------------------
    // Declare and initialize variables.

    HCRYPTPROV   hCryptProv;
#endif
};
