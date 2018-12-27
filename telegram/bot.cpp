#include "bot.h"

#include <Poco/Net/NetException.h>

#include <fstream>
#include <random>


Bot::Bot(
        const std::string& token,
        const std::string& first_name,
        const std::string& log_level,
        const std::string& server_url
):
        TelegramBotAPI(token, first_name, log_level, server_url)
{
    update_id_ = 0;
    updates_ = {};
}

void Bot::Run() {
    LoadUpdateId();
    InitSession();
    CheckBotInfo();

    try {
        while (true) {
            auto updates = GetUpdates(update_id_, kTimeout);
            for (const auto& upd : updates) {
                update_id_ = upd.update_id + 1;
                if (upd.message) {
                    ProcessMessage(*upd.message);
                }
            }
        }

    } catch (Poco::Net::ConnectionAbortedException& e) {
        SaveUpdateId();
        AbortSession();

    } catch (Poco::Net::ConnectionResetException& e) {
        SaveUpdateId();
        CloseSession();

    } catch (Poco::Exception& e) {
        log().error(e.displayText());
        SaveUpdateId();
        CloseSession();
        throw;

    } catch (std::exception& e) {
        log().error(e.what());
        SaveUpdateId();
        CloseSession();
        throw;
    }
}

void Bot::ProcessMessage(const Message& message) {
    if (message.text != nullptr) {
        return ProcessTextMessage(message);
    }
}

void Bot::ProcessTextMessage(const Message& message) {
    auto cmd = TextCommands::Default;
    try {
        cmd = kTextCommands.at(*message.text);

    } catch(std::out_of_range& e) {
        //  Bot get non handled command
        //  Therefore cmd is empty string and will go
        //  into default case in switch below
    }

    switch (cmd) {

        case TextCommands::Random:
            return ProcessRandom(message);

        case TextCommands::Weather:
            return ProcessWeather(message);

        case TextCommands::Styleguide:
            return ProcessStyleguide(message);

        case TextCommands::Stop:
            return ProcessStop(message);

        case TextCommands::Crash:
            return ProcessCrash(message);

        case TextCommands::Sticker:
            return ProcessSticker(message);

        case TextCommands::Gif:
            return ProcessGif(message);

        default:
            return ProcessDefault(message);
    }
}

void Bot::ProcessRandom(const Message& message) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int32_t> distribution;
    int32_t rand_number = distribution(generator);

    SendMessage(message.chat.id, std::to_string(rand_number));
}

void Bot::ProcessWeather(const Message& message) {
    SendMessage(message.chat.id, "Winter Is Coming.");
}

void Bot::ProcessStyleguide(const Message& message) {
    SendMessage(message.chat.id, "//  TODO: funny joke");
}

void Bot::ProcessStop(const Message& message) {
    throw Poco::Net::ConnectionResetException("'/stop' command received");
}

void Bot::ProcessCrash(const Message& message) {
    throw Poco::Net::ConnectionAbortedException("'/crash' command received");
}

void Bot::ProcessSticker(const Message& message) {
    SendSticker(message.chat.id, "CAADAgADegADECECEAACxyOkybkFAg");
}

void Bot::ProcessGif(const Message& message) {
    SendDocument(message.chat.id, "CgADAgADjQADJ7MRSM0LdfDklYBfAg");
}

void Bot::ProcessDefault(const Message& message) {
    SendMessage(message.chat.id, *message.text + " blablabla...");
}

void Bot::SaveUpdateId() {
    std::ofstream fout(kUpdateIdFilename, std::ios_base::trunc);
    if (!fout.is_open()) {
        throw Poco::OpenFileException(
                "Failed to open file to save update id ('" +
                kUpdateIdFilename + "'). Error: " + strerror(errno));
    }

    fout << std::to_string(update_id_);
    fout.close();
}

void Bot::LoadUpdateId() {
    std::ifstream fin(kUpdateIdFilename);
    if (!fin.is_open()) {
        if (errno == ENOENT) {  //  No such file
            update_id_ = 0;
            return;
        }

        throw Poco::OpenFileException(
                "Failed to open file to load update id ('" +
                kUpdateIdFilename + "'). Error: " + strerror(errno));
    }

    fin >> update_id_;
    fin.close();
}
