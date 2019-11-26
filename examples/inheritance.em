import core


let foo = {
  name: 'foo',
  one: 1,
  two: 2
}

let bar = clone foo
bar.two = 'two'
bar.three = 3

core.print(bar.one) # 1
core.print(bar.three) # 3
core.print(bar.two) # 'two'
core.print(bar.name) # 'foo'
core.print(foo.name) # 'foo'
