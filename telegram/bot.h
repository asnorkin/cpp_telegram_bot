#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <queue>
#include <unordered_map>
#include "bot_api.h"


class Bot : public TelegramBotAPI {
public:
    Bot() = delete;
    Bot(const std::string& token,
        const std::string& first_name,
        const std::string& log_level = "debug",
        const std::string& server_url = kDefaultTelegramServerUrl);

    void Run();
    void ProcessMessage(const Message& message);
    void ProcessTextMessage(const Message& message);
    void ProcessRandom(const Message& message);
    void ProcessWeather(const Message& message);
    void ProcessStyleguide(const Message& message);
    void ProcessStop(const Message& message);
    void ProcessCrash(const Message& message);
    void ProcessSticker(const Message& message);
    void ProcessGif(const Message& message);
    void ProcessDefault(const Message& message);

//    TODO:
//    void ProcessGif();
//    void ProcessSticker();


    void SaveUpdateId();
    void LoadUpdateId();

private:
    const std::string kUpdateIdFilename = "blablabot_update_id.txt";

    enum class TextCommands {
        Random,
        Weather,
        Styleguide,
        Stop,
        Crash,
        Sticker,
        Gif,
        Default
    };

    const std::unordered_map<std::string, TextCommands> kTextCommands = {
            {"/random", TextCommands::Random},
            {"/weather", TextCommands::Weather},
            {"/styleguide", TextCommands::Styleguide},
            {"/stop", TextCommands::Stop},
            {"/crash", TextCommands::Crash},
            {"/sticker", TextCommands::Sticker},
            {"/gif", TextCommands::Gif}
    };

    int32_t kTimeout = 30;
    int32_t update_id_;
    std::queue<Update> updates_;
};

#endif //TELEGRAM_BOT_H
