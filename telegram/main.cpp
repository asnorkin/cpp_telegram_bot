#include "bot.h"

#include <ifstream>
#include <string>

std::string ReadToken(const std::string& tokenFile) {
    std::string token;
    std::ifstream inpFile(tokenFile);
    inpFile >> token;
    inpFile.close();
    return token;
}

std::string ReadName(const std::string& nameFile) {
    std::string name;
    std::ifstream inpFile(nameFile);
    inpFile >> name;
    inpFile.close();
    return name;
}

int main(int argc, char* argv[]) {
    const auto& kBotToken = ReadToken("token.txt");
    const auto& kBotFirstName = ReadName("name.txt");
    Bot bot(kBotToken, kBotFirstName, "information");
    bot.Run();
    return 0;
}
