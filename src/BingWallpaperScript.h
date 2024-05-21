#pragma once

#include "XMLContainer.h"
#include "ResourceGetter.h"

class BingWallpaperScript {
    ResourceGetter _resource_getter;
public:
    BingWallpaperScript()
    : _resource_getter{false, "tests_archive"}
    {}
private:


};