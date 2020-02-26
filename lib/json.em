import core


let TokenType = {
    LBRACE: 0,
    RBRACE: 1,
    LBRACKET: 2,
    RBRACKET: 3,
    COMMA: 4,
    COLON: 5,

    STRING_LITERAL: 6,
    TRUE_LITERAL: 7,
    FALSE_LITERAL: 8,
    NULL_LITERAL: 9,

    ILLEGAL: 10,
    EOSF: 11,
}


object Tokenizer

    def __init__ : json
        self.json = json
        self.si = 0
        self.i = 0
        self.c = self.json.at(self.i)
        self.current = None
        self.next = None
        self.scan() # sets self.next
    end

    def scan
        if self.i >= self.json.len() then
            return self._emit(TokenType.EOSF)
        end

        self.si = self.i
        while True do
            if self.c == '{' then
                return self._advance_and_emit(TokenType.LBRACE)
            else if self.c == '}' then
                return self._advance_and_emit(TokenType.RBRACE)
            else if self.c == '[' then
                return self._advance_and_emit(TokenType.LBRACKET)
            else if self.c == ']' then
                return self._advance_and_emit(TokenType.RBRACKET)
            else if self.c == ',' then
                return self._advance_and_emit(TokenType.COMMA)
            else if self.c == ':' then
                return self._advance_and_emit(TokenType.COLON)
            else if self.c == '"' then
                self._advance()
                while self.c != '"' do self._advance() end
                return self._emit(TokenType.STRING_LITERAL)
            else if self.c.isalpha() then
                while self.c.isalpha() do self._advance() end

                let lexeme = self.json.substr(self.si, self.i - self.si)
                if lexeme == 'true' then
                    return self._emit(TokenType.TRUE_LITERAL, lexeme)
                else if lexeme == 'false' then
                    return self._emit(TokenType.FALSE_LITERAL, lexeme)
                else if lexeme == 'null' then
                    return self._emit(TokenType.NULL_LITERAL, lexeme)
                else
                    return self._emit(TokenType.ILLEGAL)
                end
            else if self._is_ws(self.c) then
                while self._is_ws(self.c) do self._advance() end
            else
                return self._emit(TokenType.ILLEGAL)
            end
        end
    end

    def _advance
        self.i += 1
        self.c = self.json.at(self.i)
    end

    def _emit : type, lexeme
        if !lexeme then
            lexeme = self.json.substr(self.si, self.i - self.si)
        end
        self.current = self.next
        self.next = {
            lexeme: lexeme,
            type: type
        }
        return self.current
    end

    def _advance_and_emit : type, lexeme
        self._advance()
        return self._emit(type, lexeme)
    end

    def _is_ws : c
        return [
            '\u0020',
            '\u000A',
            '\u000D',
            '\u0009'
        ].indexof(c) != -1
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
            if !self._match(TokenType.RBRACKET) then
                do
                    let val = self._parse_value()
                    arr.push(val)
                end while self._match(TokenType.COMMA)
                self._expect(TokenType.RBRACKET)
            end
            return arr
        else if token.type == TokenType.LBRACE then
            let obj = {}
            if !self._match(TokenType.RBRACE) then
                do
                    self._expect(TokenType.STRING_LITERAL)
                    let key = self.tokenizer.current.lexeme
                    self._expect(TokenType.COLON)
                    let val = self._parse_value()
                    obj[key] = val
                end while self._match(TokenType.COMMA)
                self._expect(TokenType.RBRACE)
            end
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

    def _match : type
        if self.tokenizer.next.type == type then
            self.tokenizer.scan()
            return True
        end

        return False
    end

end


def deserialize : json
    return clone Parser(json).parse()
end
