// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<cstring>
#include<sstream>

using namespace std;

#define print(S) cout << S << endl

static void barley_exception(string type, string msg, int line) {
	print("** (" << type << ") " << msg << " at line " << line);
    exit(1);
}