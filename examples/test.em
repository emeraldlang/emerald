import core

object Test
    def __init__ : self
        self.x = 5
        return self
    end

    def __eq__ : self, other
        return self.x == other.x
    end

    def __neq__ : self, other
        return !(self.__eq__(other))
    end

    def test : self
        print self.x
    end
end

let x = clone Test
let y = clone Test

print x != y
x.y = 7

print x.y

let a = clone core.Array
for let i = 0 to 100 do
    print a.push(i)
end

print a
