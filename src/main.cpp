
#include "BingWallpaperScript.h"
#include "Options.h"

int main(const int argc, const char** argv)
{
    Options ops{argc, argv};
    if (!ops.Valid())
        return 0;

    std::unique_ptr<PassConditionalFilter> filter{std::make_unique<PassConditionalFilter>()};
    (*filter)
        .And(std::make_unique<YearFilter>(FilterOps::GTE, "2022"))
        // .And(std::make_unique<MonthFilter>(FilterOps::GTE, "06"))
        // .And(std::make_unique<DayFilter>(FilterOps::LT, "10"))
    ;

    BingWallpaperScript script{ops.Archive(), ops.Supplemental(), ops.ThreadCount(), ops.DryRun(), std::move(filter)};
    script.Execute();
    xmlCleanupParser();    // Free globals
    return 0;
}
