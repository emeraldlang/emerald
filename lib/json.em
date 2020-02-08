import core


let TokenType = {
    LBRACE: 0,
    RBRACE: 1,
    LBRACKET: 2,
    RBRACKET: 3,

    TRUE_LITERAL: 6,
    FALSE_LITERAL: 7,
    NULL_LITERAL: 8,

    ILLEGAL: 9
}


object Tokenizer

    def __init__ : json
        self.json = json
        self.i = 0
        self.current = None
    end

    def scan
        if self.i >= self.json.len() then
            return None
        end

        let c = self.json.at(self.i)
        self.i += 1
        if c == '{' then
            return self._emit(TokenType.LBRACE)
        else if c == '}' then
            return self._emit(TokenType.RBRACE)
        else if c == '[' then
            return self._emit(TokenType.LBRACKET)
        else if c == ']' then
            return self._emit(TokenType.RBRACKET)
        else if c.isalpha() then
            let lexeme = ''
            while True do
                lexeme += c
                c = self.json.at(self.i)
                self.i += 1
                if !c.isalpha() then break end
            end

            if lexeme == 'true' then
                return self._emit(TokenType.TRUE_LITERAL)
            else if lexeme == 'false' then
                return self._emit(TokenType.FALSE_LITERAL)
            else if lexeme == 'null' then
                return self._emit(TokenType.NULL_LITERAL)
            end
        end

        return self._emit(TokenType.ILLEGAL)
    end

    def _emit : lexeme, type
        self.current = {
            lexeme: lexeme,
            type: type
        }
        return self.current
    end

end


object ParseError clones core.Exception

end


object Parser

    def __init__ : json
        self.tokenizer = clone Tokenizer(json)
    end

    def parse
        return self._parse_value()
    end

    def _parse_value
        let token = self.tokenizer.scan()
        if token.type == TokenType.LBRACKET then
            let arr = []
            do
                let val = self._parse_value()
                arr.push(val)
            end while token.type == TokenType.COMMA
            self._expect(TokenType.RBRACKET)
            return arr
        else if token.type == TokenType.LBRACE then
            let obj = {}
            do
                self._expect(TokenType.STRING_LITERAL)
                let key = self.tokenizer.current.lexeme
                self._expect(TokenType.COLON)
                let val = self._parse_value()
                obj[key] = val
            end while token.type == TokenType.COMMA
            self._expect(TokenType.RBRACE)
            return obj
        else if token.type == TokenType.TRUE_LITERAL then
            return True
        else if token.type == TokenType.FALSE_LITERAL then
            return False
        else if token.type == TokenType.NULL_LITERAL then
            return None
        end
    end

    def _expect : expected
        if self.tokenizer.scan().type != expected then
            throw clone ParseError()
        end
    end

end


def deserialize : json
    return clone Parser(json).parse()
end
