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


#include "inputpanel.hpp"
#include <gics/picture.hpp>
#include <gics/textlabel.hpp>
#include <gics/slider.hpp>
#include <gics/numericfield.hpp>
#include <QGraphicsLinearLayout>
#include <QTimeLine>


//-------------------------------------------------------------------------------------------------
InputPanel::InputPanel(QGraphicsItem* parent)
    : gics::Panel(parent)
{
    setSubClass("simple");
    background().setBorderMode(gics::Picture::fixed);

    // Create the title
    //gics::TextLabel* title = new gics::TextLabel("Input panel");
    //title->background().setVisible(false);
    //title->setHorizontalTextAlignment(gics::alignLeft);
    //QFont font = title->font();
    QFont font;
    //font.setBold(true);
    //title->setFont(font);

    // Create the slider
    m_slider = new gics::Slider(Qt::Horizontal);
    m_slider->setTitleLabelVisible(false);
    m_slider->setValueFieldVisible(false);
    m_slider->setMainScaleVisible(false);

    // Create the field
    //m_field = new gics::NumericField;
    //m_field->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    //m_field->setPrefix("value: ");
    //m_field->setSuffix(" %");
    //font = m_field->font();
    //font.setPointSize(16);
    //m_field->setFont(font);

    // Create the "play" button
    play = new gics::Button;
    play->setSubClass("circular");
    play->setIconFile(":skins/buttons.svg");
    play->setIconId("icon_play");
    play->setDisplaySettings(gics::Button::showIcon);
    play->setButtonType(gics::Button::bistable);
    play->setMinimumSize(48, 48);
    play->setIconHeight(25);

    // Create the "pause" button
    //gics::Button* pause = new gics::Button;
    //pause->setSubClass("circular");
    //pause->setIconFile(":skins/buttons.svg");
    //pause->setIconId("icon_pause");
    //pause->setDisplaySettings(gics::Button::showIcon);
    //pause->setButtonType(gics::Button::monostable);
    //pause->setMinimumSize(60, 60);
    //pause->setIconHeight(25);

    // Create the timeline
    m_timeline = new QTimeLine(3000, this);
    m_timeline->setCurveShape(QTimeLine::SineCurve);
    m_timeline->setUpdateInterval(40);

    // Create the buttons layout
    QGraphicsLinearLayout* buttonsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    buttonsLayout->addItem(play);
    //buttonsLayout->addItem(pause);

    // Create the field + buttons layout
    QGraphicsLinearLayout* fieldLayout = new QGraphicsLinearLayout(Qt::Vertical);
    //fieldLayout->addItem(m_field);
    fieldLayout->addItem(buttonsLayout);

    // Create the instruments layout
    QGraphicsLinearLayout* instrumentsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    instrumentsLayout->addItem(m_slider);
    instrumentsLayout->addItem(fieldLayout);
    instrumentsLayout->setStretchFactor(m_slider, 8);
    instrumentsLayout->setStretchFactor(fieldLayout, 1);

    // Create the main layout
    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    //mainLayout->addItem(title);
    mainLayout->addItem(instrumentsLayout);
    //mainLayout->setStretchFactor(title, 0);
    mainLayout->setStretchFactor(instrumentsLayout, 1);
    setCentralItem(mainLayout);

    // Connect signals
    connect(m_slider,  SIGNAL(valueChanged(double)), this, SLOT(handleValueChanged(double)));
    //connect(m_field, SIGNAL(valueChanged(double)), this, SLOT(handleValueChanged(double)));
    connect(m_timeline, SIGNAL(valueChanged(qreal)), this, SLOT(handleTimelineChanged(qreal)));
    connect(play, SIGNAL(clicked(bool)), this, SLOT(onBtClick(bool)));
    //connect(play, SIGNAL(clicked()), m_slider, SLOT(setDisabled()));
    //connect(play, SIGNAL(clicked()), m_field, SLOT(setDisabled()));
    //connect(play, SIGNAL(clicked()), m_timeline, SLOT(start()));
    //connect(pause, SIGNAL(clicked()), m_slider, SLOT(setEnabled()));
    //connect(pause, SIGNAL(clicked()), m_field, SLOT(setEnabled()));
    //connect(pause, SIGNAL(clicked()), m_timeline, SLOT(stop()));
    connect(m_timeline, SIGNAL(finished()), this, SLOT(handleTimelineFinished()));
}

//-------------------------------------------------------------------------------------------------
void InputPanel::handleValueChanged(double value)
{
    m_slider->setValue(value);
    //m_field->setValue(value);

    emit valueChanged(value);
}

//-------------------------------------------------------------------------------------------------
void InputPanel::handleTimelineChanged(qreal value)
{
    handleValueChanged(value * 100.);
}

//-------------------------------------------------------------------------------------------------
void InputPanel::handleTimelineFinished()
{
    m_timeline->toggleDirection();
    m_timeline->start();
}

void InputPanel::onBtClick(bool down)
{
    if (down)
    {
	play->setIconId("icon_pause");
	m_timeline->start();
    }
    else
    {
	play->setIconId("icon_play");
	m_timeline->stop();
	emit oneLucky();
    }
}

void InputPanel::onKeySpace()
{
    play->click();
}
