#include "alias.h"


Alias* createAlias(char* cmd, char* als)
{
	
	Alias* a=(Alias*)calloc(1, sizeof(Alias));
	
	strcpy(a->cmd, cmd);
	strcpy(a->als, als);
	return a;
}
