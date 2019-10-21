import core


let _default_config = {
    delimiter: ',',
    has_header: False
}


object CsvReader

    def __init__ : self, stream, config = {}
        self.stream = stream
        self.config = core.extend({}, _default_config, config)
        self.headers = []
        self.records = []
    end

    def read : self
        if self.config.has_header &&
                self.headers.empty() then
            self.headers = self.stream
                .readline()
                .split(self.config.delimiter)
        end

        let row = self.stream
            .readline()
            .split(self.config.delimiter)
        let n = row.size()
        if self.headers.empty() then
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
