import core


def additive_sequence : n, t0, t1
  if (n == 0) then return t0 end
  if (n == 1) then return t1 end
  return additive_sequence(n - 1, t1, t0 + t1)
end

def fib : n
  return additive_sequence(n, 0, 1)
end

for let i = 1 to 10 do
    core.print('fib({0}) = {1}'.format(i, fib(i)))
end
