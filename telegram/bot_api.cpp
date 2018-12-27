#include "bot_api.h"
#include "logger.h"

#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPMessage.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/URI.h>
#include <jsoncpp/json/json.h>

#include <iostream>
#include <optional>
#include <unordered_map>

using Poco::Logger;
using Poco::Net::Context;
using Poco::Net::HTTPException;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::URI;


class TelegramBotAPI::TelegramBotAPIImpl {
public:
    TelegramBotAPIImpl(
            std::string token,
            std::string first_name,
            const std::string& log_level,
            const std::string& server_url
    ):
            token_{std::move(token)},
            first_name_{std::move(first_name)},
            server_url_{server_url},
            log_{GetLogger("BotLog", kLogLevels.at(log_level))}
    {}

    void InitSession();
    void CloseSession();
    void AbortSession();

    void CheckBotInfo();

    User GetMe();

    std::vector<Update> GetUpdates(
            std::optional<int32_t> offset,
            std::optional<int32_t> timeout);

    //  TODO: add reply_markup parameter
    void SendMessage(
            int32_t chat_id,
            const std::string& text,
            std::optional<std::string> parse_mode,
            std::optional<bool> disable_web_page_preview,
            std::optional<bool> disable_notification,
            std::optional<int32_t> reply_to_message_id);

    //  TODO: add other parameters
    void SendSticker(int32_t chat_id, const std::string& file_id);
    void SendDocument(int32_t chat_id, const std::string& document);

    Logger& log() { return log_; }

private:
    const std::unordered_map<std::string, Poco::Message::Priority>
        kLogLevels = {
            {"fatal", Poco::Message::Priority::PRIO_FATAL},
            {"crit", Poco::Message::Priority::PRIO_CRITICAL},
            {"error", Poco::Message::Priority::PRIO_ERROR},
            {"warning", Poco::Message::Priority::PRIO_WARNING},
            {"notice", Poco::Message::Priority::PRIO_NOTICE},
            {"information", Poco::Message::Priority::PRIO_INFORMATION},
            {"debug", Poco::Message::Priority::PRIO_DEBUG},
            {"trace", Poco::Message::Priority::PRIO_TRACE}
    };

    std::istream& GetRequest(const URI& uri);
    std::istream& PostRequest(const URI& uri, const Json::Value& json);
    Json::Value GetJsonFromStream(std::istream& istream);
    URI GetRequestUri(const std::string& request);
    void CheckResponseJson(const Json::Value& json);

    Chat ConvertJsonToChat(const Json::Value& json);
    User ConvertJsonToUser(const Json::Value& json);
    Message ConvertJsonToMessage(const Json::Value& json);
    Update ConvertJsonToUpdate(const Json::Value& json);
    std::vector<Update> ConvertJsonToUpdates(const Json::Value& json);
    Sticker ConvertJsonToSticker(const Json::Value& json);

    int64_t GetIntFromJson(const Json::Value& json, std::string value_name);
    std::string GetStringFromJson(const Json::Value& json, std::string value_name);
    bool GetBoolFromJson(const Json::Value& json, std::string value_name);
    double GetDoubleFromJson(const Json::Value& json, std::string value_name);

    std::unique_ptr<int32_t> OptionalGetIntFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<std::string> OptionalGetStringFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<bool> OptionalGetBoolFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<double> OptionalGetDoubleFromJson(const Json::Value& json, std::string value_name);

    Chat GetChatFromJson(const Json::Value& json, std::string value_name);
    User GetUserFromJson(const Json::Value& json, std::string value_name);
    Message GetMessageFromJson(const Json::Value& json, std::string value_name);
    Update GetUpdateFromJson(const Json::Value& json, std::string value_name);
    Sticker GetStickerFromJson(const Json::Value& json, std::string value_name);

