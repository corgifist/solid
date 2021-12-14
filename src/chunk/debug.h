// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "chunk.h"
#include "../common.h"

void disassemble(Chunk *chunk, const char *name);

int offsetize(Chunk *chunk, int offset);

int simpleOffset(int offset, const char *name);

int unknownOffset(int offset);

int constOffset(int offset, const char *name, Chunk *chunk);

int operatorOffset(int offset, const char *name, Chunk *chunk);
