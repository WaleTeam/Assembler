#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
}

void parser_emit_code(struct Parser *self, void *buffer, emu_size_t size) {

}

struct ParserNode *parser_select_node(struct Parser *self, struct Token *token) {

	struct ParserNode *result = 0;

	switch(token->type) {
		case TokenTypeString:
			result = parserNode_create(ParserNodeTypeString);
			break;
		case TokenTypeNumber:
			result = parserNode_create(ParserNodeTypeNumber);
			break;
		case TokenTypeStructural:
			result = parserNode_create(ParserNodeTypeStructural);
			break;
		case TokenTypeWord:
			result = parserNode_create(ParserNodeTypeWord);
			break;
		default:
			//FIXME: throw error
			result = parserNode_create(ParserNodeTypeNone);
			break;
	}

	return result;
}

void parser_parse_token(struct Parser *self, struct Token *token) {

	struct ParserNode *node = self->currentNode;

	if(node->state == ParserNodeStateSuccessful) {
		node->emit(node, self);
		parserNode_free(node);

		node = parser_select_node(self, token);
		self->currentNode = node;

		node->parse(node, self, token);

	} else if(node->state == ParserNodeStateError) {
		self->state = ParserStateError;
	} else {
		node->parse(node, self, token);
	}
}

void parser_finish(struct Parser *self) {

}

//#############################################################################
// ### node parsers ###
//#############################################################################
void parserNode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {

}

void parserNodeStructuralOrigin_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {

}

void parserNodeStructural_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {

}

void parserNodeStructuralRegMode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	
}

void parserNodeStructuralLabel_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	
}

void parserNodeWord_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {

}

void parserNodeOpcode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	
}

void parserNodeString_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	
}

void parserNodeNumber_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	
}

//#############################################################################
// ### binary emitter ###
//#############################################################################
void parserNode_emit(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructuralOrigin_emit(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructural_emit(struct ParserNode *self, struct Parser *parser) {

}

void parserNodeStructuralRegMode_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeStructuralLabel_emit(struct ParserNode *self, struct Parser *parser) {
	
}

void parserNodeWord_emit(struct ParserNode *self, struct Parser *parser) {

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
		case ParserNodeTypeStructural:
			result = emu_malloc(sizeof(struct ParserNodeStructural));
			result->parse = parserNodeStructural_parse;
			result->emit = parserNodeStructural_emit;
			result->state = ParserNodeStateInitialized;
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
		case ParserNodeTypeWord:
			result = emu_malloc(sizeof(struct ParserNodeWord));
			result->parse = parserNodeWord_parse;
			result->emit = parserNodeWord_emit;
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