#pragma once

#include "XMLContainer.h"

#include <filesystem>
#include <map>

#include "CurlWriteChunks.h"

enum class RESOURCE_TYPE {
    HTML,
    JPG,
};

class ResourceGetterPrivateTestsAdapter;

class ResourceGetter {
    friend ResourceGetterPrivateTestsAdapter;

    bool _get_from_online;
    std::filesystem::path _local_root;
public:
    ResourceGetter(bool get_from_online, const std::filesystem::path& local_root)
    : _get_from_online{get_from_online}
    , _local_root{local_root}
    {}
    /**
     * @param URI the absolute uri of the resource (www.example.com/index.html)
     */
    auto GetResource(const std::string& URI, const RESOURCE_TYPE type) -> XMLContainerPtr {
        // Allow
        XMLContainerPtr local {GetLocalResource(URI, type)};
        if (local && local->Valid()) {
            return std::move(local);
        }
        if (_get_from_online) {
            return GetOnlineResource(URI, type);
        }
        return nullptr;
    }

private:
    auto URLToLocalResource(const std::string& URL, const RESOURCE_TYPE type) -> std::filesystem::path {
        const static std::map<RESOURCE_TYPE, std::string> extensions{
            {RESOURCE_TYPE::HTML, ".html"},
            {RESOURCE_TYPE::JPG, ".JPG"},
        };

        // construct destination file name
        // https://bingwallpaper.anerg.com/index.html => index.html
        // https://bingwallpaper.anerg.com/archive/us/202402 => archive/us/202402
        // https://bingwallpaper.anerg.com/detail/us/SleepingFox => archive/us/202402
        const std::string dot_com{".com"};
        std::filesystem::path local_file{URL.substr(URL.find(dot_com)+dot_com.size()+1)};
        if (local_file.extension() == "") {
            // add extension based on type
            local_file.replace_extension(extensions.at(type));
        }
        return _local_root / local_file;
    }

    auto GetOnlineResource(const std::string& URL, const RESOURCE_TYPE type) -> XMLContainerPtr {
        const std::filesystem::path local_relative_path{URLToLocalResource(URL, type)};
        CurlWriteChunks curl{URL, local_relative_path};
        return std::make_unique<XMLContainer>(local_relative_path);
    }

    auto GetLocalResource(const std::string& URL, const RESOURCE_TYPE type) -> XMLContainerPtr {
        const std::filesystem::path local_relative_path{URLToLocalResource(URL, type)};
        return std::make_unique<XMLContainer>(local_relative_path);
    }
};