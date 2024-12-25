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

#include "audiointerfaceforqt.h"

#include <QSettings>
#include <QMediaDevices>

#include "core/audio/pcmdevice.h"
#include "core/system/log.h"

const int AudioInterfaceForQt::DEFAULT_BUFFER_SIZE_MS(100);

AudioInterfaceForQt::AudioInterfaceForQt(QAudioDevice::Mode mode, QObject *parent)
    : QObject(parent)
    , mMode(mode)
    , mSettingsPrefix(mode == QAudioDevice::Mode::Input ? "audio/input/" : "audio/output/")
    , mPCMDevice(this)
{
    // these settings are fixed
    // mFormat.setCodec("audio/pcm");
    mFormat.setSampleFormat(QAudioFormat::SampleFormat::Int32);
    // mFormat.setSampleSize(sizeof(PCMDevice::DataType) * 8);
    // mFormat.setSampleType(QAudioFormat::SignedInt);

    // default values, these can be changed
    mFormat.setSampleRate(mode == QAudioDevice::Mode::Input ? 44100 : 22050);
    mFormat.setChannelCount(mode == QAudioDevice::Mode::Input ? 1 : 2);
}

void AudioInterfaceForQt::reinitialize(int samplingRate, int channelCount, const QAudioDevice& device, int bufferSizeMS)
{
    exit();
    mDevice = device;
    mFormat.setSampleRate(samplingRate);
    mFormat.setChannelCount(channelCount);

    // only necessary if default settings
    if (!device.isFormatSupported(mFormat)) {
        LogW("Raw audio format not supported by backend, falling back to nearest supported");
        return;
        // mFormat = deviceInfo.nearestFormat(mFormat);
        // // update sampling rate, buffer type has to stay the same!
        // if (!device.isFormatSupported(mFormat))
        // {
        //     LogW("Fallback failed. Probably there is no device available.");
        //     return;
        // }

        // if (mFormat.sampleSize() != sizeof(PCMDevice::DataType) * 8) {
        //     LogW("Sample size not supported");
        //     return;
        // }

        // if (mFormat.sampleType() != QAudioFormat::SignedInt) {
        //     LogW("Sample format not supported");
        //     return;
        // }
    }

    QAudio::Error err = createDevice(mFormat, device, bufferSizeMS);
    if (err != QAudio::NoError) {
        LogE("Error creating audio device with error %d", static_cast<int>(err));
        return;
    }

    LogI("Initialized audio using device: %s", getDeviceName().c_str());

    QSettings s;
    s.setValue(mSettingsPrefix + "samplerate", QVariant::fromValue(mFormat.sampleRate()));
    s.setValue(mSettingsPrefix + "channels", QVariant::fromValue(mFormat.channelCount()));

    s.setValue(mSettingsPrefix + "devicename", QVariant::fromValue(mDevice.description()));
    s.setValue(mSettingsPrefix + "buffersize", QVariant::fromValue(bufferSizeMS));
}

void AudioInterfaceForQt::init()
{
    QSettings s;

    // get device info
    QString deviceName = s.value(mSettingsPrefix + "devicename").toString();
    if (deviceName.isEmpty()) {
        if (mMode == QAudioDevice::Mode::Input) {
            mDevice = QMediaDevices::defaultAudioInput();
        } else if (mMode == QAudioDevice::Mode::Output) {
            mDevice = QMediaDevices::defaultAudioOutput();
        }
    } else {
        QList<QAudioDevice> devices;
        if(mMode == QAudioDevice::Mode::Input) {
            devices = QMediaDevices::audioInputs();
        } else {
            devices = QMediaDevices::audioOutputs();
        }
        for (QAudioDevice& d: devices) {
            if (d.description() == deviceName) {
                mDevice = d;
                break;
            }
        }
    }

    if (mDevice.isNull()) {
        if (mMode == QAudioDevice::Mode::Input) {
            mDevice = QMediaDevices::defaultAudioInput();
        } else if (mMode == QAudioDevice::Mode::Output) {
            mDevice = QMediaDevices::defaultAudioOutput();
        }
    }

    // get format
    mFormat.setSampleRate(s.value(mSettingsPrefix + "samplerate", QVariant::fromValue(mFormat.sampleRate())).toInt());
    mFormat.setChannelCount(s.value(mSettingsPrefix + "channels", QVariant::fromValue(mFormat.channelCount())).toInt());

    // these settings are required
    if (mMode == QAudioDevice::Mode::Input) {
        mFormat.setChannelCount(1);
    }

    // buffer size
    int bufferSize = s.value(mSettingsPrefix + "buffersize", QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS)).toInt();

    // initialize device
    reinitialize(mFormat.sampleRate(), mFormat.channelCount(), mDevice, bufferSize);
}

const std::string AudioInterfaceForQt::getDeviceName() const
{
    return mDevice.description().toStdString();
}

int AudioInterfaceForQt::getSamplingRate() const
{
    return mFormat.sampleRate();
}

int AudioInterfaceForQt::getChannelCount() const
{
    return mFormat.channelCount();
}

PCMDevice *AudioInterfaceForQt::getDevice() const
{
    return mPCMDevice.getDevice();
}

void AudioInterfaceForQt::setDevice(PCMDevice *device)
{
    mPCMDevice.setDevice(device);
}

int AudioInterfaceForQt::getBufferSizeMS() const
{
    QSettings s;
    return s.value(mSettingsPrefix + "buffersize", QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS)).toInt();
}