    std::unique_ptr<Chat> OptionalGetChatFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<User> OptionalGetUserFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<Message> OptionalGetMessageFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<Update> OptionalGetUpdateFromJson(const Json::Value& json, std::string value_name);
    std::unique_ptr<Sticker> OptionalGetStickerFromJson(const Json::Value& json, std::string value_name);

    void CheckJsonValue(
            const Json::Value& json,
            std::string value_name,
            Json::ValueType value_type);

    std::string token_;
    std::string first_name_;
    std::string server_url_;

    std::unique_ptr<HTTPClientSession> psession_;
    Logger& log_;
};

void
TelegramBotAPI::TelegramBotAPIImpl
::InitSession() {
    log_.information("Initializing session..");

    auto host_uri = URI(server_url_);
    if (server_url_.substr(0, 5) == "https") {
        psession_ = std::make_unique<HTTPSClientSession>(
                host_uri.getHost(),
                host_uri.getPort());

    } else {
        psession_ = std::make_unique<HTTPClientSession>(
                host_uri.getHost(),
                host_uri.getPort());
    }

    log_.information("Session initialization finished");
}

void
TelegramBotAPI::TelegramBotAPIImpl
::CloseSession() {
    log_.information("Closing session..");

    psession_.reset();

    log_.information("Session closing finished");
}

void
TelegramBotAPI::TelegramBotAPIImpl
::AbortSession() {
    log_.information("Aborting session..");

    psession_->abort();

    log_.information("Session aborting finished");
}

void
TelegramBotAPI::TelegramBotAPIImpl
::CheckBotInfo() {
    log_.information("Checking bot info..");

    auto user = GetMe();
    if (!user.is_bot || user.first_name != first_name_) {
        std::string err_msg = "Wrong bot info: " + user.GetInfo();
        log_.error(err_msg);
        throw Poco::LogicException(err_msg);
    }

    log_.information("Checking bot info finished.");
}

User
TelegramBotAPI::TelegramBotAPIImpl
::GetMe() {
    log_.information("Sending GetMe..");

    auto uri = GetRequestUri("getMe");
    std::istream& response_stream = GetRequest(uri);
    auto response_json = GetJsonFromStream(response_stream);
    CheckResponseJson(response_json);
    auto user = ConvertJsonToUser(response_json["result"]);

    log_.debug("Response json got:\n" + response_json.toStyledString());
    log_.information("Sending GetMe finished");
    return user;
}

std::string
BuildGetUpdatesRequestString(
        std::optional<int32_t> offset,
        std::optional<int32_t> timeout
) {
    if (offset && timeout) {
        return "getUpdates?offset=" + std::to_string(offset.value())
               + "&timeout=" + std::to_string(timeout.value());

    } else if (offset) {
        return "getUpdates?offset=" + std::to_string(offset.value());

    } else if (timeout) {
        return "getUpdates?timeout=" + std::to_string(timeout.value());

    } else {
        return "getUpdates";
    }
}

std::vector<Update>
TelegramBotAPI::TelegramBotAPIImpl
::GetUpdates(
        std::optional<int32_t> offset,
        std::optional<int32_t> timeout
) {
    log_.information("Getting updates..");

    std::string req_str = BuildGetUpdatesRequestString(offset, timeout);

    auto uri = GetRequestUri(req_str);
    std::istream& response_stream = GetRequest(uri);
    auto response_json = GetJsonFromStream(response_stream);
    CheckResponseJson(response_json);
    auto updates = ConvertJsonToUpdates(response_json["result"]);

    log_.debug("Response json got:\n" + response_json.toStyledString());
    log_.information("Getting updates finished. Got "
                     + std::to_string(updates.size()) + " updates.");
    return updates;
}

