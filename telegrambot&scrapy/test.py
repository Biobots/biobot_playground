from http import cookiejar
from urllib import request

cookie = cookiejar.CookieJar()
handler = request.HTTPCookieProcessor(cookie)
opener = request.build_opener(handler)
res = opener.open('http://www.baidu.com')

for item in cookie:
    print(item.name+':'+item.value)
