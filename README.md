# С++ telegram bot API library

This library provides a pure C++ interface for the Telegram Bot API based on Poco C++ library.


### Usage

```
#include "bot.h"

int main(int argc, char* argv[]) {
    std::string kBotToken = ...
    std::string kBotFirstName = ...;
    Bot bot(kBotToken, kBotFirstName, "information");
    bot.Run();
    return 0;
}
```

Also you should implement message handling logic in function `void ProcessMessage(const Message& message)` in `bot.cpp` and few of the following methods depending on what types of messages you want to handle:

```
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
```


### Features
This library provides base classes for telegram API and methods to use them in message handling loop:
 - Chat
 - Message
 - Sticker
 - Update
 - User  

but you can easily add other in `bot_api.h` by analogue.