Json::Value
CreateJsonForSendMessage(
        int32_t chat_id,
        const std::string& text,
        std::optional<std::string> parse_mode,
        std::optional<bool> disable_web_page_preview,
        std::optional<bool> disable_notification,
        std::optional<int32_t> reply_to_message_id
) {
    Json::Value json;
    json["chat_id"] = chat_id;
    json["text"] = text;
    if (parse_mode) {
        json["parse_mode"] = parse_mode.value();
    }

    if (disable_web_page_preview) {
        json["disable_web_page_preview"] = disable_web_page_preview.value();
    }

    if (disable_notification) {
        json["disable_notification"] = disable_notification.value();
    }

    if (reply_to_message_id) {
        json["reply_to_message_id"] = reply_to_message_id.value();
    }

    return json;
}

void
TelegramBotAPI::TelegramBotAPIImpl
::SendMessage(
        int32_t chat_id,
        const std::string& text,
        std::optional<std::string> parse_mode,
        std::optional<bool> disable_web_page_preview,
        std::optional<bool> disable_notification,
        std::optional<int32_t> reply_to_message_id
) {
    log_.information("Sending message..");

    auto uri = GetRequestUri("sendMessage");
    Json::Value json = CreateJsonForSendMessage(
            chat_id,
            text,
            parse_mode,
            disable_web_page_preview,
            disable_notification,
            reply_to_message_id);

    std::istream& response_stream = PostRequest(uri, json);
    auto response_json = GetJsonFromStream(response_stream);
    CheckResponseJson(response_json);

    log_.debug("Response json got:\n" + response_json.toStyledString());
    log_.information("Sending message finished");
}

void
TelegramBotAPI::TelegramBotAPIImpl
::SendSticker(
        int32_t chat_id,
        const std::string& file_id
) {
    log_.information("Sending sticker..");
    log_.debug("Sticker file id: " + file_id);

    Json::Value json;
    json["chat_id"] = chat_id;
    json["sticker"] = file_id;

    auto uri = GetRequestUri("sendSticker");
    std::istream& response_stream = PostRequest(uri, json);
    auto response_json = GetJsonFromStream(response_stream);
    CheckResponseJson(response_json);

    log_.debug("Response json got:\n" + response_json.toStyledString());
    log_.information("Sending sticker finished");
}

void
TelegramBotAPI::TelegramBotAPIImpl
::SendDocument(
        int32_t chat_id,
        const std::string& document
) {
    log_.information("Sending document..");
    log_.debug("Document: " + document);

    Json::Value json;
    json["chat_id"] = chat_id;
    json["document"] = document;

    auto uri = GetRequestUri("sendDocument");
    std::istream& response_stream = PostRequest(uri, json);
    auto response_json = GetJsonFromStream(response_stream);
    CheckResponseJson(response_json);

    log_.debug("Response json got:\n" + response_json.toStyledString());
    log_.information("Sending sticker finished");
}

std::istream&
TelegramBotAPI::TelegramBotAPIImpl
::GetRequest(
        const URI& uri
) {
    log_.debug("GET request on uri '" + uri.toString() + "'");

    HTTPRequest request(
            HTTPRequest::HTTP_GET,
            uri.getPathAndQuery(),
            HTTPMessage::HTTP_1_1);
    auto tmp = request.getURI();
    psession_->sendRequest(request);

    HTTPResponse response;
    std::istream& response_stream = psession_->receiveResponse(response);
    if (response.getStatus() != HTTPResponse::HTTP_OK) {
        std::string err_msg = "GET request with uri '" +
                uri.toString() + "' got response status: " +
                std::to_string(response.getStatus()) + ", reason: " +
                response.getReason() + ". Expected " +
                std::to_string(HTTPResponse::HTTP_OK);

        log_.error(err_msg);
        throw Poco::Net::HTTPException(err_msg);
    }

    return response_stream;
}

