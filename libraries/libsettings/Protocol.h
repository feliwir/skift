#pragma once

#include <libipc/Peer.h>
#include <libutils/json/Json.h>

#include <libsettings/Path.h>

namespace settings
{

struct Message
{
    enum Type : uint8_t
    {
        CLIENT_READ,
        CLIENT_WRITE,
        CLIENT_WATCH,
        CLIENT_UNWATCH,

        SERVER_ACK,
        SERVER_VALUE,
        SERVER_NOTIFY,
    };

    Type type;
    Optional<Path> path;
    Optional<json::Value> payload;
};

struct Protocol
{
    using Message = settings::Message;

    static Result encode_message(Connection *connection, const Message &message);

    static ResultOr<Message> decode_message(Connection *connection);
};

} // namespace settings
