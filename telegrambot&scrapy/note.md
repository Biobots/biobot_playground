# urllib
## opener & handler
- openers：opener可以想象成一瓶水的瓶盖，但是真正处理信息（能让你解渴）的是handler（瓶子里面的东西）
- handlers：解决问题，工作都由它完成。你可以想象成瓶子里面的是果汁可以给小朋友补充维生素c，可以是运动饮料给运动员补充体能，不同的饮料解决不同的需求，不同的handlers可以了解用什么协议打开什么url。每个handlers知道如何通过特定协议打开URLs，或者如何处理URL打开时的各个方面，例如HTTP重定向或者HTTP cookies。

创建支持特定功能的handler，并根据这个handler创建相应的opener，使用这个opener来打开request，默认情况下相当于urlopen函数

``` python
request = urllib.request.Request(url,data, headers or {})
opener = self.opener or urllib.request.build_opener(urllib.request.ProxyHandler)
```