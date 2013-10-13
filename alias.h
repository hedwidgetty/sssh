#pragma once

#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#define MOST 1000

struct alias
{
	
	char cmd[MOST];
	char als[MOST];
};
typedef struct alias Alias;

Alias* createAlias(char* cmd, char* als);
