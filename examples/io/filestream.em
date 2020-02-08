import core
import io


let file_name = 'examples/helloworld.em'
let fs = clone io.FileStream
if fs.open(file_name, io.FileAccess.read) then
    core.print(fs.read())
else
    core.print('could not open file: {0}'.format(file_name))
end
