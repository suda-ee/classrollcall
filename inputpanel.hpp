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


#ifndef INPUTPANEL_HPP
#define INPUTPANEL_HPP


#include <gics/panel.hpp>
#include <gics/button.hpp>


class QTimeLine;

namespace gics
{
    class Slider;
    class NumericField;
}

/**
 * \brief Panel that contains all the input instruments
 */
class InputPanel : public gics::Panel
{
    Q_OBJECT

public:

    /**
     * \brief Default constructor
     *
     * \param parent Parent item (0 by default)
     */
    InputPanel(QGraphicsItem* parent = 0);

    void onKeySpace();

signals:

    /**
     * \brief Signal emitted when the input value has changed
     *
     * \param value New input value
     */
    void valueChanged(double value);

    void oneLucky();

private slots:

    /**
     * \brief Called when one of the input instruments have changed its value
     *
     * \param value New value
     */
    void handleValueChanged(double value);

    /**
     * \brief Called when the timeline is updated
     *
     * \param value New value
     */
    void handleTimelineChanged(qreal value);

    /**
     * \brief Called when the timeline finishes
     */
    void handleTimelineFinished();

    void onBtClick(bool down);

private:

    gics::Slider* m_slider; ///< The slider
    //gics::NumericField* m_field; ///< The numeric field
    QTimeLine* m_timeline; ///< Timeline for controlling the automatic update of the value
    gics::Button* play;

};

#endif // INPUTPANEL_HPP
