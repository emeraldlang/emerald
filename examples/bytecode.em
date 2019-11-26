import bytecode
import core


def some_func
    let x = 'hello'
    let y = 5
end

for let opcode in bytecode.bytecode(some_func) do
    core.print(opcode.opname)
end
