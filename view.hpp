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


#ifndef VIEW_HPP
#define VIEW_HPP


#include <QGraphicsView>
#include <QGraphicsScene>


namespace gics
{
    class Instrument;
}

class QTimeLine;
class QGraphicsWidget;
class QGraphicsLayout;
class QGraphicsPixmapItem;
class QResizeEvent;

/**
 * \brief Main window class
 *
 * This is the main class of the application, it handles the
 * graphics scene, view and instruments.
 */
class View : public QGraphicsView
{
    Q_OBJECT

public:

    /**
     * \brief Default constructor
     *
     * \param parent Parent widget (0 by default)
     */
    View(QWidget* parent = 0);

    /**
     * \brief Set the front instrument
     *
     * \param instrument Front instrument
     */
    void setFrontInstrument(gics::Instrument* instrument);

    /**
     * \brief Get the front instrument
     *
     * \return Current front instrument
     */
    gics::Instrument* frontInstrument() const;

    /**
     * \brief Set the back instrument
     *
     * \param instrument Back instrument
     */
    void setBackInstrument(gics::Instrument* instrument);

    /**
     * \brief Get the back instrument
     *
     * \return Current back instrument
     */
    gics::Instrument* backInstrument() const;

public slots:

    /**
     * \brief Switch between the front instrument and the back instrument
     */
    void flip();

    /**
     * \brief Activate or deactivate the OpenGL renderer
     *
     * \param use True to enable the OpenGL renderer, false to disable it
     */
    void useOpenGL(bool use);

protected:

    /**
     * \brief Called when the view is resized
     *
     * \param event Event's attributes
     */
    virtual void resizeEvent(QResizeEvent* event);

    /**
     * \brief Called when the mouse moves over the view
     *
     * We override this event in order to ignore it, so that it is
     * caught by the parent window.
     *
     * \param event Event's attributes
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

private:

    /**
     * \brief Return a pixmap containing a rendererd item
     *
     * \param instrument Item to render
     *
     * \return Pixmap containing the capture
     */
    QPixmap capture(gics::Instrument* instrument);

private slots:

    /**
     * \brief Slot called whenever the timeline is updated
     *
     * \param value Current timeline's value
     */
    void handleTimelineUpdated(qreal value);

    /**
     * \brief Slot called when the execution of the timeline finishes
     */
    void handleTimelineFinished();

private:

    QGraphicsScene m_scene; ///< Graphics scene containing the instruments
    gics::Instrument* m_front; ///< Front instrument
    gics::Instrument* m_back; ///< Back instrument
    QTimeLine* m_timeline; ///< Timeline used for the flipping effect
    QGraphicsPixmapItem* m_frontCapture; ///< Capture of the front instrument
    QGraphicsPixmapItem* m_backCapture; ///< Capture of the back instrument
};

#endif // VIEW_HPP
