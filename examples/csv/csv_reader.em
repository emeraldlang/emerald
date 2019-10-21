import csv
import io


let file_name = 'examples/csv/data.csv'
let stream = clone io.FileStream
stream.open(file_name, io.FileAccess.read)
if stream.is_open() then
    let reader = clone csv.CsvReader(stream, {
        has_header: True
    })
    reader.read()
else
    print 'error opening file {0}'.format(file_name)
end
