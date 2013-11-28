// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @version $Id: DbtPlyEngine.cpp 91 2013-05-19 10:32:48Z gdherbom $

#include <Pacpus/DbitePlayer/DbtPlyEngine.h>

#include <Pacpus/kernel/ComponentManager.h>
#include <Pacpus/kernel/Log.h>

#include <cassert>
#include <limits>
#include <QDir>

using namespace std;

// Construction de la fabrique de composant DbtPlyEngine
//static ComponentFactory<DbtPlyEngine> factory("DbtPlyEngine");

namespace pacpus {

DECLARE_STATIC_LOGGER("pacpus.core.DbtPlyEngine");

static const string kPropertyDataDirectory = "datadir";

static const string kPropertyLoggerConfiguration = "log-config-file";

static const string kPropertyReplayMode = "replay_mode";
static const string kPropertyReplayModeLastData = "1";
static const string kPropertyReplayModeAllData = "2";

typedef float SpeedType;
static const SpeedType kInitialSpeed = 1.0;
static const SpeedType kMinSpeed = 1.0 / 32;
static const SpeedType kMaxSpeed = 32;

////////////////////////////////////////////////////////////////////////////////
/// Constructor
DbtPlyEngine::DbtPlyEngine(QString name)
    : ComponentBase(name)
    , mCurrentState(StoppedState::getInstance())
    , mSpeed(kInitialSpeed)
    , mIsReverse(false)
{
    LOG_TRACE("constructor");

    sync_ = new QSemaphore(1);
    sync_->acquire();
    direction_ = 1;
    tMinMaxSem_ = new QSemaphore(1);
    tMinMaxSem_->release();
    tDbtMin_ = numeric_limits<road_time_t>::max();
    tDbtMax_ = 0;
    dataDir_ = QString::null;
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor.
DbtPlyEngine::~DbtPlyEngine()
{
    LOG_TRACE("destructor");
}

////////////////////////////////////////////////////////////////////////////////
/// Returns the directory where the data are stored.
/// @return QString::null if directory is invalid.
QString DbtPlyEngine::getDataDir()
{
    return dataDir_;
}

////////////////////////////////////////////////////////////////////////////////
/// Slot.
/// called when trigger reaches a new timer event
void DbtPlyEngine::engReceiver()
{
    sync_->release();
}

////////////////////////////////////////////////////////////////////////////////
/// @returns @b true if the engine is in the PLAY_STATE, @b false otherwise
bool DbtPlyEngine::isPlaying()
{
    return mCurrentState->isPlaying();
}

////////////////////////////////////////////////////////////////////////////////
/// main loop of the thread
/// entry point for a new thread after the invocation of QThread::start()
void DbtPlyEngine::run()
{
    LOG_DEBUG("run");

    // We are waiting the first trigger signal to init some values
    // before entering in the while loop
    sync_->acquire();
    lastTDbt_ = tDbtMin_;

    // For statistics purpose
    static const size_t kDeltaArraySize = 1000;
    int deltaTDbtTab[kDeltaArraySize];
    int i = 0;

    while (THREAD_ALIVE) {
        tNow_ = road_time();
        float elapsedTime = tNow_ - lastTNow_;
        int deltaTDbt = direction_ * elapsedTime * mSpeed;
        tDbt_ = lastTDbt_ + deltaTDbt;
        deltaTDbtTab[(i++) % kDeltaArraySize] = elapsedTime;

        if ((tDbt_ <= tDbtMax_) && (tDbt_ >= tDbtMin_)) {
            Q_EMIT play(tDbt_, tNow_, mIsReverse);
            Q_EMIT curReplayTime(tDbt_);
            lastTNow_ = tNow_ ;
            lastTDbt_ = tDbt_ ;
        } else {
            lastTNow_ = tNow_ ;
            lastTDbt_ = tDbt_ ;
            LOG_INFO("Reading is finished.");

            mCurrentState->stop(*this);
        }

        // The waiting is placed at the end due to the init before the while loop
        sync_->acquire();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Slot.
/// change the direction of replaying : normal or reverse
void DbtPlyEngine::changeDirection(bool reverse)
{
    if (reverse) {
        direction_ = -1;
        mIsReverse = true;
    } else {
        direction_ = +1;
        mIsReverse = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Configuration method of the engine
/// called automatically by the component manager
ComponentBase::COMPONENT_CONFIGURATION DbtPlyEngine::configureComponent(XmlComponentConfig /*config*/)
{
    // datadir
    dataDir_ = param.getProperty(kPropertyDataDirectory.c_str());
    LOG_INFO("property " << kPropertyDataDirectory.c_str() << "=\""
             << dataDir_ << "\"");
    if (dataDir_.isNull()) {
        LOG_FATAL("The data directory '" << componentName << "' is invalid or unavailable!");
    }

    dataDir_ = QDir::toNativeSeparators(dataDir_);
    if (!dataDir_.endsWith(QDir::separator())) {
        dataDir_ += QDir::separator();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // logger configuration
    QString loggerConfig = param.getProperty(kPropertyLoggerConfiguration.c_str());
    LOG_INFO("property " << kPropertyLoggerConfiguration.c_str() << "=\""
             << loggerConfig << "\"");
    if (!loggerConfig.isNull()) {
        LOG_INFO("configuring logger with file '" << loggerConfig << "'");
        LogConfigurator::configureLoggerWithFile(loggerConfig.toStdString().c_str());
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Replay Mode
    QString replayModeValue = param.getProperty(kPropertyReplayMode.c_str());
    LOG_INFO("property " << kPropertyReplayMode.c_str() << "=\""
             << replayModeValue << "\"");
    if (replayModeValue.isNull()) {
        LOG_INFO("property " << kPropertyReplayMode.c_str() << " unset."
                 << " Set to default = 1.");
        replayMode_ = PlayModeLastData;
    } else {
        if (kPropertyReplayModeLastData == replayModeValue.toStdString()) {
            replayMode_ = PlayModeLastData;
        } else if (kPropertyReplayModeAllData == replayModeValue.toStdString()) {
            replayMode_ = PlayModeAllData;
        } else {
            LOG_WARN("unknown " << kPropertyReplayMode.c_str() << " '" << replayModeValue << "'."
                     << " Set to default = 1.");
            replayMode_ = PlayModeLastData;
        }
    }

    return ComponentBase::CONFIGURED_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// The start function of the engine
void DbtPlyEngine::startActivity()
{
    LOG_INFO("Starting...");

    THREAD_ALIVE = true;
    start();
}

////////////////////////////////////////////////////////////////////////////////
/// The stop function of the engine
void DbtPlyEngine::stopActivity()
{ 
    LOG_TRACE("stopping activity...");

    THREAD_ALIVE = false;
}

////////////////////////////////////////////////////////////////////////////////
/// Slot.
/// called by each file manager to provide min and max time of their data
void DbtPlyEngine::tMinMax(road_time_t tMin, road_time_t tMax)
{
    tMinMaxSem_->acquire();
    if (tDbtMin_>tMin) {
        tDbtMin_= tMin;
    }
    if (tDbtMax_<tMax) {
        tDbtMax_ = tMax;
    }
    Q_EMIT timeMinMax(tDbtMin_, tDbtMax_);
    tMinMaxSem_->release();
}

////////////////////////////////////////////////////////////////////////////////
/// Slot.
/// called when the time slider is pressed
void DbtPlyEngine::pauseEvent()
{
    LOG_DEBUG("Clicked: pause");
    mCurrentState->pause(*this);
}

////////////////////////////////////////////////////////////////////////////////
/// Slot.
/// called when the time slider is released
void DbtPlyEngine::playEvent()
{
    LOG_DEBUG("Clicked: play");

    // get user interface
    ComponentManager * mgr = ComponentManager::getInstance();
    DbtPlyUserInterface * ui = dynamic_cast<DbtPlyUserInterface *>(mgr->getComponent("dbiteUserInterface"));
    assert(ui);
    // get time value from slider
    lastTDbt_ = ui->getTime() + tDbtMin_;

    mCurrentState->play(*this);
}

////////////////////////////////////////////////////////////////////////////////
void DbtPlyEngine::stopEvent()
{
    LOG_DEBUG("Clicked: stop");
    mCurrentState->stop(*this);
}

////////////////////////////////////////////////////////////////////////////////
void DbtPlyEngine::speedUpEvent()
{
    LOG_DEBUG("Clicked: speed Up");
    mCurrentState->speedUp(*this);
}

////////////////////////////////////////////////////////////////////////////////
void DbtPlyEngine::speedDownEvent()
{
    LOG_DEBUG("Clicked: speed down");
    mCurrentState->speedDown(*this);
}

////////////////////////////////////////////////////////////////////////////////
const DbtPlyEngineState * DbtPlyEngine::getState()
{
    return mCurrentState;
}

////////////////////////////////////////////////////////////////////////////////
void DbtPlyEngine::setState(DbtPlyEngineState * newState)
{
    assert(newState);
    LOG_DEBUG(mCurrentState->toString() << " => " << newState->toString());
    mCurrentState = newState;

    Q_EMIT displayStateSig(mCurrentState, mSpeed);

    LOG_DEBUG("tDbt = " << tDbt_ << "\tlastTDbt = " << lastTDbt_);
    LOG_DEBUG("tNow = " << tNow_ << "\tlastTNow = " << lastTNow_);
}

void DbtPlyEngine::speedUp()
{
    mSpeed *= 2;
    if (mSpeed > kMaxSpeed) {
        mSpeed = kMaxSpeed;
    }
    assert(kMinSpeed <= mSpeed);
    assert(mSpeed <= kMaxSpeed);
    LOG_INFO("event: Speed Up, speed = " << mSpeed);
    Q_EMIT displayStateSig(mCurrentState, mSpeed);
}

void DbtPlyEngine::speedDown()
{
    mSpeed /= 2;
    if (mSpeed < kMinSpeed) {
        mSpeed = kMinSpeed;
    }
    assert(kMinSpeed <= mSpeed);
    assert(mSpeed <= kMaxSpeed);
    LOG_INFO("event: Speed Up, speed = " << mSpeed);
    Q_EMIT displayStateSig(mCurrentState, mSpeed);
}

void DbtPlyEngine::reset()
{
    lastTNow_ = road_time();
    lastTDbt_ = tDbtMin_;
    mSpeed = kInitialSpeed;
    Q_EMIT stopfile();

    // get user interface
    ComponentManager * mgr = ComponentManager::getInstance();
    DbtPlyUserInterface * ui = dynamic_cast<DbtPlyUserInterface *>(mgr->getComponent("dbiteUserInterface"));
    assert(ui);
    // reset time value of the slider
    ui->resetTime();
}

} // namespace pacpus
