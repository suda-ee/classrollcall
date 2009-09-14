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


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <QMainWindow>
#include <gics/textlabel.hpp>


class View;
class InputPanel;
class OutputPanel;

/**
 * \brief Main window class
 *
 * This is the main class of the application, it handles the
 * graphics scene, view and instruments.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * \brief Default constructor
     *
     * \param parent Parent widget (0 by default)
     * \param flags Window flags (0 by default)
     */
    MainWindow(QWidget* parent = 0, Qt::WFlags flags = 0);

private:

    void parserClassRoom();

public slots:

    void refreshhot(double value);
    void popOne();

protected:

    /**
     * \brief Called when a mouse button is pressed over the window
     *
     * \param event Event's attributes
     */
    virtual void mousePressEvent(QMouseEvent* event);

    /**
     * \brief Called when the mouse moves over the window
     *
     * \param event Event's attributes
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

    /**
     * \brief Called when a key is pressed
     *
     * \param event Event's attributes
     */
    virtual void keyPressEvent(QKeyEvent* event);

private:

    gics::TextLabel* title;
    View* m_view; ///< Graphics view showing the scene
    QPoint m_dragPosition; ///< Last cursor position when dragging the window
    InputPanel* m_input; ///< Input panel
    OutputPanel* m_output; ///< Output panel

    int m_theone;
    QString m_classname;
    QList<QString> m_sids;
    QList<QString> m_names;
};

#endif // MAINWINDOW_HPP
