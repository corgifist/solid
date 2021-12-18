// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<cstring>
#include<sstream>
#include<stdint-gcc.h>
#include <cmath>
#include<map>

#include "runtime.h"

using namespace std;

#define print(S) cout << S << endl

static void barley_exception(string type, string msg, int line) {
    runtime_result = RUNTIME_ERROR;
    print("** (" << type << ") " << msg << " at line " << line);
}

static void system_exception(string type, string msg) {
    runtime_result = RUNTIME_ERROR;
    print("** (" << type << ") " << msg);
}