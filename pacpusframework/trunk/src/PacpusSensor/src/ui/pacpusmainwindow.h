// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @date    June, 2006
/// @version $Id: pacpusmainwindow.h 91 2013-05-19 10:32:48Z gdherbom $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef PACPUSMAINWINDOW_H
#define PACPUSMAINWINDOW_H

#include <qcheckbox.h>
#include <qdialog.h>
#include <qtimer.h>

#include <Pacpus/kernel/ComponentManager.h>
#include <Pacpus/kernel/XmlConfigFile.h>
#include "ui_pacpusmainwindow.h"

class QGroupBox;
class QVBoxLayout;

namespace pacpus {

/// Overloaded class of the QCheckBox in order to get the text
/// of the button when it is clicked.
class ComponentCheckBox
        : public QCheckBox
{ 
    Q_OBJECT

public:
    /// @todo Documentation
    ComponentCheckBox(const QString & text, QWidget * parent, const char * /*name*/ = 0 )
        : QCheckBox(text, parent)
    {
        connect(this, SIGNAL(toggled(bool)),
                SLOT(change(bool)));
    }

    /// @todo Documentation
    ~ComponentCheckBox()
    {
    }

public Q_SLOTS:
    /// @todo Documentation
    void change(bool state)
    {
        if (state) {
            Q_EMIT activate(text());
        } else {
            Q_EMIT deactivate(text());
        }
    }

Q_SIGNALS:
    /// @todo Documentation
    void activate(QString);
    /// @todo Documentation
    void deactivate(QString);
};

////////////////////////////////////////////////////////////////////////////////
/// @todo Documentation
class PacpusMainWindow
        : public QDialog
        , public Ui::PacpusMainWindow
{
    Q_OBJECT

public:
    /// Constructor
    PacpusMainWindow(QWidget * parent = 0);
    /// Destructor
	~PacpusMainWindow();

public Q_SLOTS:
    /// slot
    /// Starts all the components and checks all the checkbox
    void startAcquisition();
    /// slot
    /// called when a checkbox is checked 
    /// start the corresponding component 
    /// start the monitoring loop if not running 
    void startComponent(QString component);
    
    /// slot
    /// Stops all the components and unchecks all the checkbox
    void stopAcquisition();
    /// slot
    /// called when a checkbox is unchecked
    /// stop the corresponding component
    /// eventually stop the monitoring loop if any other component is active
    void stopComponent(QString component);

    /// @todo Documentation
    void monitoring();

private:
    ComponentManager * componentManager_;

    QVBoxLayout * layoutBox_;
    QGroupBox * componentContainer_;
    QButtonGroup * componentGroup_;

    QTimer monitoringTimer_;

    bool continue_;
    unsigned short nbActiveComponent_;
};

} // namespace pacpus

#endif // PACPUSMAINWINDOW_H