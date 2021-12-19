// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "chunk.h"
#include "../common.h"

void disassemble(Chunk *chunk, const char *name);

int offsetize(Chunk *chunk, int offset);

int simpleOffset(int offset, const char *name);

int unknownOffset(Chunk* chunk, int offset);

int constOffset(int offset, const char *name, Chunk *chunk);

int buildSwitchOffset(int offset, const char* name, Chunk* chunk);

int longConstOffset(int offset, const char* name, Chunk* chunk);

int operatorOffset(int offset, const char *name, Chunk *chunk);

int switchOffset(int offset, const char* name, Chunk* chunk);

int switchTableOffset(int offset, const char *name);
