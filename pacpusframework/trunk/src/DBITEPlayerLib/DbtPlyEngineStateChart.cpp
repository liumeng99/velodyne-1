// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @version $Id: DbtPlyEngineStateChart.cpp 91 2013-05-19 10:32:48Z gdherbom $

#include <Pacpus/DbitePlayer/DbtPlyEngineStateChart.h>

#include <Pacpus/DbitePlayer/DbtPlyEngine.h>
#include <Pacpus/kernel/Log.h>

namespace pacpus {

DECLARE_STATIC_LOGGER("pacpus.core.DbtPlyEngineStateChart");

typedef float SpeedType;
static const SpeedType kMinSpeed = 1.0 / 32;
static const SpeedType kMaxSpeed = 32;

////////////////////////////////////////////////////////////////////////////////
DbtPlyEngineState::DbtPlyEngineState()
{
}

DbtPlyEngineState::~DbtPlyEngineState()
{
}

void DbtPlyEngineState::play(DbtPlyEngine & /*engine*/)
{
    // do nothing
}

void DbtPlyEngineState::pause(DbtPlyEngine & /*engine*/)
{
    // do nothing
}

void DbtPlyEngineState::stop(DbtPlyEngine & /*engine*/)
{
    // do nothing
}

void DbtPlyEngineState::speedUp(DbtPlyEngine & engine)
{
    engine.speedUp();
}

void DbtPlyEngineState::speedDown(DbtPlyEngine & engine)
{
    engine.speedDown();
}

bool DbtPlyEngineState::isPlaying()
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////
PlayingState::PlayingState()
{
}

void PlayingState::pause(DbtPlyEngine & engine)
{
    engine.setState(PausedState::getInstance());
}

void PlayingState::stop(DbtPlyEngine & engine)
{
    engine.reset();
    engine.setState(StoppedState::getInstance());
}

bool PlayingState::isPlaying()
{
    return true;
}

QString PlayingState::toString() const
{
    return "Playing";
}

DbtPlyEngineState * PlayingState::getInstance()
{
    return &mInstance;
}

PlayingState PlayingState::mInstance;

////////////////////////////////////////////////////////////////////////////////
PausedState::PausedState()
{
}

void PausedState::play(DbtPlyEngine & engine)
{
    engine.setLastTNow(road_time());
    engine.setState(PlayingState::getInstance());
}

void PausedState::stop(DbtPlyEngine & engine)
{
    engine.reset();
    engine.setState(StoppedState::getInstance());
}

QString PausedState::toString() const
{
    return "Paused";
}

DbtPlyEngineState * PausedState::getInstance()
{
    return &mInstance;
}

PausedState PausedState::mInstance;

////////////////////////////////////////////////////////////////////////////////
StoppedState::StoppedState()
{
}

void StoppedState::play(DbtPlyEngine & engine)
{
    engine.setLastTNow(road_time());
    engine.setState(PlayingState::getInstance());
}

QString StoppedState::toString() const
{
    return "Stopped";
}

DbtPlyEngineState * StoppedState::getInstance()
{
    return &mInstance;
}

StoppedState StoppedState::mInstance;

} // namespace pacpus
