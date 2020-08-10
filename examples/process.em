import core
import process

let parent = process.id()

def work : arg1, arg2
    # do some work
    core.print('received args: {0}, {1}'.format(arg1, arg2))
    process.send(parent, 'hello world!')
end

let pid = process.create(work, 1.5, 'some string')
core.print('message from process<{0}>: {1}'.format(pid, process.receive()))
process.join(pid)
