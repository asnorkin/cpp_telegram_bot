#ifndef TELEGRAM_BOT_API_H
#define TELEGRAM_BOT_API_H


#include <memory>
#include <optional>
#include <vector>
#include <Poco/Logger.h>


using Poco::Logger;


constexpr auto kDefaultTelegramServerUrl = "https://api.telegram.org/";


struct Chat;
struct Message;
struct Sticker;
struct Update;
struct User;


//  TODO: create structures
struct ChatPhoto {};
struct MessageEntity {};
struct Audio {};
struct Document {};
struct Game {};
struct PhotoSize {};
struct Video {};
struct Voice {};
struct VideoNote {};
struct Contact {};
struct Location {};
struct Venue {};
struct True {};
struct Invoice {};
struct SuccessfulPayment {};
struct InlineQuery {};
struct ChosenInlineResult {};
struct CallbackQuery {};
struct ShippingQuery {};
struct PreCheckoutQuery {};
struct MaskPosition {};


struct Chat {
    int64_t id;
    std::string type;
    std::unique_ptr<std::string> title;
    std::unique_ptr<std::string> username;
    std::unique_ptr<std::string> first_name;
    std::unique_ptr<std::string> last_name;
    std::unique_ptr<bool> all_members_are_admins;
    std::unique_ptr<ChatPhoto> photo;
    std::unique_ptr<std::string> description;
    std::unique_ptr<std::string> invite_link;
    std::unique_ptr<Message> pinned_message;
    std::unique_ptr<std::string> sticker_set_name;
    std::unique_ptr<bool> can_set_sticker_set;
};


struct Message {
    int32_t message_id;
    std::unique_ptr<User> from;
    int32_t date;
    Chat chat;
    std::unique_ptr<User> forward_from;
    std::unique_ptr<Chat> forward_from_chat;
    std::unique_ptr<int32_t> forward_from_message_id;
    std::unique_ptr<std::string> forward_signature;
    std::unique_ptr<int32_t> forward_date;
    std::unique_ptr<Message> reply_to_message;
    std::unique_ptr<int32_t> edit_date;
    std::unique_ptr<std::string> media_group_id;
    std::unique_ptr<std::string> author_signature;
    std::unique_ptr<std::string> text;
    std::unique_ptr<std::vector<MessageEntity>> entities;
    std::unique_ptr<std::vector<MessageEntity>> caption_entities;
    std::unique_ptr<Audio> audio;
    std::unique_ptr<Document> document;
    std::unique_ptr<Game> game;
    std::unique_ptr<std::vector<PhotoSize>> photo;
    std::unique_ptr<Sticker> sticker;
    std::unique_ptr<Video> video;
    std::unique_ptr<Voice> voice;
    std::unique_ptr<VideoNote> video_note;
    std::unique_ptr<std::string> caption;
    std::unique_ptr<Contact> contact;
    std::unique_ptr<Location> location;
    std::unique_ptr<Venue> venue;
    std::unique_ptr<std::vector<User>> new_chat_members;
    std::unique_ptr<User> left_chat_member;
    std::unique_ptr<std::string> new_chat_title;
    std::unique_ptr<std::vector<PhotoSize>> new_chat_photo;
    std::unique_ptr<True> delete_chat_photo;
    std::unique_ptr<True> group_chat_created;
    std::unique_ptr<True> supergroup_chat_created;
    std::unique_ptr<True> channel_chat_created;
    std::unique_ptr<int32_t> migrate_to_chat_id;
    std::unique_ptr<int32_t> migrate_from_chat_id;
    std::unique_ptr<Message> pinned_message;
    std::unique_ptr<Invoice> invoice;
    std::unique_ptr<SuccessfulPayment> successful_payment;
};


struct Sticker {
    std::string file_id;
    int32_t width;
    int32_t height;
    std::unique_ptr<PhotoSize> thumb;
    std::unique_ptr<std::string> emoji;
    std::unique_ptr<std::string> set_name;
    std::unique_ptr<MaskPosition> mask_position;
    std::unique_ptr<int32_t> file_size;
};


struct Update {
    int32_t update_id;
    std::unique_ptr<Message> message;
    std::unique_ptr<Message> edited_message;
    std::unique_ptr<Message> channel_post;
    std::unique_ptr<Message> edited_channel_post;
    std::unique_ptr<InlineQuery> inline_query;
    std::unique_ptr<ChosenInlineResult> chosen_inline_result;
    std::unique_ptr<CallbackQuery> callback_query;
    std::unique_ptr<ShippingQuery> shipping_query;
    std::unique_ptr<PreCheckoutQuery> pre_checkout_query;
};


struct User {
    int32_t id;
    bool is_bot;
    std::string first_name;

    std::unique_ptr<std::string> last_name;
    std::unique_ptr<std::string> username;
    std::unique_ptr<std::string> language_code;

    std::string GetInfo();
};


class TelegramBotAPI {
public:
    TelegramBotAPI(const std::string& token,
                   const std::string& first_name,
                   const std::string& log_level,
                   const std::string& server_url);
    ~TelegramBotAPI();

    void InitSession();
    void CloseSession();
    void AbortSession();

    void CheckBotInfo();

    User GetMe();
    std::vector<Update> GetUpdatesWithTimeout(int32_t timeout);
    std::vector<Update> GetUpdatesWithOffset(int32_t offset);
    std::vector<Update> GetUpdates(
            std::optional<int32_t> offset = std::nullopt,
            std::optional<int32_t> timeout = std::nullopt);

    void SendMessage(int32_t chat_id, const std::string&);
    void SendMessage(int32_t chat_id, const std::string&, int32_t);

    void SendSticker(int32_t chat_id, const std::string& file_id);
    void SendDocument(int32_t chat_id, const std::string &document);

    Logger& log();

private:
    class TelegramBotAPIImpl;
    std::unique_ptr<TelegramBotAPIImpl> pimpl_;
};


#endif //TELEGRAM_BOT_API_H
