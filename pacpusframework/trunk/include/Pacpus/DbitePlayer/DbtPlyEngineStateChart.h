// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @author  Marek Kurdej <firstname.surname@utc.fr>
/// @date    April, 2007
/// @version $Id: DbtPlyEngineStateChart.h 91 2013-05-19 10:32:48Z gdherbom $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief DbitePlayer state machine.
///
/// Detailed description.

#ifndef DEF_PACPUS_DBTPLYENGINESTATECHART_H
#define DEF_PACPUS_DBTPLYENGINESTATECHART_H

#include <QString>

#include <Pacpus/DbitePlayer/DbitePlayerConfig.h>

namespace pacpus {

class DbtPlyEngine;

////////////////////////////////////////////////////////////////////////////////
class DBITE_PLAYER_API DbtPlyEngineState
{
public:
    /// @todo Documentation
    virtual ~DbtPlyEngineState();
    
    /// @todo Documentation
    virtual void play(DbtPlyEngine & engine);
    /// @todo Documentation
    virtual void pause(DbtPlyEngine & engine);
    /// @todo Documentation
    virtual void stop(DbtPlyEngine & engine);
    
    /// @todo Documentation
    virtual void speedUp(DbtPlyEngine & engine);
    /// @todo Documentation
    virtual void speedDown(DbtPlyEngine & engine);
    
    /// @todo Documentation
    virtual bool isPlaying();
    
    /// @todo Documentation
    virtual QString toString() const = 0;

protected:
    /// @todo Documentation
    DbtPlyEngineState();
};

////////////////////////////////////////////////////////////////////////////////
class DBITE_PLAYER_API PlayingState
        : public DbtPlyEngineState
{
public:
    /// @todo Documentation
    virtual void pause(DbtPlyEngine & engine);
    /// @todo Documentation
    virtual void stop(DbtPlyEngine & engine);
    
    /// @todo Documentation
    virtual bool isPlaying();
    
    /// @todo Documentation
    virtual QString toString() const;
    /// @todo Documentation
    static DbtPlyEngineState * getInstance();

private:
    PlayingState();
    static PlayingState mInstance;
};

////////////////////////////////////////////////////////////////////////////////
/// State class when player is on pause.
class DBITE_PLAYER_API PausedState
        : public DbtPlyEngineState
{
public:
    /// @todo Documentation
    virtual void play(DbtPlyEngine & engine);
    /// @todo Documentation
    virtual void stop(DbtPlyEngine & engine);
    
    /// @todo Documentation
    virtual QString toString() const;
    /// @todo Documentation
    static DbtPlyEngineState * getInstance();

private:
    PausedState();
    static PausedState mInstance;
};

////////////////////////////////////////////////////////////////////////////////
/// State class when player is stopped.
class DBITE_PLAYER_API StoppedState
        : public DbtPlyEngineState
{
public:
    /// @todo Documentation
    virtual void play(DbtPlyEngine & engine);
    
    /// @todo Documentation
    virtual QString toString() const;
    /// @todo Documentation
    static DbtPlyEngineState * getInstance();

private:
    StoppedState();
    static StoppedState mInstance;
};

} // namespace pacpus

#endif // DEF_PACPUS_DBTPLYENGINESTATECHART_H
