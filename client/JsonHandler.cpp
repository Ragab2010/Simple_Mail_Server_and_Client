#include "JsonHandler.h"
#include <jsoncpp/json/json.h>
#include <sstream>
#include <iostream>

std::string JsonHandler::serializeToJson(const std::string& requestType, const std::string& username, const std::string& receiver, const std::string& message) {
    Json::Value root;
    root["requestType"] = requestType;
    root["username"] = username;
    root["receiver"] = receiver;
    root["message"] = message;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::ostringstream os;
    Json::StreamWriter* writer = builder.newStreamWriter();
    writer->write(root, &os);
    delete writer;
    return os.str();
}
std::string JsonHandler::serializeToJson_v2(const std::string& requestType, const std::string& username, const std::string& receiver, const std::string& message) {
    Json::Value root;
    root["requestType"] = requestType;
    root["username"] = username;
    root["receiver"] = receiver;
    root["message"] = message;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::string jsonData = Json::writeString(builder, root);
    return jsonData;
    /*******************************/
    // std::stringstream ss
    // ss<<root;
    // return ss.str();
    /*******************************/
}

std::tuple<std::string, std::string, std::string, std::string> JsonHandler::deserializeFromJson(const std::string& jsonString) {
    std::string requestType, username, receiver, message;
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::istringstream is(jsonString);
    std::string errs;
    if (!Json::parseFromStream(builder, is, &root, &errs)) {
        std::cerr << "Error parsing JSON: " << errs << std::endl;
        return std::make_tuple("", "", "", "");
    }

    if (root.isMember("requestType") && root["requestType"].isString())
        requestType = root["requestType"].asString();
    if (root.isMember("username") && root["username"].isString())
        username = root["username"].asString();
    if (root.isMember("receiver") && root["receiver"].isString())
        receiver = root["receiver"].asString();
    if (root.isMember("message") && root["message"].isString())
        message = root["message"].asString();

    return std::make_tuple(requestType, username, receiver, message);
}

std::tuple<std::string, std::string, std::string, std::string> JsonHandler::deserializeFromJson_v2(const std::string& jsonString) {
    std::string requestType, username, receiver, message;
    Json::Value root;
    Json ::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString, root);
    if (!parsingSuccessful) {
        std::cerr << "Error parsing JSON: "<< std::endl;
        return std::make_tuple("", "", "", "");
    }

    if (root.isMember("requestType") && root["requestType"].isString())
        requestType = root["requestType"].asString();
    if (root.isMember("username") && root["username"].isString())
        username = root["username"].asString();
    if (root.isMember("receiver") && root["receiver"].isString())
        receiver = root["receiver"].asString();
    if (root.isMember("message") && root["message"].isString())
        message = root["message"].asString();

    return std::make_tuple(requestType, username, receiver, message);
}