std::istream&
TelegramBotAPI::TelegramBotAPIImpl
::PostRequest(
        const URI& uri,
        const Json::Value& json
) {
    auto json_str = json.toStyledString();
    log_.debug("POST request on uri '" + uri.toString() + "' with json:" + json_str);
    HTTPRequest request(HTTPRequest::HTTP_POST,
                        uri.getPathAndQuery(),
                        HTTPMessage::HTTP_1_1);

    request.setContentType("application/json");
    request.setContentLength(json_str.size());
    std::ostream& request_stream = psession_->sendRequest(request);
    request_stream << json_str;

    HTTPResponse response;
    std::istream& response_stream = psession_->receiveResponse(response);
    if (response.getStatus() != HTTPResponse::HTTP_OK) {
        std::string err_msg = "POST request with uri '" + uri.toString() +
                "' and json value:\n" + json_str + "\ngot response status: " +
                std::to_string(response.getStatus()) + ", reason: " +
                response.getReason() + ". Expected " +
                std::to_string(HTTPResponse::HTTP_OK);

        log_.error(err_msg);
        throw Poco::Net::HTTPException(err_msg);
    }

    return response_stream;
}

Json::Value
TelegramBotAPI::TelegramBotAPIImpl
::GetJsonFromStream(
        std::istream& istream
) {
    Json::Value result;
    Json::CharReaderBuilder reader_builder;
    std::string errs;
    if (!Json::parseFromStream(reader_builder, istream, &result, &errs)) {
        std::string err_msg = "Json parsing error. Errors: " + errs;
        throw Poco::RuntimeException(err_msg);
    }

    return result;
}

URI
TelegramBotAPI::TelegramBotAPIImpl
::GetRequestUri(
        const std::string& request
) {
    return URI(server_url_ + "bot" + token_ + "/" + request);
}

void
TelegramBotAPI::TelegramBotAPIImpl
::CheckResponseJson(
        const Json::Value& json
) {
    if (!json.isMember("ok")) {
        std::string err_msg = "Wrong response json format."
                " There is no 'ok' field. Fields: ";
        for (const auto& member : json.getMemberNames()) {
            err_msg += member + ", ";
        }

        log_.error(err_msg);
        log_.debug(json.toStyledString());
        throw Poco::DataFormatException(err_msg);
    }

    if (json["ok"].type() != Json::ValueType::booleanValue) {
        std::string err_msg = "Response json has wrong 'ok' type: " +
                              std::to_string(Json::ValueType::booleanValue) +
                              ". Expected: bool.";

        log_.error(err_msg);
        log_.debug(json.toStyledString());
        throw Poco::DataFormatException(err_msg);
    }

    if (!json["ok"].asBool()) {
        std::string err_msg = "Response json has false 'ok' field. "
                "Expected true.";

        log_.error(err_msg);
        log_.debug(json.toStyledString());
        throw Poco::RuntimeException(err_msg);
    }

    if (!json.isMember("result")) {
        std::string err_msg = "Wrong response json format."
                " There is no 'result' field. Fields: ";
        for (const auto& member : json.getMemberNames()) {
            err_msg += member + ", ";
        }

        log_.error(err_msg);
        log_.debug(json.toStyledString());
        throw Poco::DataFormatException(err_msg);
    }
}

void
TelegramBotAPI::TelegramBotAPIImpl
::CheckJsonValue(
        const Json::Value &json,
        std::string value_name,
        Json::ValueType value_type
) {
    if (!json.isMember(value_name)) {
        std::string err_msg = "Json value has no field '" +
                              value_name + "'";

        log_.error(err_msg);
        log_.debug("Json value:\n" + json.toStyledString());
        throw Poco::DataFormatException(err_msg);
    }

    if (json[value_name].type() != value_type) {
        std::string err_msg = "Json value field '" +
                value_name + "' type (" + std::to_string(json.type()) +
                ") isn't match. Expected " + std::to_string(value_type);

        log_.error(err_msg);
        throw Poco::DataFormatException(err_msg);
    }
}

int64_t TelegramBotAPI::TelegramBotAPIImpl
::GetIntFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::intValue);
    return json[value_name].asInt64();
}

