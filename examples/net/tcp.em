import net


let endpoint = clone net.IPEndpoint(clone net.IPAddress('127.0.0.1'), 8000)
let listener = clone net.TcpListener(endpoint)

listener.start()

let client = listener.accept()
let msg = client.read(5)

if msg == 'hello' then
    client.write('hello!')
else
    client.write('i don\'t understand')
end

listener.stop()
