#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
}

void parser_parse_token(struct Parser *self, struct Token *token) {

}

void parserNode_parse(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructuralOrigin_parse(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructuralRegMode_parse(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeStructuralLabel_parse(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeOpcode_parse(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeString_parse(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeNumber_parse(struct ParserNode *self, struct Parser *parser) {
	
}

struct ParserNode *parserNode_create(enum ParserNodeType type) {

	struct ParserNode *result = 0;

	switch(type) {
		default:
			return result;
		case ParserNodeTypeNone:
			result = emu_malloc(sizeof(struct ParserNode));
			result->parse = parserNode_parse;
			break;
		case ParserNodeTypeStructuralOrigin:
			result = emu_malloc(sizeof(struct ParserNodeStructuralOrigin));
			result->parse = parserNodeStructuralOrigin_parse;
			break;
		case ParserNodeTypeStructuralRegMode:
			result = emu_malloc(sizeof(struct ParserNodeStructuralRegMode));
			result->parse = parserNodeStructuralRegMode_parse;
			break;
		case ParserNodeTypeStructuralLabel:
			result = emu_malloc(sizeof(struct ParserNodeStructuralLabel));
			result->parse = parserNodeStructuralLabel_parse;
			break;
		case ParserNodeTypeOpcode:
			result = emu_malloc(sizeof(struct ParserNodeOpcode));
			result->parse = parserNodeOpcode_parse;
			break;
		case ParserNodeTypeString:
			result = emu_malloc(sizeof(struct ParserNodeString));
			result->parse = parserNodeString_parse;
			break;
		case ParserNodeTypeNumber:
			result = emu_malloc(sizeof(struct ParserNodeNumber));
			result->parse = parserNodeNumber_parse;
			break;
	}

	result->type = type;
	result->byteSize = 0;

	return result;
}

void parserNode_free(struct ParserNode *self) {
	emu_free(self);
}