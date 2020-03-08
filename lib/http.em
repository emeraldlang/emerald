import core
import net


let HttpVerb = {
    GET: 'GET',
    POST: 'POST',
}


object HttpClient

    let _version_str = 'HTTP/1.1'

    def __init__
        self.cached_connections = {}
    end

    def GET : host, url, headers={}
        return self.send(HttpVerb.GET, host, headers)
    end

    def POST : host, url, headers={}
        return self.send(HttpVerb.POST, host, headers)
    end

    def send : verb, host, url, headers={}, body=None
        let request = '{0} {1} {2}\r\n'.format(
            verb,
            url,
            self._version_str)

        for let key in headers.keys() do
            request += '{0}:{1}\r\n'.format(key, headers[key])
        end

        if body then
            request += body
        end

        let client = self.cached_connections.get_prop(host)
        if !client then
            let addresses = net.resolve(host)
            for let address in addresses do
                let endpoint = clone net.IPEndpoint(address, 80)
                client = clone net.TcpClient()
                if client.connect(endpoint) then
                    self.cached_connections[host] = client
                    break
                end
            end

            if !client then
                throw clone core.Exception()
            end
        end

        client.write(request)
    end

end
