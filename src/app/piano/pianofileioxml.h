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

#ifndef PIANOFILEIOXML_H
#define PIANOFILEIOXML_H

#include "config/prerequisites.h"

#include "pianofileiointerface.h"

class PianoFileIOXml : public PianoFileIOInterface
{
public:
    using FileVersionType = int;
    static const QString FILE_TYPE_NAME;
    static const FileVersionType UNSET_FILE_VERSION;
    static const FileVersionType CURRENT_FILE_VERSION;
    static const FileVersionType MIN_SUPPORTED_FILE_VERSION;

    virtual void write(QIODevice *device, const Piano &piano) const override final;
    virtual void read(QIODevice *device, Piano &piano) override final;

private:
    /// file version as integer
    FileVersionType mFileVersion;
};

#endif // PIANOFILEIOXML_H
