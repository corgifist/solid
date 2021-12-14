// This script is part of SOLID language.
// Copyright 2021.
#pragma once

#include<map>
#include "../values/type.h"
#include "../common.h"

namespace Table {
    void put(string str, Value value);

    Value get(string str);

    void clear();
}
