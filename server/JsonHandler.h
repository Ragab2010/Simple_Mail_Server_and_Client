#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <string>
#include <tuple>

class JsonHandler {
public:
    static std::string serializeToJson(const std::string& requestType, const std::string& username, const std::string& receiver, const std::string& message);
    static std::tuple<std::string, std::string, std::string, std::string> deserializeFromJson(const std::string& jsonString);
    static std::string serializeToJson_v2(const std::string& requestType, const std::string& username, const std::string& receiver, const std::string& message);
    static std::tuple<std::string, std::string, std::string, std::string> deserializeFromJson_v2(const std::string& jsonString);
};

#endif /* JSON_HANDLER_H */
