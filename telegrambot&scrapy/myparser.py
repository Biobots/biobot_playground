#import re
from urllib import request
from urllib import parse
import http.cookiejar

def module_test(input):
    print("f**k", input)

def getRes(url):
    data = bytes(parse.urlencode({'word': 'test'}), encoding='utf-8')
    headers = {
        'User-Agent': 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
    }
    req = request.Request(url, data=data, headers=headers)
    print(data)
    res = request.urlopen(req)
    print(res.read())
    return res