#include<stdlib.h>
#include<stdio.h>

int main() {
	char* inbuf = loadFile("1.q");
	tokenize(inbuf);
	//showTokens();
	free(inbuf);
	parse();
	return 0;
}