/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

import Qt 4.7
import "MinehuntCore" 1.0

Item {
    id: field
    property int clickx: 0
    property int clicky: 0

    property bool on: false

    width: 1000; height: 750

    Image { id: image2; source: "MinehuntCore/pics/No-Ones-Laughing-3.jpg"; anchors.fill: parent; fillMode: Image.Tile

    }


    Flipable {
        id: imode
        property int angle: 0
        width: 64;  height: 64
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        property bool flipped: false

        transform: Rotation { origin.x: 32; origin.y: 32; axis.x: 1; axis.z: 0; angle: imode.angle }

        front: Image {
            source: "MinehuntCore/pics/face-smile-big.png"
            width: 64; height: 64
        }

        back: Image {
            source: "MinehuntCore/pics/face-sad.png"
            width: 64; height: 64
        }

        states: State {
            name: "back"; when: imode.flipped
            PropertyChanges { target: imode; angle: 180 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                imode.flipped = !imode.flipped
            }
        }
    }

    Text {
        id: txtMine
        width: 800
        height: 64
        color: "#f7f40e"
        text: txtLucky
        font.family: "Arial Unicode MS"
        anchors.left: parent.left
        anchors.leftMargin: 80
        anchors.top: parent.top
        anchors.topMargin: 4
        font.pointSize: 48
        verticalAlignment: "AlignVCenter"
        MouseArea {
            id: aboutregion; anchors.fill: parent;
            onClicked: toolbar.toggle()
        }
    }

    Text {
        id: toolbar
        color: "black"
        x: 150; y: 4;
        width: 640; height: 64
        opacity: 0
        font.bold: true
        font.pointSize: 16
        text: "Copyright (C) 2010. Wenfeng CAI (xooz at live dot com).\nCopyright (C) 2010. Nokia Corporation."
        function toggle() {
            on = on==true?false:true
        }
        states : State {
            name: "shown"
            when: on
            PropertyChanges{target:toolbar; y: 700; opacity: 1}
        }
        transitions: [
            Transition {
                NumberAnimation { properties: "y,opacity"; duration: 500 }
            }
        ]
    }

    Grid {
        anchors.top: parent.top
        anchors.topMargin: 72
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 24; spacing: 1

        Repeater {
            id: repeater
            model: tiles
            delegate: Tile {}
        }
    }

    Row {
        id: gamedata
        x: 20; spacing: 20
        anchors.bottom: field.bottom; anchors.bottomMargin: 15

        Image {
            source: "MinehuntCore/pics/quit.png"
            scale: quitMouse.pressed ? 0.8 : 1.0
            smooth: quitMouse.pressed
            y: 10
            MouseArea {
                id: quitMouse
                anchors.fill: parent
                anchors.margins: -20
                onClicked: Qt.quit()
            }
        }
        Column {
            spacing: 2
            Image { source: "MinehuntCore/pics/bomb-color.png" }
            Text { anchors.horizontalCenter: parent.horizontalCenter; color: "white"; text: numMines }
        }

        Column {
            spacing: 2
            Image { source: "MinehuntCore/pics/flag-color.png" }
            Text { anchors.horizontalCenter: parent.horizontalCenter; color: "white"; text: numFlags }
        }
    }

    Image {
        anchors.bottom: field.bottom; anchors.bottomMargin: 15
        anchors.right: field.right; anchors.rightMargin: 20
        source: isPlaying ? 'MinehuntCore/pics/face-smile.png' :
        hasWon ? 'MinehuntCore/pics/face-smile-big.png': 'MinehuntCore/pics/face-sad.png'

        MouseArea { anchors.fill: parent; onPressed: reset() }
    }
    Text {
        anchors.centerIn: parent; width: parent.width - 20
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: "Minehunt will not run properly if the C++ plugin is not compiled.\n\nPlease see README."
        color: "white"; font.bold: true; font.pixelSize: 14
        visible: tiles == undefined
    }


}
