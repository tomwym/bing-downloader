#include "XMLContainer.h"

#include <iostream>

XMLContainer::XMLContainer(const std::filesystem::path& file)
: _valid{false}
, _doc{nullptr}
{
    if (!std::filesystem::exists(file))
        return;

    _doc = htmlReadFile(file.string().c_str(), NULL,HTML_PARSE_RECOVER | HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (_doc == nullptr)
    {
        std::cerr << "Document not parsed successfully.\n";
        return;
    }
}

XMLContainer::XMLContainer(const std::string& content)
: _valid{false}
, _doc{nullptr}
{
    _doc = htmlReadFile(content.c_str(), NULL,HTML_PARSE_RECOVER | HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (_doc == nullptr)
    {
        std::cerr << "Document not parsed successfully.\n";
        return;
    }
}

XMLContainer::~XMLContainer()
{
    if (_doc) {
        xmlFreeDoc(_doc); // free document
        _doc = nullptr;
    }
}

auto XMLContainer::Root() const -> xmlNode* {
    if (_doc)
        return xmlDocGetRootElement(_doc);
    return nullptr;
}

auto XMLContainer::Valid() const -> bool {
    return _doc != nullptr;
}

auto XMLContainer::Doc() const -> xmlDoc* {
    return _doc;
}