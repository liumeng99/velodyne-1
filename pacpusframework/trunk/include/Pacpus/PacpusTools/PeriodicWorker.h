// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Samuel Gosselin <firstname.surname@utc.fr>
/// @date    December, 2012
/// @version $Id: PeriodicWorker.h 116 2013-06-25 11:44:25Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Periodic worker base class
///
/// Detailed description.

#ifndef DEF_PACPUS_PERIODIC_WORKER_H
#define DEF_PACPUS_PERIODIC_WORKER_H

// Includes, pacpus.
#include "Pacpus/kernel/PacpusToolsConfig.h"
#include <Pacpus/PacpusTools/AsyncWorkerBase.h>

class QTimer;

namespace pacpus
{

/// @brief A simple base class for periodic worker.
/// 
/// @example
/// To use the PeriodicWorker, simply inherit from this class when creating your worker.
/// ~~~
/// class MyWorker
///     : public PeriodicWorkder
/// {
/// public:
///     void doWork() { std::cout << "Hey, I'm working!" << std::endl; }
/// };
///
/// // Do its work every second.
/// MyWorker worker;
/// worker.startWork(1000);
/// ~~~
class PACPUSTOOLS_API PeriodicWorker
    : public AsyncWorkerBase
{
    Q_OBJECT
public:
    /// Ctor of PeriodicWorker.
    PeriodicWorker();

    /// Dtor of PeriodicWorker.
    virtual ~PeriodicWorker();

    /// Start the periodic worker.
    /// @param msec Period in mseconds.
    void startWork(int msec);

    /// Stop the periodic worker, but do not delete it.
    void stopWork();

    public Q_SLOTS:
        /// Do the work.
        /// This method need to be implemented in the subclasses, it will be called
        /// each time the timer has reached its period.
        virtual void doWork() = 0;

private:
    QTimer * mHeartbeat;
};

} // namespace pacpus

#endif // DEF_PACPUS_PERIODIC_WORKER_H
