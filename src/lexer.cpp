#include "lexer.h"
#include "common.h"

class Lexer {
	private:
		const char* input;
		unsigned int length, pos, line;
		vector<Token> result;
		
		Token token(TokenType type, const char* msg) {
			Token tok;
			tok.type = type;
			tok.text = msg;
			tok.line = line;
			return tok;
		}

        char peek(int relativePos) {
            string str = input;
            int finalPos = pos + relativePos;
            char at = str.at(finalPos);
            if (at == '\0') return '\0';
            return at;
        }

        char advance() {
            char result = peek(0);
            if (result == '\n') line++;
            pos++;
            return result;
        }

        bool end() {
            return pos >= length;
        }

	public:
		Lexer(const char* source) {
			input = source;
			length = strlen(source);
			line = 1;
            pos = 0;
		}
		
		vector<Token> lex() {
			return result;
		}

        void setInput(const char* msg) {
            input = msg;
        }

        const char* getInput() const {
            return input;
        }

        int getPos() const {
            return pos;
        }

		
};
