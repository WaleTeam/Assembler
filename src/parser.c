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
	self->state = ParserStateError;
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

//#############################################################################
// ### constructors ###
//#############################################################################
void *check_for_alloc(void * ptr, emu_size_t size) {

	if(ptr == 0) {
		return emu_malloc(size);
	}

	return ptr;
}

struct ParserNode *parserNode_init(struct ParserNode *self) {

	struct ParserNode *result = check_for_alloc(self, sizeof(struct ParserNode));

	result->tokenTester = parserNode_tokenTester;
	result->parse = parserNode_parse;
	result->emit = parserNode_emit;
	result->state = ParserNodeStateSuccessful;
	result->type = ParserNodeTypeNone;
	result->byteSize = 0;

	return result;
}

struct ParserNodeStructural *parserNodeStructural_init(struct ParserNodeStructural *self) {

	struct ParserNodeStructural *result = check_for_alloc(self, sizeof(struct ParserNodeStructural));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeStructural;

	return result;
}

struct ParserNodeStructuralOrigin *parserNodeStructuralOrigin_init(struct ParserNodeStructuralOrigin *self) {

	struct ParserNodeStructuralOrigin *result = check_for_alloc(self, sizeof(struct ParserNodeStructuralOrigin));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeStructuralOrigin;

	return result;
}

struct ParserNodeStructuralRegMode *parserNodeStructuralRegMode_init(struct ParserNodeStructuralRegMode *self) {

	struct ParserNodeStructuralRegMode *result = check_for_alloc(self, sizeof(struct ParserNodeStructuralRegMode));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeStructuralRegMode;

	return result;
}

struct ParserNodeStructuralLabel *parserNodeStructuralLabel_init(struct ParserNodeStructuralLabel *self) {

	struct ParserNodeStructuralLabel *result = check_for_alloc(self, sizeof(struct ParserNodeStructuralLabel));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeStructuralLabel;
	result->parserNode.tokenTester = parserNodeStructuralLabel_tokenTester;

	return result;
}

struct ParserNodeWord *parserNodeWord_init(struct ParserNodeWord *self) {

	struct ParserNodeWord *result = check_for_alloc(self, sizeof(struct ParserNodeWord));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeWord;
	result->subNode = 0;

	return result;
}

struct ParserNodeOpcode *parserNodeOpcode_init(struct ParserNodeOpcode *self) {

	struct ParserNodeOpcode *result = check_for_alloc(self, sizeof(struct ParserNodeOpcode));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeOpcode;

	return result;
}

struct ParserNodeString *parserNodeString_init(struct ParserNodeString *self) {

	struct ParserNodeString *result = check_for_alloc(self, sizeof(struct ParserNodeString));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeString;

	return result;
}

struct ParserNodeNumber *parserNodeNumber_init(struct ParserNodeNumber *self) {

	struct ParserNodeNumber *result = check_for_alloc(self, sizeof(struct ParserNodeNumber));

	parserNode_init((struct ParserNode *) result);
	result->parserNode.type = ParserNodeTypeNumber;

	return result;
}

struct ParserNode *parserNode_create(enum ParserNodeType type) {

	struct ParserNode *result = 0;

	switch(type) {
		default:
			return result;
		case ParserNodeTypeNone:
			return parserNode_init(0);
		case ParserNodeTypeStructural:
			return (struct ParserNode *)parserNodeStructural_init(0);
		case ParserNodeTypeStructuralOrigin:
			return (struct ParserNode *)parserNodeStructuralOrigin_init(0);
		case ParserNodeTypeStructuralRegMode:
			return (struct ParserNode *)parserNodeStructuralRegMode_init(0);
		case ParserNodeTypeStructuralLabel:
			return (struct ParserNode *)parserNodeStructuralLabel_init(0);
		case ParserNodeTypeWord:
			return (struct ParserNode *)parserNodeWord_init(0);
		case ParserNodeTypeOpcode:
			return (struct ParserNode *)parserNodeOpcode_init(0);
		case ParserNodeTypeString:
			return (struct ParserNode *)parserNodeString_init(0);
		case ParserNodeTypeNumber:
			return (struct ParserNode *)parserNodeNumber_init(0);
	}

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