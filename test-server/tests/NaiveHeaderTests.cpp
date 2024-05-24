#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "BasicTypes.h"
#include "NaiveHeader.h"

using namespace ::testing;

TEST(NaiveHeaderTests, TestConstruct)
{
    std::cout << "TestConstruct" << '\n';
    Header h;
    h.AddContentType(CONTENT_TYPE::IMAGE_JPEG)
     .AddContentLength(5)
     .AddDate()
     .AddHost("0.0.0.0:8080")
     .AddAcceptRanges();
    std::cout << h.Serialize();
}