std::string TelegramBotAPI::TelegramBotAPIImpl
::GetStringFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::stringValue);
    return json[value_name].asString();
}

bool TelegramBotAPI::TelegramBotAPIImpl
::GetBoolFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::booleanValue);
    return json[value_name].asBool();
}

double TelegramBotAPI::TelegramBotAPIImpl
::GetDoubleFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::realValue);
    return json[value_name].asDouble();
}

std::unique_ptr<int32_t>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetIntFromJson(
        const Json::Value &json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<int32_t>(
            GetIntFromJson(json, value_name));
}

std::unique_ptr<std::string>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetStringFromJson(
        const Json::Value &json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<std::string>(
            GetStringFromJson(json, value_name));
}

std::unique_ptr<bool>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetBoolFromJson(
        const Json::Value &json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<bool>(
            GetBoolFromJson(json, value_name));
}

std::unique_ptr<double>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetDoubleFromJson(
        const Json::Value &json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<double>(
            GetDoubleFromJson(json, value_name));
}

Chat
TelegramBotAPI::TelegramBotAPIImpl
::GetChatFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::objectValue);
    return ConvertJsonToChat(json[value_name]);
}

User
TelegramBotAPI::TelegramBotAPIImpl
::GetUserFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::objectValue);
    return ConvertJsonToUser(json[value_name]);
}

Message
TelegramBotAPI::TelegramBotAPIImpl
::GetMessageFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::objectValue);
    return ConvertJsonToMessage(json[value_name]);
}

Update
TelegramBotAPI::TelegramBotAPIImpl
::GetUpdateFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::objectValue);
    return ConvertJsonToUpdate(json[value_name]);
}

Sticker
TelegramBotAPI::TelegramBotAPIImpl
::GetStickerFromJson(
        const Json::Value &json,
        std::string value_name
) {
    CheckJsonValue(json, value_name, Json::objectValue);
    return Sticker();
}

std::unique_ptr<Chat>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetChatFromJson(
        const Json::Value& json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<Chat>(
            GetChatFromJson(json, value_name));
}

std::unique_ptr<User>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetUserFromJson(
        const Json::Value& json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<User>(
            GetUserFromJson(json, value_name));
}

std::unique_ptr<Message>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetMessageFromJson(
        const Json::Value& json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<Message>(
            GetMessageFromJson(json, value_name));
}

std::unique_ptr<Update>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetUpdateFromJson(
        const Json::Value& json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<Update>(
            GetUpdateFromJson(json, value_name));
}

std::unique_ptr<Sticker>
TelegramBotAPI::TelegramBotAPIImpl
::OptionalGetStickerFromJson(
        const Json::Value &json,
        std::string value_name
) {
    if (!json.isMember(value_name)) {
        return {nullptr};
    }

    return std::make_unique<Sticker>(
            GetStickerFromJson(json, value_name));
}

Chat
TelegramBotAPI::TelegramBotAPIImpl::
ConvertJsonToChat(
        const Json::Value& json
) {
    Chat chat;

    chat.id = GetIntFromJson(json, "id");
    chat.type = GetStringFromJson(json, "type");
    chat.title = OptionalGetStringFromJson(json, "title");
    chat.username = OptionalGetStringFromJson(json, "username");
    chat.first_name = OptionalGetStringFromJson(json, "first_name");
    chat.last_name = OptionalGetStringFromJson(json, "last_name");
    chat.all_members_are_admins = OptionalGetBoolFromJson(
            json, "all_members_are_administrators");

    //chat.photo =

    chat.description = OptionalGetStringFromJson(json, "description");
    chat.invite_link = OptionalGetStringFromJson(json, "invite_link");
    chat.pinned_message = OptionalGetMessageFromJson(json, "pinned_message");
    chat.sticker_set_name = OptionalGetStringFromJson(json, "sticker_set_name");
    chat.can_set_sticker_set = OptionalGetBoolFromJson(json, "can_sticker_set_name");

    return chat;
}

