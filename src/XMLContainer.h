#pragma once

#include <filesystem>
#include <libxml/HTMLparser.h>

class XMLContainer {
public:
    XMLContainer() = delete;
    XMLContainer(const std::filesystem::path& file);
    XMLContainer(const std::string& content);

    ~XMLContainer();

    [[nodiscard]] auto Root() const -> xmlNode*;
    [[nodiscard]] auto Valid() const -> bool;
    [[nodiscard]] auto Doc() const -> xmlDoc*;
private:
    bool _valid;
    htmlDocPtr _doc;
};
using XMLContainerPtr = std::unique_ptr<XMLContainer>;
