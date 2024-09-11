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

///////////////////////////////////////////////////////////////////////////////
/// \mainpage Entropy-Piano-Tuner
/// \author Christoph Wick and Haye Hinrichsen
/// \author Institute for Physics and Astronomy<br>
/// University of Würzburg<br> 97074 Würzburg, Germany
/// \author e-mail: info at piano-tuner.org
///
/// The Entropy-Piano-Tuner (EPT) is a free experimental software for piano
/// tuning.
///
/// The source code of the EPT consists mainly of two parts, namely, the
/// graphical user interface, which is implemented by using Qt,
/// and a GUI-independent part called 'core'. The tuning algorithms and
/// third-party components can be found in separate folders.
///
/// To download and compile the EPT source on your system please install
/// the open-source version of Qt and the version management sytsem git.
/// Then create a local clone of the source by typing
///
/// \code git clone https://gitlab.com/entropytuner/Entropy-Piano-Tuner.git
/// \endcode
///
/// Alternatively you can also download the project without using git.
/// Go to https://gitlab.com/entropytuner/Entropy-Piano-Tuner/tree/master
/// and choose one of the download options in the upper right corner of
/// the window.
///
/// More details can be found on our development web pages at
/// http://develop.piano-tuner.org
//////////////////////////////////////////////////////////////////////////////

#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <qdebug.h>


#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/spdlog-inl.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "core/system/serverinfo.h"
#include "core/system/eptexception.h"
#include "core/config.h"

#include "implementations/filemanagerforqt.h"
#include "implementations/settingsforqt.h"
#include "implementations/platformtools.h"
#include "config/qtconfig.h"
#include "runguard.h"
#include "tunerapplication.h"



void spdlog_init(const std::string& filename) {
    int size = 10*1024*1024; // 10M
    int backcount = 5;       // 5

    // create console_sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    // create rotating file sink
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, size, backcount, true);
    file_sink->set_level(spdlog::level::debug);

    // sink's bucket
    spdlog::sinks_init_list sinks{console_sink, file_sink};

    // create async logger, and use global threadpool
    spdlog::init_thread_pool(1024 * 8, 1);
    auto logger = std::make_shared<spdlog::async_logger>("aslogger", sinks, spdlog::thread_pool());

    spdlog::register_logger(logger);
    spdlog::set_level(spdlog::level::debug);

    // set default logger
    spdlog::set_default_logger(logger);
    spdlog::initialize_logger(logger);

    SPDLOG_LOGGER_DEBUG(logger, "SPDLOG_LOGGER_DEBUG——debug");
    SPDLOG_DEBUG("SPDLOG_DEBUG——debug");
    spdlog::debug("debug——debug");

    spdlog::info("usdhaid");
}

int main(int argc, char *argv[])
{
    // basic application properties (needed for settings)
    QCoreApplication::setOrganizationName("tp3");
    QCoreApplication::setOrganizationDomain(serverinfo::SERVER_DOMAIN.c_str());
    QCoreApplication::setApplicationName("Entropy Piano Tuner");

    // =========================================================================================
    // Create application an initialize the basic components:
    //  - Plaform dependent tools
    //  - Settings
    //  - Translation

    // create our application object
    TunerApplication a(argc, argv);

    // setup platformtools

    // required if no platform specific platform tools
    std::unique_ptr<PlatformTools> defaultPlatformTools;
    if (!PlatformTools::getSingleton()) {
        defaultPlatformTools.reset(new PlatformTools());
        // no platform specific platform tools, use default ones
    }

    // Settings object
    QSettings settings;

    // install language files
    QTranslator qtTranslator;

    QString localeName(settings.value(SettingsForQt::KEY_LANGUAGE_ID, QString()).toString());
    if (localeName.isEmpty()) {
        // system language
        localeName = QLocale::system().name();
    }
    // set default to be sure that this is not "C"
    QLocale::setDefault(QLocale(localeName));

    // Qt translation
    bool qTranslatorLoadStatus = qtTranslator.load(QLocale(), "qt", "_", ":/languages/translations");

    a.installTranslator(&qtTranslator);

    // application translation
    QTranslator myappTranslator;
    qTranslatorLoadStatus = myappTranslator.load(QLocale(), "piano_tuner", "_", ":/languages/translations");
    a.installTranslator(&myappTranslator);

    // =========================================================================================
    // Check if app is already running

    // only single instance also on desktop (on mobile platforms this is handled already, winphone needs an "extra sausage")
#if defined(Q_OS_DESKTOP) && !defined(Q_OS_WINPHONE)
    RunGuard guard("entropypianotuner_runguard");
    if ( !guard.tryToRun() ) {
        // a QApplication is required for showing message boxes
        QMessageBox::warning(nullptr, a.tr("Application can not be started"), a.tr("The Entropy Piano Tuner could not be started because another instance is already running."));
        return 0;
    }
#endif

    // =========================================================================================
    // Launching the application, mainwindow, core, ...

    // create file manager instance to initialize file paths
    new FileManagerForQt();

    // Initialize log

    spdlog_init(FileManagerForQt::getSingleton().getLogFilePath("log.txt"));

    int exitCode = -1;

    try {
        // create settings
        (new SettingsForQt())->load();

        // increase run count
        SettingsForQt::getSingleton().increaseApplicationRuns();

        a.playStartupSound();

        // init all components
        a.init();

        // start the application
        a.start();

        // execute the application
        exitCode = a.exec();
    }
    catch (const EptException &e) {
        qCritical() << "Unhandled exception: ";
        qCritical() << QString::fromStdString(e.getFullDescription());
        exitCode = EXIT_FAILURE;
    }
    catch (const std::exception &e) {
        qCritical() << "Unhandled exception: ";
        qCritical() << QString::fromStdString(e.what());
        exitCode = EXIT_FAILURE;
    }
    catch (...) {
        qCritical() << "Unhandled exception: ";
        qCritical() << "unknown exception";
        exitCode = EXIT_FAILURE;
    }

    PlatformTools::getSingleton()->enableScreensaver();

    return exitCode;
}
