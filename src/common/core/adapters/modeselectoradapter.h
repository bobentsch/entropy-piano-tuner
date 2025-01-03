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

//=============================================================================
//                        Adapter for mode selection
//=============================================================================

#ifndef MODESELECTORADAPTER_H
#define MODESELECTORADAPTER_H

#include "core/prerequisites.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief The ModeSelectorAdapter class
///
/// The tuner operates in different modes (recording, calculating, tuning etc.).
/// This class provides an abstract interface which allows the GUI to switch
/// between the operation modes.
///
/// In Qt this class is implemented in ModeSelector (file modeselector.h).
///////////////////////////////////////////////////////////////////////////////

class EPT_EXTERN ModeSelectorAdapter
{
public:
    ModeSelectorAdapter();
    ~ModeSelectorAdapter() {}

protected:
    void setMode(OperationMode m);
    OperationMode mCurrentMode;
};

#endif // MODESELECTORADAPTER_H
