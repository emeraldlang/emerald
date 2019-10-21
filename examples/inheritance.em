let foo = {
  name: 'foo',
  one: 1,
  two: 2
}

let bar = clone foo
bar.two = 'two'
bar.three = 3

print bar.one # 1
print bar.three # 3
print bar.two # 'two'
print bar.name # 'foo'
print foo.name # 'foo'
