#ifndef _MESSAGE_VALIDATOR_H
#define _MESSAGE_VALIDATOR_H

#include <string>
#include <memory>
#include "rapidjson/document.h"

struct manual_message_payload {
    int cmd;
    int valMs;
};

class MessageValidator;

class Message {
    bool isManual;
    int cmd;
    int valMs;
public:
    bool isManualMessage();
    manual_message_payload getManualMessagePayload();

    friend class MessageValidator;
};


class MessageValidator {
public:
    static std::unique_ptr<Message> validateMessage(std::string message);
};

#endif