#include "parser.h"

void parser_init(struct Parser *self) {
	self->origin = 0;
	self->flags = 0;
	self->currentNode = parserNode_create(ParserNodeTypeNone);
	self->symbolTable = emu_malloc(sizeof(struct List));

	list_init(self->symbolTable, 0);
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

	parserNode_free(self->currentNode);
	self->currentNode = result;

	return result;
}

void parser_parse_token(struct Parser *self, struct Token *token) {

	struct ParserNode *node = self->currentNode;

	if(node->state == ParserNodeStateSuccessful || node->state == ParserNodeStateEarlySuccess || node->state == ParserNodeStateError) {
		node->emit(node, self);
		node = parser_select_node(self, token);
	}

	if(node->state == ParserNodeStateInitialized || node->state == ParserNodeStateParsing) {
		node->parse(node, self, token);

		if(node->state == ParserNodeStateEarlySuccess) {
			parser_parse_token(self, token);
		}
	} else {
		self->state = ParserStateError;
	}
}

void parser_finish(struct Parser *self) {

}

//#############################################################################
// ### helpers ###
//#############################################################################
int is_keyword(char *string) {
	return 0;
}

int is_known_label(char *string) {
	return 0;
}

int is_opcode(char *string) {
	return 0;
}

//#############################################################################
// ### token testers ###
//#############################################################################

int parserNode_tokenTester(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	return 0;
}

int parserNodeStructuralLabel_tokenTester(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	return 0;
}

//#############################################################################
// ### node parsers ###
//#############################################################################
void parserNode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as default\n");
}

void parserNodeStructuralOrigin_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as origin\n");
}

void parserNodeStructural_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as structural\n");
}

void parserNodeStructuralRegMode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as regmode\n");
}

void parserNodeStructuralLabel_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as label: I\n");

	if(self->state == ParserNodeStateInitialized) {
		emu_log("parsing token as label: A\n");
		struct ParserNodeStructuralLabel *node = (struct ParserNodeStructuralLabel *)self;
		emu_strncpy(node->labelName, token->string, kMaxStringLength);

		self->state = ParserNodeStateParsing;
	} else if(self->state == ParserNodeStateParsing) {

		emu_log("parsing token as label: B\n");

		if(token->stringIndex == 1 && token->string[0] == ':') {
			emu_log("parsing token as label: C\n");
			self->state = ParserNodeStateSuccessful;
		} else {
			emu_log("parsing token as label: D\n");
			self->state = ParserNodeStateError;
		}
	}
}

void parserNodeWord_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {

	struct ParserNodeWord *node = (struct ParserNodeWord *)self;

	emu_log("Word Parser called\n");

	if(node->subNode == 0) {

		if(is_opcode(token->string)) {

			node->subNode = parserNode_create(ParserNodeTypeOpcode);
		} else {

			//try to define or reference a label
			node->subNode = parserNode_create(ParserNodeTypeStructuralLabel);
		}
	}

	if(node->subNode != 0 && node->parserNode.state != ParserNodeStateError) {
		node->subNode->parse(node->subNode, parser, token);
		node->parserNode.state = node->subNode->state;
	}
}

void parserNodeOpcode_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as opcode\n");
}

void parserNodeString_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as string\n");
}

void parserNodeNumber_parse(struct ParserNode *self, struct Parser *parser, struct Token *token) {
	emu_log("parsing token as number\n");
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
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNode_parse;
			result->emit = parserNode_emit;
			result->state = ParserNodeStateSuccessful;
			break;
		case ParserNodeTypeStructural:
			result = emu_malloc(sizeof(struct ParserNodeStructural));
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeStructural_parse;
			result->emit = parserNodeStructural_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeStructuralOrigin:
			result = emu_malloc(sizeof(struct ParserNodeStructuralOrigin));
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeStructuralOrigin_parse;
			result->emit = parserNodeStructuralOrigin_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeStructuralRegMode:
			result = emu_malloc(sizeof(struct ParserNodeStructuralRegMode));
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeStructuralRegMode_parse;
			result->emit = parserNodeStructuralRegMode_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeStructuralLabel:
			result = emu_malloc(sizeof(struct ParserNodeStructuralLabel));
			result->tokenTester = parserNodeStructuralLabel_tokenTester;
			result->parse = parserNodeStructuralLabel_parse;
			result->emit = parserNodeStructuralLabel_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeWord:
			result = emu_malloc(sizeof(struct ParserNodeWord));
			((struct ParserNodeWord *)result)->subNode = 0;
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeWord_parse;
			result->emit = parserNodeWord_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeOpcode:
			result = emu_malloc(sizeof(struct ParserNodeOpcode));
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeOpcode_parse;
			result->emit = parserNodeOpcode_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeString:
			result = emu_malloc(sizeof(struct ParserNodeString));
			result->tokenTester = parserNode_tokenTester;
			result->parse = parserNodeString_parse;
			result->emit = parserNodeString_emit;
			result->state = ParserNodeStateInitialized;
			break;
		case ParserNodeTypeNumber:
			result = emu_malloc(sizeof(struct ParserNodeNumber));
			result->tokenTester = parserNode_tokenTester;
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

	switch(self->type) {
		case ParserNodeTypeWord:
			parserNode_free(((struct ParserNodeWord *)self)->subNode);
		default:
			emu_free(self);
	}
}