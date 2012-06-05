#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
}

//#############################################################################
// ### node parsers ###
//#############################################################################
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

void parserNode_emit(struct ParserNode *self, struct Parser *parser) {

}

//#############################################################################
// ### binary emitter ###
//#############################################################################
void parserNodeStructuralOrigin_emit(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructuralRegMode_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeStructuralLabel_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeOpcode_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeString_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeNumber_emit(struct ParserNode *self, struct Parser *parser) {
	
}

struct ParserNode *parserNode_create(enum ParserNodeType type) {

	struct ParserNode *result = 0;

	switch(type) {
		default:
			return result;
		case ParserNodeTypeNone:
			result = emu_malloc(sizeof(struct ParserNode));
			result->parse = parserNode_parse;
			result->emit = parserNode_emit;
			break;
		case ParserNodeTypeStructuralOrigin:
			result = emu_malloc(sizeof(struct ParserNodeStructuralOrigin));
			result->parse = parserNodeStructuralOrigin_parse;
			result->emit = parserNodeStructuralOrigin_emit;
			break;
		case ParserNodeTypeStructuralRegMode:
			result = emu_malloc(sizeof(struct ParserNodeStructuralRegMode));
			result->parse = parserNodeStructuralRegMode_parse;
			result->emit = parserNodeStructuralRegMode_emit;
			break;
		case ParserNodeTypeStructuralLabel:
			result = emu_malloc(sizeof(struct ParserNodeStructuralLabel));
			result->parse = parserNodeStructuralLabel_parse;
			result->emit = parserNodeStructuralLabel_emit;
			break;
		case ParserNodeTypeOpcode:
			result = emu_malloc(sizeof(struct ParserNodeOpcode));
			result->parse = parserNodeOpcode_parse;
			result->emit = parserNodeOpcode_emit;
			break;
		case ParserNodeTypeString:
			result = emu_malloc(sizeof(struct ParserNodeString));
			result->parse = parserNodeString_parse;
			result->emit = parserNodeString_emit;
			break;
		case ParserNodeTypeNumber:
			result = emu_malloc(sizeof(struct ParserNodeNumber));
			result->parse = parserNodeNumber_parse;
			result->emit = parserNodeNumber_emit;
			break;
	}

	result->type = type;
	result->byteSize = 0;

	return result;
}

void parserNode_free(struct ParserNode *self) {
	emu_free(self);
}