import core
import datetime


let date = clone datetime.Date(2020, 3, 20)
core.print(date) # 2020-Mar-20

let duration = clone datetime.TimeDuration(1, 20, 33, 543)
core.print(duration) # 01:20:33.543

let time = clone datetime.Time(date, duration)
core.print(time) # 2020-Mar-20 01:20:33.543

core.print(datetime.local_time())
core.print(datetime.universal_time())
