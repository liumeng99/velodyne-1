// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @author  Marek Kurdej <firstname.surname@utc.fr>
/// @date    April, 2007
/// @version $Id: main.cpp 91 2013-05-19 10:32:48Z gdherbom $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief DbitePlayer main function.
///
/// Detailed description.

#include <Pacpus/DbitePlayer/DbtPlyEngine.h>
#include <Pacpus/DbitePlayer/DbtPlyTrigger.h>
#include <Pacpus/DbitePlayer/DbtPlyUserInterface.h>
#include <Pacpus/kernel/ComponentManager.h>
#include <Pacpus/kernel/ComponentFactory.h>
#include <Pacpus/kernel/Log.h>
#include <Pacpus/kernel/PacpusApplication.h>
#include <Pacpus/kernel/PacpusPluginInterface.h>

#include <cassert>
#include <QApplication>

using namespace pacpus;
using namespace std;

DECLARE_STATIC_LOGGER("pacpus.core.DbitePlayer");

static const string kDefaultXmlConfigFilePath = "DbitePlayer.xml";

int main(int argc, char * argv[])
{
    PacpusApplication app(argc, argv);
    ComponentManager * mgr = ComponentManager::getInstance();

    static ComponentFactory<DbtPlyEngine> factoryDbtPlyEngine("DbtPlyEngine");
    Q_UNUSED(factoryDbtPlyEngine);
    static ComponentFactory<DbtPlyTrigger> factoryDbtPlyTrigger("DbtPlyTrigger");
    Q_UNUSED(factoryDbtPlyTrigger);
    static ComponentFactory<DbtPlyUserInterface> factoryDbtPlyUserInterface("DbtPlyUserInterface");
    Q_UNUSED(factoryDbtPlyUserInterface);

    string configFilePath;
    if (argc > 1) {
        configFilePath = argv[1];
    } else {
        configFilePath = kDefaultXmlConfigFilePath;
        LOG_WARN("no XML config file specified. Using default: " << configFilePath);
    }
    LOG_INFO("loading file '" << configFilePath << "'");
    size_t componentCount = mgr->loadComponents(configFilePath.c_str());
    if (componentCount == 0) {
        LOG_FATAL("no components, exiting");
        return EXIT_FAILURE;
    }

    // start all the components (engine, trigger, user interface and the DBT file managers)
    mgr->start();
    // save application exit status
    int exitStatus = app.exec();
    // stop all components before exiting app
    mgr->stop();

    mgr->destroy();

    // return application exit status
    return exitStatus;
}
