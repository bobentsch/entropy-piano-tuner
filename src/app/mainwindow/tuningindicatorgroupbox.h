/*****************************************************************************
 * Copyright 2018 Haye Hinrichsen, Christoph Wick
 *
 * This file is part of Entropy Piano Tuner.
 *
 * Entropy Piano Tuner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Entropy Piano Tuner is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Entropy Piano Tuner. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef TUNINGINDICATORGROUPBOX_H
#define TUNINGINDICATORGROUPBOX_H

#include "config/prerequisites.h"

#include "drawers/tuningindicatorview.h"
#include "widgets/displaysizedependinggroupbox.h"


class TuningIndicatorGroupBox : public DisplaySizeDependingGroupBox
{
public:
    TuningIndicatorGroupBox(QWidget *parent);

    TuningIndicatorView* getTunincIndicatorView() { return graph; }

private:
    TuningIndicatorView *graph;
};

#endif // TUNINGINDICATORGROUPBOX_H
