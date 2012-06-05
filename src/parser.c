#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
}

void parser_parse_token(struct Parser *self, struct Token *token) {

}

struct ParserNode *parserNode_create(enum ParserNodeType type) {

	struct ParserNode *result = 0;

	switch(type) {
		default:
			return result;
		case ParserNodeTypeNone:
			result = emu_malloc(sizeof(struct ParserNode));
			break;
		case ParserNodeTypeStructuralOrigin:
			result = emu_malloc(sizeof(struct ParserNodeStructuralOrigin));
			break;
		case ParserNodeTypeStructuralRegMode:
			result = emu_malloc(sizeof(struct ParserNodeStructuralRegMode));
			break;
		case ParserNodeTypeStructuralLabel:
			result = emu_malloc(sizeof(struct ParserNodeStructuralLabel));
			break;
		case ParserNodeTypeOpcode:
			result = emu_malloc(sizeof(struct ParserNodeOpcode));
			break;
		case ParserNodeTypeString:
			result = emu_malloc(sizeof(struct ParserNodeString));
			break;
		case ParserNodeTypeNumber:
			result = emu_malloc(sizeof(struct ParserNodeNumber));
			break;
	}

	result->type = type;
	result->byteSize = 0;

	return result;
}

void parserNode_free(struct ParserNode *self) {
	emu_free(self);
}