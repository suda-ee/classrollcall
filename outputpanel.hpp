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


#ifndef OUTPUTPANEL_HPP
#define OUTPUTPANEL_HPP


#include <gics/panel.hpp>
#include <gics/textlabel.hpp>
#include <QList>
#include <gics/lcdlabel.hpp>


namespace gics
{
    class LevelIndicator;
    class NumericLabel;
    class Gauge;
    class Led;
    class Lcd;
}

/**
 * \brief Panel that contains all the output instruments
 */
class OutputPanel : public gics::Panel
{
    Q_OBJECT

public:

    /**
     * \brief Default constructor
     *
     * \param parent Parent item (0 by default)
     */
    OutputPanel(QGraphicsItem* parent = 0);

public slots:

    /**
     * \brief Change the current value
     *
     * \param value New value
     */
    void setValue(double value);
    void setID(const QString &id);
    void setName(const QString &name);
    void pumpUpOne();

private:

    gics::LevelIndicator* m_indicator; ///< The level indicator
    //gics::NumericLabel* m_label; ///< The numeric label
    gics::Gauge* m_gauge; ///< The gauge
    QList<gics::Led*> m_leds; ///< The LEDs
    gics::Lcd* m_lcd; ///< The LCD
    gics::TextLabel* m_name; // The student's name
    gics::LcdLabel* m_sid; ///< The student's ID number
};

#endif // OUTPUTPANEL_HPP
