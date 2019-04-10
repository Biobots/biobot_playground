from urllib import request
from bs4 import BeautifulSoup
import nltk
from nltk.corpus import stopwords

res = request.urlopen("http://php.net")
html = res.read()
soup = BeautifulSoup(html, "html5lib")
text = soup.get_text(strip=True)
tokens = text.split()
clean_tokens = list()
sr = stopwords.words("english")
for token in tokens:
    if token not in sr:
        clean_tokens.append(token)
freq = nltk.FreqDist(clean_tokens)
for key,value in freq.items():
    print(str(key)+":"+str(value))
freq.plot(20, cumulative=False)
