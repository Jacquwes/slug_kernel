#pragma once

/*
* This file defines general utilities:
* Memory management
*/

typedef decltype(sizeof(char)) size_t;

void* malloc(size_t size);
void free(void* ptr);
