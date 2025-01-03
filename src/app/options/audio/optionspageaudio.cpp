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

#include "optionspageaudio.h"
#include "optionspageaudioinputoutputpage.h"
// #include "optionspageaudiomidipage.h"
#include "mainwindow/mainwindow.h"

namespace options {

PageAudio::PageAudio(OptionsDialog *optionsDialog) :
    PageSavingTabWidget("audio"),
    CentralWidgetInterface(this)
{
    this->addTab(new PageAudioInputOutput(optionsDialog, QAudio::AudioInput), tr("Input device"));
    this->addTab(new PageAudioInputOutput(optionsDialog, QAudio::AudioOutput), tr("Output device"));
    // this->addTab(new PageAudioMidi(optionsDialog, optionsDialog->getMainWindow()->getMidiAutoConnector()), tr("Midi"));

    restorePageFromSettings();
}

void PageAudio::apply() {
    dynamic_cast<ContentsWidgetInterface*>(currentWidget())->apply();
}

}  // namespace options
