#include "bot.h"

constexpr auto kBotToken = "489689257:AAE35Lj-cOSGpyqq8szl1J6A5VkGY6Arb54";
constexpr auto kBotFirstName = "blablabot";


int main(int argc, char* argv[]) {
    Bot bot(kBotToken, kBotFirstName, "information");
    bot.Run();
    return 0;
}