# urllib
## opener & handler
- openers��opener���������һƿˮ��ƿ�ǣ���������������Ϣ���������ʣ�����handler��ƿ������Ķ�����
- handlers��������⣬������������ɡ�����������ƿ��������ǹ�֭���Ը�С���Ѳ���ά����c���������˶����ϸ��˶�Ա�������ܣ���ͬ�����Ͻ����ͬ�����󣬲�ͬ��handlers�����˽���ʲôЭ���ʲôurl��ÿ��handlers֪�����ͨ���ض�Э���URLs��������δ���URL��ʱ�ĸ������棬����HTTP�ض������HTTP cookies��

����֧���ض����ܵ�handler�����������handler������Ӧ��opener��ʹ�����opener����request��Ĭ��������൱��urlopen����

``` python
request = urllib.request.Request(url,data, headers or {})
opener = self.opener or urllib.request.build_opener(urllib.request.ProxyHandler)
```