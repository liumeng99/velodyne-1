// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @author  Marek Kurdej <firstname.surname@utc.fr>
/// @version    $Id: Log.cpp 91 2013-05-19 10:32:48Z gdherbom $

#include <Pacpus/kernel/Log.h>

#ifdef PACPUS_USE_LOG

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4231 4251)
#endif // _MSC_VER

#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/propertyconfigurator.h>
#include <QString>

#ifdef _MSC_VER
#   pragma warning(pop)
#endif // _MSC_VER

using namespace log4cxx;

namespace pacpus {

static int niftyCounter;

LogConfigurator::LogConfigurator()
{
    using namespace log4cxx;

    if (0 == niftyCounter++) {
        BasicConfigurator::resetConfiguration();

        LayoutPtr lp(new PatternLayout(LOG4CXX_STR("%-6r [%-5p] %30.30c - %m%n")));
        AppenderPtr ap(new ConsoleAppender(lp));
        BasicConfigurator::configure(ap);
        Logger::getRootLogger()->setLevel(log4cxx::Level::getInfo());
    }
}

LogConfigurator::~LogConfigurator()
{
    if (0 == --niftyCounter) {
        // clean up
    }
}

void LogConfigurator::configureLoggerWithFile(const char * configFilename)
{
    BasicConfigurator::resetConfiguration();
    PropertyConfigurator::configure(configFilename);
}

} // namespace pacpus

helpers::CharMessageBuffer & operator<<(helpers::CharMessageBuffer & os, const QString & val)
{
    return os << val.toStdString();
}

helpers::CharMessageBuffer & operator<<(helpers::MessageBuffer & os, const QString & val)
{
    return os << val.toStdString();
}

#else // PACPUS_USE_LOG

namespace pacpus {

LogConfigurator::LogConfigurator()
{
}

LogConfigurator::~LogConfigurator()
{
}

void LogConfigurator::configureLoggerWithFile(const char * /*configFilename*/)
{
}

} // namespace pacpus

#endif // PACPUS_USE_LOG