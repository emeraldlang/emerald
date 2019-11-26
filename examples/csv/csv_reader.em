import core
import csv
import io


let file_name = 'examples/csv/data.csv'
let stream = clone io.FileStream
stream.open(file_name, io.FileAccess.read)
if stream.is_open() then
    try
        let reader = clone csv.CsvReader(stream, {
            has_header: True
        })
        core.print(reader.read())
    catch exc
        core.print(exc)
    end
else
    core.print('error opening file {0}'.format(file_name))
end
