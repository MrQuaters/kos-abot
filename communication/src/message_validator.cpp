#include "message_validator.h"
 
using namespace rapidjson;

std::unique_ptr<Message> MessageValidator::validateMessage(std::string message) {
    Document document;
    document.Parse(message.c_str());
    if (!document.IsObject() || document.HasParseError() || !document.HasMember("cmd") || !document["cmd"].IsString()) {
        return nullptr;
    }

    Message *msg = new Message();
    auto uPrt = std::unique_ptr<Message>(msg);

    auto cmd = document["cmd"].GetString();
    msg->cmd = -1;
    msg->speed = -1;
    msg->isManual = true;
    if (strcmp(cmd,"forward") == 0) {
        msg->cmd = 0;
    } 
    if (strcmp(cmd, "back") == 0) {
        msg->cmd = 1;
    }
    if (strcmp(cmd, "left") == 0) {
        msg->cmd = 2;
    }
    if (strcmp(cmd, "right") == 0) {
        msg->cmd = 3;
    }
    if (strcmp(cmd, "stop") == 0) {
        msg->cmd = 4;
        return uPrt;
    }

    if (msg->cmd < 0) {
        return nullptr;
    }

    if (!document.HasMember("val") || !document["val"].IsDouble()) {
        return nullptr;
    }
    
    msg->valMs = (int)(document["val"].GetDouble() * 1000);
    if (msg->valMs < 0) {
        return nullptr;
    }

    if (document.HasMember("spd") && document["spd"].IsDouble()) {
        int speed = (int)(document["spd"].GetDouble() * 100);
        if (speed<0 || speed>100) {
            return nullptr;
        } 
        msg->speed = speed;
    } 
    return uPrt;
}


bool Message::isManualMessage() {
    return isManual;
}

manual_message_payload Message::getManualMessagePayload() {
    return {
        cmd,
        valMs,
        speed
    };
}