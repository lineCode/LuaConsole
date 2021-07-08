/* MIT License
 * 
 * Copyright (c) 2017-2021 Cody Tilkins
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "darr.h"

typedef struct LangCache {
	size_t indices;
	Array* lines;
	char* buffer;
} LangCache;


static inline LangCache* langfile_load(const char* path) {
	
	FILE* fp;
	fp = fopen(path, "rb");
	if(fp == 0) {
		fprintf(stderr, "%s %s!\n", "Couldn't open file", path);
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	LangCache* lc = (LangCache*) malloc(1 * sizeof(LangCache));
	if(lc == 0) {
		fputs("Out of memory!\n", stderr);
		return 0;
	}
	
	char* buffer = (char*) malloc(size * sizeof(char) + 1);
	if(buffer == 0) {
		fputs("Out of memory!\n", stderr);
		return 0;
	}
	fread(buffer, size, 1, fp);
	fclose(fp);
	buffer[size] = 0;
	lc->buffer = buffer;
	
	Array* array = array_new(10, 1);
	if(array == 0) {
		fputs("Out of memory!\n", stderr);
		return 0;
	}
	lc->lines = array;
	
	size_t indices = 0;
	char* begin = buffer;
	char* ch = buffer;
	while(*ch != '\0') {
		if (*ch == '\\') {
			*ch = '\n';
			ch++;
			continue;
		}
		if (*ch == '\n') {
			if(ch - begin > 1) {
				array_push(array, begin);
				indices++;
			}
			begin = ch + 1;
			*ch = 0;
			ch++;
			continue;
		}
		ch++;
	}
	lc->indices = indices;
	
	return lc;
}

static inline const char* langfile_get(LangCache* lc, const char* key) {
	for(size_t i=0; i<lc->indices; i++) {
		char* current = (char*) array_get(lc->lines, i);
		if(memcmp(current, key, strlen(key)) == 0) {
			while(*current++ != '=');
			return (const char*) current;
		}
	}
	return 0;
}

static inline void langfile_free(void* lc) {
	if(lc == 0)
		return;
	LangCache* _lc = (LangCache*) lc;
	array_free(_lc->lines);
	free(_lc->buffer);
	free((LangCache*) _lc);
}

