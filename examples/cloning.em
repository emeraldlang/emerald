import core

object Test clones core.Array
    def __init__ : self
        print 'hello'
    end
end

let t = clone Test
print t
