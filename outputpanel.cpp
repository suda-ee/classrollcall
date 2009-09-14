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


#include "outputpanel.hpp"
#include <gics/picture.hpp>
#include <gics/circularscale.hpp>
#include <gics/textlabel.hpp>
#include <gics/levelindicator.hpp>
#include <gics/numericlabel.hpp>
#include <gics/gauge.hpp>
#include <gics/led.hpp>
#include <gics/lcd.hpp>
#include <QGraphicsLinearLayout>


//-------------------------------------------------------------------------------------------------
OutputPanel::OutputPanel(QGraphicsItem* parent)
    : gics::Panel(parent)
{
    setSubClass("simple");
    background().setBorderMode(gics::Picture::fixed);

    // Create the title
    //gics::TextLabel* title = new gics::TextLabel("Output panel");
    //title->background().setVisible(false);
    //title->setHorizontalTextAlignment(gics::alignLeft);
    //QFont font = title->font();
    QFont font;
    //font.setBold(true);
    //title->setFont(font);

    // Create the level indicator
    m_indicator = new gics::LevelIndicator(Qt::Horizontal);
    m_indicator->setTitleLabelVisible(false);
    m_indicator->setValueLabelVisible(false);
    m_indicator->scale().graduations(0).setLabelsPrecision(0);
    m_indicator->setMainScaleVisible(false);

    // Create the numeric label
    //m_label = new gics::NumericLabel;
    //m_label->setPrefix("value = ");
    //font = m_label->font();
    //font.setPointSize(14);
    //m_label->setFont(font);
    //m_label->setHorizontalSizePolicy(QSizePolicy::Fixed);
    //m_label->setMinimumWidth(135);
    //m_label->setMaximumWidth(135);
    
    // Create the gauge
    m_gauge = new gics::Gauge;
    m_gauge->scale().setStartAngle(135);
    m_gauge->scale().setAngleCourse(270);
    m_gauge->valueLabel().setShowMinusSign(false);

    // Create the LEDs
    for (int i = 0; i < 3; ++i)
    {
        gics::Led* led = new gics::Led;
        led->setMinimumSize(32, 32);
        m_leds.append(led);
    }
    m_leds[0]->setSubClass("green");
    m_leds[1]->setSubClass("orange");
    m_leds[2]->setSubClass("red");

    // Create the LCD
    m_lcd = new gics::Lcd;
    m_lcd->setDisplayType(gics::Lcd::pixelSegments);
    m_lcd->setShowMinusSign(false);
    m_lcd->setIntegerPrecision(2);
    m_lcd->setDecimalPrecision(0);

    // Create the name text
    m_name = new gics::TextLabel(tr("¼×ÒÒ±û¶¡"));
    m_name->setHorizontalTextAlignment(gics::alignHCenter);
    m_name->setFont(QFont(tr("ºÚÌå"), 72, QFont::Black));
    m_name->setColor(Qt::yellow);

    // Create the indicator layout
    QGraphicsLinearLayout* indicatorLayout = new QGraphicsLinearLayout(Qt::Vertical);
    indicatorLayout->addItem(m_indicator);
    indicatorLayout->addItem(m_gauge);
    //indicatorLayout->addItem(m_label);
    indicatorLayout->setStretchFactor(m_indicator, 0);
    indicatorLayout->setStretchFactor(m_gauge, 1);
    //indicatorLayout->setStretchFactor(m_label, 0);

    // Create the LEDs layout
    QGraphicsLinearLayout* ledsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    foreach (gics::Led* led, m_leds)
        ledsLayout->addItem(led);

    // Create the LEDs + LCD layout
    QGraphicsLinearLayout* lcdLayout = new QGraphicsLinearLayout(Qt::Vertical);
    lcdLayout->addItem(ledsLayout);
    lcdLayout->addItem(m_lcd);
    lcdLayout->setStretchFactor(ledsLayout, 0);
    lcdLayout->setStretchFactor(m_lcd, 1);

    // Create the instruments layout
    QGraphicsLinearLayout* instrumentsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    instrumentsLayout->addItem(indicatorLayout);
    instrumentsLayout->addItem(lcdLayout);
    instrumentsLayout->addItem(m_name);
    instrumentsLayout->setStretchFactor(indicatorLayout, 1);
    instrumentsLayout->setStretchFactor(lcdLayout, 1);
    instrumentsLayout->setStretchFactor(m_name, 3);

    // Create the sid
    m_sid = new gics::LcdLabel;
    m_sid->setText("0000000000");
    m_sid->setInactiveColor(QColor(0, 0, 0, 0));
    m_sid->setActiveColor(Qt::yellow);

    // Create the main layout
    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->addItem(m_sid);
    mainLayout->addItem(instrumentsLayout);
    mainLayout->setStretchFactor(m_sid, 1);
    mainLayout->setStretchFactor(instrumentsLayout, 2);
    setCentralItem(mainLayout);
}

//-------------------------------------------------------------------------------------------------
void OutputPanel::setValue(double value)
{
    m_indicator->setValue(value);
    //m_label->setValue(value);
    m_gauge->setValue(value);
    //m_lcd->setValue(value);

    double step = (m_indicator->maximum() - m_indicator->minimum()) / (m_leds.size() + 1);
    for (int i = 0; i < m_leds.size(); ++i)
    {
        m_leds[i]->setOn(value >= step * (i + 1));
    }
}

void OutputPanel::setID(const QString &id)
{
    m_sid->setText(id);
}

void OutputPanel::setName(const QString &name)
{
    m_name->setText(name);
}

void OutputPanel::pumpUpOne()
{
    m_lcd->setValue(m_lcd->value() + 1);
}
