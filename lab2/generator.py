from random import randint

MIN = -1e9
MAX = 1e9

size = int(input('Введите размер массива: '))

out = open('array.txt', 'w')

out.write(f'{size}\n')
for _ in range(size):
   out.write(f'{randint(MIN, MAX)} ')
out.close()
