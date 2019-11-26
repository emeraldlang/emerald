import core


def collatz : n
  let c = 1
  while (n != 1) do
    if (n % 2 == 0) then
      n = n / 2
    else
      n = 3 * n + 1
    end
    c += 1
  end
  return c
end

core.print(collatz(13))