User
TelegramBotAPI::TelegramBotAPIImpl::
ConvertJsonToUser(
        const Json::Value& json
) {
    User user;

    user.id = GetIntFromJson(json, "id");
    user.is_bot = GetBoolFromJson(json, "is_bot");
    user.first_name = GetStringFromJson(json, "first_name");
    user.last_name = OptionalGetStringFromJson(json, "last_name");
    user.username = OptionalGetStringFromJson(json, "username");
    user.language_code = OptionalGetStringFromJson(json, "language_code");

    return user;
}

Message
TelegramBotAPI::TelegramBotAPIImpl::
ConvertJsonToMessage(
        const Json::Value& json
) {
    Message message;

    message.message_id = GetIntFromJson(json, "message_id");
    message.from = OptionalGetUserFromJson(json, "from");
    message.date = GetIntFromJson(json, "date");
    message.chat = GetChatFromJson(json, "chat");
    message.forward_from = OptionalGetUserFromJson(json, "forward_from");
    message.forward_from_chat = OptionalGetChatFromJson(json, "forward_from_chat");
    message.forward_from_message_id = OptionalGetIntFromJson(json, "forward_from_message_id");
    message.forward_signature = OptionalGetStringFromJson(json, "forward_signature");
    message.forward_date = OptionalGetIntFromJson(json, "forward_date");
    message.reply_to_message = OptionalGetMessageFromJson(json, "reply_to_message");
    message.edit_date = OptionalGetIntFromJson(json, "edit_date");
    message.media_group_id = OptionalGetStringFromJson(json, "media_group_id");
    message.author_signature = OptionalGetStringFromJson(json, "author_signature");
    message.text = OptionalGetStringFromJson(json, "text");

    //  message.entities =
    //  message.caption_entities =
    //  message.audio =
    //  message.document =
    //  message.game =
    //  message.photo =

    message.sticker = OptionalGetStickerFromJson(json, "sticker");

    //  message.video =
    //  message.voice =
    //  message.video_note =

    message.caption = OptionalGetStringFromJson(json, "caption");

    //  message.contact =
    //  message.location =
    //  message.venue =
    //  message.new_chat_members =

    message.left_chat_member = OptionalGetUserFromJson(json, "left_chat_member");
    message.new_chat_title = OptionalGetStringFromJson(json, "new_chat_title");

    //  message.new_chat_photo =
    //  message.delete_chat_photo =
    //  message.group_chat_created =
    //  message.supergroup_chat_created =
    //  message.channel_chat_created =

    message.migrate_to_chat_id = OptionalGetIntFromJson(json, "migrate_to_chat_id");
    message.migrate_from_chat_id = OptionalGetIntFromJson(json, "migrate_from_chat_id");
    message.pinned_message = OptionalGetMessageFromJson(json, "pinned_message");

    //  message.invoice =
    //  message.successful_payment =

    return message;
}

Sticker
TelegramBotAPI::TelegramBotAPIImpl
::ConvertJsonToSticker(
        const Json::Value &json
) {
    Sticker sticker;

    sticker.file_id = GetStringFromJson(json, "file_id");
    sticker.width = GetIntFromJson(json, "width");
    sticker.height = GetIntFromJson(json, "height");

    //  sticker.thumb

    sticker.emoji = OptionalGetStringFromJson(json, "emoji");
    sticker.emoji = OptionalGetStringFromJson(json, "set_name");

    //  sticker.mask_position

    sticker.file_size = OptionalGetIntFromJson(json, "file_size");

    return sticker;
}

