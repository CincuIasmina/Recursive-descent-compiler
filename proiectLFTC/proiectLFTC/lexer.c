#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "utils.h"

char* denumiri[] = { "ID","INT","REAL","STR"
// keywords
,"VAR","FUNCTION","IF","ELSE","WHILE","END","RETURN","TYPE_INT","TYPE_REAL","TYPE_STR"
// delimiters
,"COMMA","COLON","SEMICOLON","LPAR","RPAR","FINISH"
// operators
,"ADD","SUB","MUL","DIV","AND","OR","NOT","ASSIGN","EQUAL","NOTEQ","LESS","GREATER","GREATEREQ"
};

Token tokens[MAX_TOKENS];
int nTokens;
int line = 1;		// the current line in the input files

// adds a token to the end of the tokens list and returns it
// sets its code and line
Token* addTk(int code) {
	if (nTokens == MAX_TOKENS)err("too many tokens");
	Token* tk = &tokens[nTokens];
	tk->code = code;
	tk->line = line;
	nTokens++;
	return tk;
}

// copy in the dst buffer the string between [begin,end)
char* copyn(char* dst, const char* begin, const char* end) {
	char* p = dst;
	if (end - begin > MAX_STR)err("string too long");
	while (begin != end)*p++ = *begin++;
	*p = '\0';
	return dst;
}

void tokenize(const char* pch) {
	const char* start;
	Token* tk;
	char buf[MAX_STR + 1];
	char buf2[MAX_STR + 1];
	for (;;) {
		switch (*pch) {

		case ' ':case '\t':pch++; break;
		case '\r':		// handles different kinds of newlines (Windows: \r\n, Linux: \n, MacOS, OS X: \r or \n)
			if (pch[1] == '\n')pch++;
			// fallthrough to \n
		case '\n':
			line++;
			pch++;
			break;
		case '#':
			for (start = pch++; *pch != '\n'; pch++) {}
			break;
		case ':': addTk(COLON); pch++; break;
		case ';': addTk(SEMICOLON); pch++; break;
		case '(': addTk(LPAR); pch++; break;
		case ')': addTk(RPAR); pch++; break;
		case '+': addTk(ADD); pch++; break;
		case '-': addTk(SUB); pch++; break;
		case '*': addTk(MUL); pch++; break;
		case '/': addTk(DIV); pch++; break;
		case '&':
			if (pch[1] == '&') {
				addTk(AND);
				pch += 2;
			}
			else
				err("missing second &");
			break;
		case '|':
			if (pch[1] == '|') {
				addTk(OR);
				pch += 2;
			}
			else
				err("missing second |");
			break;
		case '!':
			if (pch[1] == '=') {
				addTk(NOTEQ);
				pch += 2;
			}
			else {
				addTk(NOT);
				pch++;
			}
			break;
		case '<':
			if (pch[1] == '=') {
				addTk(LESSEQ);
				pch += 2;
			}
			else {
				addTk(LESS);
				pch++;
			}
			break;
		case '>':
			if (pch[1] == '=') {
				addTk(GREATEREQ);
				pch += 2;
			}
			else {
				addTk(GREATER);
				pch++;
			}
		case '\0':addTk(FINISH); return;
		case ',':
			addTk(COMMA);
			pch++;
			break;
		case '=':
			if (pch[1] == '=') {
				addTk(EQUAL);
				pch += 2;
			}
			else {
				addTk(ASSIGN);
				pch++;
			}
			break;
		case '"':
			for (start = ++pch; *pch != '"' && *pch != '\0'; pch++) {}
			if (*pch == '\0') { err("end of file while in double-quoted string"); }
			char* text = copyn(buf, start, pch);
			tk = addTk(STR);
			strcpy(tk->text, text);
			pch++;
			break;
		default:
			if (isalpha(*pch) || *pch == '_') {
				for (start = pch++; isalnum(*pch) || *pch == '_'; pch++) {}
				char* text = copyn(buf, start, pch);
				if (strcmp(text, "int") == 0) { tk = addTk(TYPE_INT); strcpy(tk->text, text); }
				else if (strcmp(text, "real") == 0) { tk = addTk(TYPE_REAL); strcpy(tk->text, text); }
				else if (strcmp(text, "str") == 0) { tk = addTk(TYPE_STR); strcpy(tk->text, text); }
				else if (strcmp(text, "var") == 0) { tk = addTk(VAR); strcpy(tk->text, text); }
				else if (strcmp(text, "function") == 0) { tk = addTk(FUNCTION); strcpy(tk->text, text); }
				else if (strcmp(text, "if") == 0) { tk = addTk(IF); strcpy(tk->text, text); }
				else if (strcmp(text, "else") == 0) { tk = addTk(ELSE); strcpy(tk->text, text); }
				else if (strcmp(text, "while") == 0) { tk = addTk(WHILE); strcpy(tk->text, text); }
				else if (strcmp(text, "end") == 0) { tk = addTk(END); strcpy(tk->text, text); }
				else if (strcmp(text, "return") == 0) { tk = addTk(RETURN); strcpy(tk->text, text); }
				else {
					tk = addTk(ID);
					strcpy(tk->text, text);
				}
			}
			else if (isdigit(*pch)) {
				for (start = pch++; isdigit(*pch); pch++) {}
				const char* firstPart = copyn(buf, start, pch);
				if (*pch == '.') {
					pch++;
					if (!isdigit(*pch)) {
						err("Invalid real number: missing digits after '.' at line %d", line);
					}
					for (start = pch++; isdigit(*pch); pch++) {}
					const char* secondPart = copyn(buf2, start, pch);
					strcpy(buf, strcat(firstPart, secondPart));
					tk = addTk(REAL);
					tk->r = atof(buf);
				}
				else {
					tk = addTk(INT);
					tk->i = atoi(firstPart);
				}
			}
			else err("invalid char: %c (%d)", *pch, *pch);
		}
	}
}

void showTokens() {
	for (int i = 0; i < nTokens; i++) {
		Token* tk = &tokens[i];
		char* nume = denumiri[tk->code];

		if (strcmp(nume, "ID") == 0) {
			printf("%d %s:%s\n", tk->line, nume, tk->text);
		}
		else if (strcmp(nume, "INT") == 0) {
			printf("%d %s:%d\n", tk->line, nume, tk->i);
		}
		else if (strcmp(nume, "REAL") == 0) {
			printf("%d %s:%g\n", tk->line, nume, tk->r);
		}
		else if (strcmp(nume, "STR") == 0) {
			printf("%d %s:%s\n", tk->line, nume, tk->text);
		}
		else {
			printf("%d %s\n", tk->line, nume);
		}
	}

}