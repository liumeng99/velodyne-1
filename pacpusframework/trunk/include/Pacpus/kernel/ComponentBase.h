// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Gerald Dhermobez <firstname.surname@utc.fr>
/// @author  Marek Kurdej <firstname.surname@utc.fr>
/// @author  Samuel Gosselin <firstname.surname@utc.fr>
/// @date    February, 2006
/// @version $Id: ComponentBase.h 116 2013-06-25 11:44:25Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Generic ComponentBase class. This is an abstract class.
///
/// Detailed description.
/// @todo        - see if some methods can be private with ComponentManager
///              friendship
///              - include the copy of Xml node in param here
///              - see if there is a possibility to avoid the constraint
///              on parameters in the constructor of derived class

#ifndef DEF_PACPUS_COMPONENTBASE_H
#define DEF_PACPUS_COMPONENTBASE_H

#include <Pacpus/kernel/ComponentManager.h>
#include <Pacpus/kernel/PacpusLibConfig.h>
#include <Pacpus/kernel/XmlComponentConfig.h>

#include <QString>

namespace pacpus {

// Forward declarations.
class ComponentManager;

/// Base class of a Pacpus component.
class PACPUSLIB_API ComponentBase
{
    friend class ComponentManager;

public:
    /// Enumeration of the state that can take a component, the three last states suppose
    /// that the component is started.
    enum COMPONENT_STATE
    {
        STOPPED,
        NOT_MONITORED,
        MONITOR_OK,
        MONITOR_NOK
    };

    /// Resulting state of a component after its configuration.
    enum COMPONENT_CONFIGURATION
    {
        CONFIGURED_OK,
        NOT_CONFIGURED,
        CONFIGURATION_DELAYED,
        CONFIGURED_FAILED
    };

    /// Ctor of ComponentBase.
    /// @param name Name of your component.
    ComponentBase(const QString & name);

    /// Dtor of ComponentBase.
    virtual ~ComponentBase();

    /// @returns Value of the current state.
    COMPONENT_STATE getState();

    /// Checks whether the component if configurer or not.
    /// @returns @b true if the component is configured, @b false otherwise.
    bool isConfigured() const;

protected:
    /// Change the state of the component.
    /// @param state New component state.
    void setState(COMPONENT_STATE state);

    /// Called when the component starts, you must override this function.
    virtual void startActivity() = 0;

    /// Called when the component stops, you must override this function.
    virtual void stopActivity() = 0;

    /// Called by the ComponentManager, it configure the component thanks a XML node.
    /// @param config Component's XML node.
    /// @returns State of the configuration.
    /// @todo FIXME: 'config' should be const, but we can't change the prototype without breaking old stuff.
    virtual COMPONENT_CONFIGURATION configureComponent(XmlComponentConfig config) = 0;

protected:
    /// The XML node that is got in the configureComponent method
    XmlComponentConfig param;

    /// the name of the component. It is this one in the XML config file
    QString componentName;

    /// is the component is recording data?
    bool recording;

    /// provided for compatibility with old DBITE framework
    bool THREAD_ALIVE;

    /// is the component active?
    bool mIsActive;

    /// a pointer to the manager of components
    ComponentManager * mgr;

private:
    /// called by the ComponentManager to start the component
    int startComponent();

    /// called by the ComponentManager to stop the component
    int stopComponent();

    /// store the state of the component
    COMPONENT_STATE componentState_;

    /// is the component configured (ie configureComponent method was called)
    COMPONENT_CONFIGURATION configuration_;
};

} // namespace pacpus

#endif // DEF_PACPUS_COMPONENTBASE_H