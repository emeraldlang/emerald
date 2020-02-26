import core
import json


let obj = json.deserialize('{"test": "hello"}')
core.print(obj.test) # 'hello'
