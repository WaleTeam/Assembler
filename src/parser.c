#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
}

void parser_emit_code(struct Parser *self, void *buffer, emu_size_t size) {

}

void parser_parse_token(struct Parser *self, struct Token *token) {

	struct ParserNode *node = self->currentNode;

	if(node->state == ParserNodeStateSuccessful) {
		node->emit(node, self);
	} else if(node->state == ParserNodeStateError) {
		self->state = ParserStateError;
	} else {
		node->parse(node, self);
	}
}

void parser_finish(struct Parser *self) {
	
}

//#############################################################################
// ### node parsers ###
//#############################################################################
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
			result->state = ParserNodeStateSuccessful;
			break;
		case ParserNodeTypeStructuralOrigin:
			result = emu_malloc(sizeof(struct ParserNodeStructuralOrigin));
			result->parse = parserNodeStructuralOrigin_parse;
			result->emit = parserNodeStructuralOrigin_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeStructuralRegMode:
			result = emu_malloc(sizeof(struct ParserNodeStructuralRegMode));
			result->parse = parserNodeStructuralRegMode_parse;
			result->emit = parserNodeStructuralRegMode_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeStructuralLabel:
			result = emu_malloc(sizeof(struct ParserNodeStructuralLabel));
			result->parse = parserNodeStructuralLabel_parse;
			result->emit = parserNodeStructuralLabel_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeOpcode:
			result = emu_malloc(sizeof(struct ParserNodeOpcode));
			result->parse = parserNodeOpcode_parse;
			result->emit = parserNodeOpcode_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeString:
			result = emu_malloc(sizeof(struct ParserNodeString));
			result->parse = parserNodeString_parse;
			result->emit = parserNodeString_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeNumber:
			result = emu_malloc(sizeof(struct ParserNodeNumber));
			result->parse = parserNodeNumber_parse;
			result->emit = parserNodeNumber_emit;
			result->state = ParserNodeStateInitialized;
			break;
	}

	result->type = type;
	result->byteSize = 0;

	return result;
}

void parserNode_free(struct ParserNode *self) {
	emu_free(self);
}