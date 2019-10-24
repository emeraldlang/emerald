import core


let _default_config = {
    delimiter: ',',
    has_header: False
}


object CsvReader

    def __init__ : self, stream, config = {}
        self.stream = stream
        self.config = core.extend({}, _default_config, config)
        self.headers = None
        self.records = []
    end

    def read : self
        if self.config.has_header &&
                self.headers != None then
            self.headers = self.stream
                .readline()
                .split(self.config.delimiter)
        end

        let row = self.stream
            .readline()
            .split(self.config.delimiter)
        let n = row.size()
        if self.headers != None then
            let res = {}
            for let i = 0 to n do
                res[self.headers.at(i)] = row.at(i)
            end
        else
            let res = []
            for let i = 0 to n do
                res.push(row.at(i))
            end
        end

        self.records.push(res)
        return res
    end

    def __str__ : self
        return '<csv_reader>'
    end

end


object CsvWriter

    def __init__ : self, stream, config = {}
        self.stream = stream
        self.config = core.extend({}, _default_config, config)
        self.headers = None
        self.records = []
    end

    def write_headers : self, headers
        self.headers = headers
    end

    def write : self, record
        self.records.push(record)
    end

    def flush : self
        let buffer = ''
        if self.headers != None then
            for let i = 0 to self.records.size() do
                let records = []
                for let j = 0 to self.headers.size() do
                    let column = self.records.at(i)[self.headers[i]]
                    records.push(column)
                end
                buffer.append(records.join(','))
            end
        else
            for let i = 0 to self.records.size() do
                let row = self.records
                    .at(i)
                    .join(self.config.delimiter)
                buffer.append(row)
            end
        end

        self.stream.write(buffer)
        self.records.clear()
    end

    def __str__ : self
        return '<csv_writer>'
    end

end
