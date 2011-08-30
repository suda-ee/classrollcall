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


#include "view.hpp"
#include <gics/picture.hpp>
#include <gics/instrument.hpp>
#include <gics/math.hpp>
#include <QGraphicsView>
#include <QGraphicsLayout>
#include <QMouseEvent>
#include <QGLWidget>
#include <QTransform>
#include <QTimeLine>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


//-------------------------------------------------------------------------------------------------
View::View(QWidget* parent)
    : QGraphicsView(parent)
    , m_front(0)
    , m_back(0)
{
    // View setup
    setStyleSheet("background: transparent");
    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::HighQualityAntialiasing);
    setCacheMode(CacheBackground);
    setInteractive(true);

    // Scene setup
    setScene(&m_scene);

    // Create the capture items
    m_frontCapture = new QGraphicsPixmapItem;
    m_backCapture = new QGraphicsPixmapItem;

    // Timeline setup
    m_timeline = new QTimeLine(1000, this);
    m_timeline->setUpdateInterval(10);

    // Connect signals
    connect(m_timeline, SIGNAL(valueChanged(qreal)), this, SLOT(handleTimelineUpdated(qreal)));
    connect(m_timeline, SIGNAL(finished()),          this, SLOT(handleTimelineFinished()));
}

//-------------------------------------------------------------------------------------------------
void View::setFrontInstrument(gics::Instrument* instrument)
{
    if (m_front)
        delete m_front;

    m_front = instrument;

    if (m_front)
    {
        if (m_timeline->state() != QTimeLine::Running)
        {
            m_scene.addItem(m_front);
            m_front->setVisible(m_timeline->direction() == QTimeLine::Forward);
        }
    }
}

//-------------------------------------------------------------------------------------------------
gics::Instrument* View::frontInstrument() const
{
    return m_front;
}

//-------------------------------------------------------------------------------------------------
void View::setBackInstrument(gics::Instrument* instrument)
{
    if (m_back)
        delete m_back;

    m_back = instrument;

    if (m_back)
    {
        if (m_timeline->state() != QTimeLine::Running)
        {
            m_scene.addItem(m_back);
            m_back->setVisible(m_timeline->direction() == QTimeLine::Backward);
        }
    }
}

//-------------------------------------------------------------------------------------------------
gics::Instrument* View::backInstrument() const
{
    return m_back;
}

//-------------------------------------------------------------------------------------------------
void View::flip()
{
    if (m_front && m_back && (m_timeline->state() != QTimeLine::Running))
    {
        // Take a capture of the front and back instruments
        m_frontCapture->setPixmap(capture(m_front));
        m_backCapture->setPixmap(capture(m_back));

        // Replace the front and back instruments with their capture
        m_scene.removeItem(m_front);
        m_scene.removeItem(m_back);
        m_scene.addItem(m_frontCapture);
        m_scene.addItem(m_backCapture);

        // Start the timeline
        m_timeline->start();
    }
}

//-------------------------------------------------------------------------------------------------
void View::useOpenGL(bool use)
{
    if (use)
    {
        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
        setStyleSheet("background: black");
    }
    else
    {
        setViewport(new QWidget);
        setStyleSheet("background: transparent");
    }
}

//-------------------------------------------------------------------------------------------------
void View::resizeEvent(QResizeEvent* event)
{
    // Resize the view
    QGraphicsView::resizeEvent(event);

    // Resize the widgets
    if (m_front) m_front->resize(size());
    if (m_back)  m_back->resize(size());

    // Resize the scene
    m_scene.setSceneRect(0, 0, size().width(), size().height());
}

//-------------------------------------------------------------------------------------------------
void View::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);

    // This is to workaround Qt not forwarding properly the "accepted" state of the mouse event sent to the scene
    if (!m_scene.mouseGrabberItem())
        event->ignore();
}

//-------------------------------------------------------------------------------------------------
QPixmap View::capture(gics::Instrument* instrument)
{
    // Show only the item in the scene
    m_front->setVisible(false);
    m_back->setVisible(false);
    instrument->setVisible(true);

    // Prepare the target pixmap
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);

    // Render the item
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    m_scene.render(&painter);
    painter.end();

    return pixmap;
}

//-------------------------------------------------------------------------------------------------
void View::handleTimelineUpdated(qreal value)
{
    // Compute the transform to apply
    QPointF center(width() / 2.f, height() / 2.f);
    qreal angle = value < 0.5 ? (180 * value) : (180 * value - 180);
    qreal sizeFactor = 1 - sin(GICS_PI * value) * 0.4;
    QTransform transform = QTransform()
                           .translate(center.x(), center.y())
                           .rotate(angle, Qt::XAxis)
                           .scale(sizeFactor, sizeFactor)
                           .translate(-center.x(), -center.y());

    // Update the instruments
    if (value < 0.5)
    {
        m_backCapture->setVisible(false);
        m_frontCapture->setVisible(true);
        m_frontCapture->setTransform(transform);
    }
    else
    {
        m_frontCapture->setVisible(false);
        m_backCapture->setVisible(true);
        m_backCapture->setTransform(transform);
    }

    // Update the mask of the parent window to match the transformed view :)
    QRegion region = transform.map(QRegion(rect()));
    if (region.boundingRect().isValid())
        parentWidget()->setMask(region);
}

//-------------------------------------------------------------------------------------------------
void View::handleTimelineFinished()
{
    // Reset the parent window's mask
    parentWidget()->setMask(QRegion());

    // Flip the animation's direction
    m_timeline->toggleDirection();

    // Put the real instruments back into the scene
    m_scene.removeItem(m_frontCapture);
    m_scene.removeItem(m_backCapture);
    m_scene.addItem(m_front);
    m_scene.addItem(m_back);
    m_front->setVisible(m_timeline->direction() == QTimeLine::Forward);
    m_back->setVisible(m_timeline->direction() == QTimeLine::Backward);
}
