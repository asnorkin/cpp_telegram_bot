//
// Created by alexaxnder on 23.11.17.
//

#ifndef TELEGRAM_LOGGER_H
#define TELEGRAM_LOGGER_H


#include <Poco/AutoPtr.h>
#include <Poco/Channel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/PatternFormatter.h>


using Poco::AutoPtr;
using Poco::Channel;
using Poco::ColorConsoleChannel;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;


Logger&
InitLogger(
        const std::string& logger_name,
        Poco::Message::Priority priority
) {
    AutoPtr<ColorConsoleChannel> channel(new ColorConsoleChannel());
    AutoPtr<PatternFormatter> pattern_formatter(new PatternFormatter("%s: %p: %t"));
    AutoPtr<FormattingChannel> formatting_channel(new FormattingChannel(pattern_formatter,
                                                                        channel));

    return Logger::create(
            logger_name,
            formatting_channel,
            priority);
}


Logger&
GetLogger(
        const std::string& logger_name = "Log",
        Poco::Message::Priority priority = Poco::Message::PRIO_DEBUG
) {
    static Logger& logger = InitLogger(logger_name, priority);
    return logger;
}


#endif //TELEGRAM_LOGGER_H
