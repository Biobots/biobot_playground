#计算最小公倍数，最大公约数
def Cal_GCD_LCM():
    x = int(input('x='))
    y = int(input('y='))
    if x > y:
        (x, y) = (y, x)
    for factor in range(x, 0, -1):
        if x % factor == 0 and y % factor == 0:
            print('GCD:%d\n' % (factor))
            print('LCM:%d\n' % (x * y / factor))
            break
    return

#判断素数
from math import sqrt
def isPrime():
    num = int(input('num='))
    maxdivisor = int(sqrt(num))
    is_prime = True
    for x in range(2, maxdivisor + 1):
        if num % x == 0:
            is_prime = False
            break
    #(num == 1) ? print(False) : print(is_prime) python无此类三目运算
    print(False if(num == 1) else is_prime)
    return

isPrime()
