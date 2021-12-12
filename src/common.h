#pragma once

#include<iostream>
#include<cstdlib>
#include<vector>
#include<cstring>

using namespace std;

#define print(S) cout << S << endl

static void barley_exception(const char* type, const char* msg, int line) {
	print("** (" << type << ") " << msg << " at line " << line);
}
