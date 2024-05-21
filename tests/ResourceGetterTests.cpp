#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ResourceGetter.h"

using namespace ::testing;

class ResourceGetterPrivateTestsAdapter {
    ResourceGetter& _getter;
public:
    ResourceGetterPrivateTestsAdapter(ResourceGetter& getter) : _getter{getter} {}

    auto URLToLocalResource(const std::string& URL, const RESOURCE_TYPE type) -> std::filesystem::path {
        return _getter.URLToLocalResource(URL, type);
    }

    auto GetOnlineResource(const std::string& URL, const RESOURCE_TYPE type) -> XMLContainerPtr {
        return _getter.GetOnlineResource(URL, type);
    }

    auto GetLocalResource(const std::string& URL, const RESOURCE_TYPE type) -> XMLContainerPtr {
        return _getter.GetLocalResource(URL, type);
    }
};


const std::string wallpaper_literal1{"https://bingwallpaper.anerg.com/index.html"};
const std::string wallpaper_literal2{"https://bingwallpaper.anerg.com/archive/us/202401"};
const std::string wallpaper_literal3{"https://bingwallpaper.anerg.com/detail/us/SleepingFox"};

TEST(ResourceGetterTests, OnlineGetter)
{
    ResourceGetter getter{true, "tests_content"};
    {
        auto xml = getter.GetResource(wallpaper_literal1, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
    {
        auto xml = getter.GetResource(wallpaper_literal2, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
    {
        auto xml = getter.GetResource(wallpaper_literal3, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
}

TEST(ResourceGetterTests, URLToLocalResourceTest)
{
    ResourceGetter resource_getter{false, ""};
    ResourceGetterPrivateTestsAdapter getter{resource_getter};
    {
        auto path = getter.URLToLocalResource(wallpaper_literal1, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(path.string() == "index.html");
    }
    {
        auto path = getter.URLToLocalResource(wallpaper_literal2, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(path.string() == "archive/us/202401.html");
    }
    {
        auto path = getter.URLToLocalResource(wallpaper_literal3, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(path.string() == "detail/us/SleepingFox.html");
    }
}

TEST(ResourceGetterTests, GetLocalResourceTest)
{
    ResourceGetter resource_getter{false, "tests_content"};
    ResourceGetterPrivateTestsAdapter getter{resource_getter};
    {
        auto xml = getter.GetLocalResource(wallpaper_literal1, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
    {
        auto xml = getter.GetLocalResource(wallpaper_literal2, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
    {
        auto xml = getter.GetLocalResource(wallpaper_literal3, RESOURCE_TYPE::HTML);
        EXPECT_TRUE(xml);
        EXPECT_TRUE(xml->Valid());
    }
}

