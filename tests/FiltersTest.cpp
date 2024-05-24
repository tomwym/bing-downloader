#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "filters/IFilter.h"
#include "filters/PassConditionalFilter.h"
#include "filters/YearFilter.h"
#include "filters/MonthFilter.h"
#include "filters/DayFilter.h"

using namespace ::testing;
using std::make_unique;

TEST(FilterTests, BasicAdd)
{
    const std::vector<std::string> collection{
        "20230101",
        "20230102",
        "20230103",
        "20230601",
        "20230602",
        "20230603",
        "20231201",
        "20231202",
        "20231203",
        "20240101",
        "20240102",
        "20240103",
        "20240601",
        "20240602",
        "20240603",
        "20241201",
        "20241202",
        "20241203",
    };
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"));
        auto filter_result = filter.Apply(collection);
        std::vector<std::string> truth {"20240101","20240102","20240103","20240601","20240602","20240603","20241201","20241202","20241203"};
        EXPECT_TRUE(filter_result == truth);
    }
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"))
            .And(make_unique<MonthFilter>(FilterOps::EQ, "06"));
        auto filter_result = filter.Apply(collection);
        std::vector<std::string> truth {"20240601","20240602","20240603"};
        EXPECT_TRUE(filter_result == truth);
    }
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"))
            .And(make_unique<MonthFilter>(FilterOps::EQ, "06"))
            .And(make_unique<DayFilter>(FilterOps::EQ, "02"));
        auto filter_result = filter.Apply(collection);
        std::vector<std::string> truth {"20240602"};
        EXPECT_TRUE(filter_result == truth);
    }
}

TEST(FilterTests, LessThanYearTests)
{
    {
        auto yf = make_unique<YearFilter>(FilterOps::LT, "2024");
        EXPECT_TRUE(yf->Filter("2023"));
        EXPECT_FALSE(yf->Filter("2024"));
        EXPECT_FALSE(yf->Filter("2025"));
    }
    const std::vector<std::string> year_collection{
        "2021",
        "2022",
        "2023",
        "2024",
    };
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::LT, "2024"));
        auto filter_result = filter.Apply(year_collection);
        std::vector<std::string> truth {"2021", "2022", "2023"};
        EXPECT_TRUE(filter_result == truth);
    }
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::LTE, "2024"));
        auto filter_result = filter.Apply(year_collection);
        EXPECT_TRUE(filter_result == year_collection);
    }
}

TEST(FilterTests, GreaterThanYearTests)
{
    {
        auto yf = make_unique<YearFilter>(FilterOps::GT, "2024");
        EXPECT_FALSE(yf->Filter("2023"));
        EXPECT_FALSE(yf->Filter("2024"));
        EXPECT_TRUE(yf->Filter("2025"));
    }
    const std::vector<std::string> year_collection{
        "2023",
        "2024",
        "2025",
    };
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::GT, "2023"));
        auto filter_result = filter.Apply(year_collection);
        std::vector<std::string> truth {"2024", "2025"};
        EXPECT_TRUE(filter_result == truth);
    }
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::GTE, "2023"));
        auto filter_result = filter.Apply(year_collection);
        EXPECT_TRUE(filter_result == year_collection);
    }
}

TEST(FilterTests, LessThanDayTests)
{
    {
        auto yf = make_unique<DayFilter>(FilterOps::LT, "03");
        EXPECT_TRUE(yf->Filter("00000002"));
        EXPECT_FALSE(yf->Filter("00000003"));
        EXPECT_FALSE(yf->Filter("00000004"));
    }
    const std::vector<std::string> day_collection{
        "00000001",
        "00000012",
        "00000013",
        "00000020",
    };
    {
        PassConditionalFilter filter;
        filter.And(make_unique<DayFilter>(FilterOps::LT, "13"));
        auto filter_result = filter.Apply(day_collection);
        std::vector<std::string> truth {"00000001", "00000012"};
        EXPECT_TRUE(filter_result == truth);
    }
    {
        PassConditionalFilter filter;
        filter.And(make_unique<DayFilter>(FilterOps::LTE, "20"));
        auto filter_result = filter.Apply(day_collection);
        EXPECT_TRUE(filter_result == day_collection);
    }
}