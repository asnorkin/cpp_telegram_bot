#include <catch.hpp>

#include "../telegram/fake.h"
#include "../telegram/bot.h"

#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <iostream>


constexpr auto kBotToken = "123";
constexpr auto kBotFirstName = "blablabot";


TEST_CASE("Checking getMe") {

    telegram::FakeServer fake("Single getMe");
    fake.Start();

    Bot bot(kBotToken, kBotFirstName, "debug", fake.GetUrl());
    bot.InitSession();
    try {
        User user = bot.GetMe();

    } catch (Poco::Exception& e) {
        std::cerr << "Exception occured: " << e.displayText() << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception occured: " << e.what() << "\n";
    }

    fake.StopAndCheckExpectations();
}

TEST_CASE("getMe error handling") {

    telegram::FakeServer fake("getMe error handling");
    fake.Start();

    Bot bot(kBotToken, kBotFirstName, "debug", fake.GetUrl());
    bot.InitSession();

    try {
        User user = bot.GetMe();

    } catch (Poco::Net::HTTPException& e) {
        std::cout << "Exception occured as expected\n";

    } catch (Poco::Exception& e) {
        std::cerr << "Exception occured: " << e.displayText() << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception occured: " << e.what() << "\n";
    }

    try {
        User user = bot.GetMe();

    } catch (Poco::RuntimeException& e) {
        std::cout << "Exception occured as expected\n";

    } catch (Poco::Exception& e) {
        std::cerr << "Exception occured: " << e.displayText() << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception occured: " << e.what() << "\n";
    }

    fake.StopAndCheckExpectations();
}

TEST_CASE("Single getUpdates and send messages") {
    telegram::FakeServer fake("Single getUpdates and send messages");
    fake.Start();

    Bot bot(kBotToken, kBotFirstName, "debug", fake.GetUrl());
    bot.InitSession();

    try {
        auto updates = bot.GetUpdates();
        auto first_chat_id = updates[0].message->chat.id;
        bot.SendMessage(first_chat_id, "Hi!");

        auto second_chat_id = updates[1].message->chat.id;
        bot.SendMessage(second_chat_id, "Reply", 2);
        bot.SendMessage(second_chat_id, "Reply", 2);

    } catch (Poco::Exception& e) {
        std::cerr << "Exception occured: " << e.displayText() << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception occured: " << e.what() << "\n";
    }

    fake.StopAndCheckExpectations();
}

TEST_CASE("Handle getUpdates offset") {
    telegram::FakeServer fake("Handle getUpdates offset");
    fake.Start();

    Bot bot(kBotToken, kBotFirstName, "debug", fake.GetUrl());
    bot.InitSession();

    try {
        auto updates = bot.GetUpdatesWithTimeout(5);
        int32_t max_update_id = updates[1].update_id;

        updates = bot.GetUpdates(max_update_id + 1, 5);
        updates = bot.GetUpdates(max_update_id + 1, 5);

    } catch (Poco::Exception& e) {
        std::cerr << "Exception occured: " << e.displayText() << "\n";

    } catch (std::exception& e) {
        std::cerr << "Exception occured: " << e.what() << "\n";
    }

    fake.StopAndCheckExpectations();
}
