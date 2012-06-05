#ifndef _PROJECT_AS_PARSER_H_INCLUDED_
#define _PROJECT_AS_PARSER_H_INCLUDED_


#include "tokenizer.h"

struct Parser {

};


void parser_init(struct Parser *self);
void parse_token(struct Token *token);

#endif //_PROJECT_AS_PARSER_H_INCLUDED_