Update
TelegramBotAPI::TelegramBotAPIImpl::
ConvertJsonToUpdate(
        const Json::Value& json
) {
    Update update;

    update.update_id = GetIntFromJson(json, "update_id");
    update.message = OptionalGetMessageFromJson(json, "message");
    update.edited_message = OptionalGetMessageFromJson(json, "edited_message");
    update.channel_post = OptionalGetMessageFromJson(json, "channel_post");
    update.edited_channel_post = OptionalGetMessageFromJson(json, "edited_channel_post");

    //  update.inline_query =
    //  update.chosen_inline_result =
    //  update.callback_query =
    //  update.shipping_query =

    return update;
}

std::vector<Update>
TelegramBotAPI::TelegramBotAPIImpl::
ConvertJsonToUpdates(
        const Json::Value& json
) {
    log_.debug(json.toStyledString());
    if (json.type() != Json::ValueType::arrayValue) {
        std::string err_msg = "Json value is " + std::to_string(json.type()) +
                ". Expected " + std::to_string(Json::ValueType::arrayValue);

        log_.error(err_msg);
        throw Poco::DataFormatException(err_msg);
    }

    std::vector<Update> updates;
    updates.reserve(json.size());
    for (const auto& upd : json) {
        try {
            updates.push_back(ConvertJsonToUpdate(upd));
        } catch (Poco::Exception& e) {
            log_.warning("Failed to handle update:" + e.displayText() +
                                 ". Skipped.");
            log_.debug("Update data:\n" + upd.toStyledString());
        }
    }

    return updates;
}


//
//      TelegramBotAPI class methods
//

std::string
User::GetInfo() {
    return "id: " + std::to_string(id) +
           "\nis_bot: " + std::to_string(is_bot) +
           "\nfirst_name: " + first_name + "\n";
}

TelegramBotAPI::
TelegramBotAPI(
        const std::string& token,
        const std::string& first_name,
        const std::string& log_level,
        const std::string& server_url
):
    pimpl_{std::make_unique<TelegramBotAPIImpl>(
            token, first_name, log_level, server_url)}
{}

TelegramBotAPI::
~TelegramBotAPI() = default;

void
TelegramBotAPI::
InitSession() {
    return pimpl_->InitSession();
}

void
TelegramBotAPI::
CloseSession() {
    return pimpl_->CloseSession();
}

void
TelegramBotAPI::
AbortSession() {
    return pimpl_->AbortSession();
}

void
TelegramBotAPI
::CheckBotInfo() {
    return pimpl_->CheckBotInfo();
}

User
TelegramBotAPI::
GetMe() {
    return pimpl_->GetMe();
}

std::vector<Update>
TelegramBotAPI::
GetUpdates(
        std::optional<int32_t> offset,
        std::optional<int32_t> timeout
) {
    return pimpl_->GetUpdates(offset, timeout);
}

std::vector<Update>
TelegramBotAPI
::GetUpdatesWithOffset(
        int32_t offset
) {
    return pimpl_->GetUpdates(offset, std::nullopt);
}

std::vector<Update>
TelegramBotAPI
::GetUpdatesWithTimeout(
        int32_t timeout
) {
    return pimpl_->GetUpdates(std::nullopt, timeout);
}

void TelegramBotAPI::
SendMessage(
        int32_t chat_id,
        const std::string& text
) {
    return pimpl_->SendMessage(
            chat_id,
            text,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            std::nullopt);
}

void
TelegramBotAPI
::SendMessage(
        int32_t chat_id,
        const std::string& text,
        int32_t reply_to_message_id
) {
    return pimpl_->SendMessage(
            chat_id,
            text,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            reply_to_message_id);
}

void
TelegramBotAPI
::SendSticker(
        int32_t chat_id,
        const std::string& file_id
) {
    return pimpl_->SendSticker(chat_id, file_id);
}

void
TelegramBotAPI
::SendDocument(
        int32_t chat_id,
        const std::string& document
) {
    return pimpl_->SendDocument(chat_id, document);
}

Logger&
TelegramBotAPI::
log() {
    return pimpl_->log();
}
