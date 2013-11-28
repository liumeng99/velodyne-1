// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @version $Id: DbtPlyTrigger.cpp 91 2013-05-19 10:32:48Z gdherbom $

#include <Pacpus/DbitePlayer/DbtPlyTrigger.h>

#include <Pacpus/kernel/ComponentManager.h>
#include <Pacpus/kernel/Log.h>

namespace pacpus {

DECLARE_STATIC_LOGGER("pacpus.core.DbtPlyTrigger");

// Construction de la fabrique de composant DbtPlyTrigger
//static ComponentFactory<DbtPlyTrigger> factory("DbtPlyTrigger");

DbtPlyTrigger::DbtPlyTrigger(QString name)
    : ComponentBase(name)
    , mEngine(NULL)
{
    THREAD_ALIVE = false;
}

DbtPlyTrigger::~DbtPlyTrigger()
{ 
}

ComponentBase::COMPONENT_CONFIGURATION DbtPlyTrigger::configureComponent(XmlComponentConfig /*config*/)
{
    ComponentManager * mgr = ComponentManager::getInstance();
    // we get a pointer to the engine component
    mEngine = dynamic_cast<DbtPlyEngine *>(mgr->getComponent("dbiteEngine"));
    if (NULL == mEngine) {
        LOG_FATAL("cannot get a pointer of the 'dbiteEngine' component");
        return CONFIGURED_FAILED;
    }
    connect(this, SIGNAL(triggerSig()),
            mEngine,SLOT(engReceiver()),
            Qt::DirectConnection);

    return CONFIGURED_OK;
}

void DbtPlyTrigger::startActivity()
{   
    THREAD_ALIVE = true;
    start();
}

void DbtPlyTrigger::stopActivity()
{ 
    THREAD_ALIVE = false;
}

// The thread is started when the user press the play button
void DbtPlyTrigger::run()
{
#ifdef WIN32
    timeBeginPeriod(1);
#endif

    while(THREAD_ALIVE) {
        if (mEngine->isPlaying()) {
            Q_EMIT triggerSig();
        }
        msleep(1);
    }

#ifdef WIN32
    timeEndPeriod(1);
#endif 
}

} // namespace pacpus
