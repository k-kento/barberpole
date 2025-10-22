#pragma once

#include <string>
#include "render_message.hpp"

class UpdateTextureMessage : public RenderMessage {

public:

    explicit UpdateTextureMessage(const std::string &filePath) : filePath{filePath} {}

    std::string filePath;

};
