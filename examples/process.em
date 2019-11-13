import core
import process

let parent = process.id()

def work
    # do some work
    process.send(parent, 'hello world!')
end

let pid = process.create(work)
print 'message from process<{0}>: '.format(pid)
print process.receive()
