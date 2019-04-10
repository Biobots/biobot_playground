import nltk
from nltk.tokenize import sent_tokenize, word_tokenize

test = "Hello Mr.Adam, how are you? I hope everything is going well. Today is a good day, see you dude."

print(sent_tokenize(test))
print(word_tokenize(test))


from nltk.corpus import wordnet

syn = wordnet.synsets("depress")
print(syn[0].definition())
print(syn[0].examples())


from nltk.tokenize import WhitespaceTokenizer

t = WhitespaceTokenizer().tokenize(test)
print(t)


from tokenize1 import tkn1

tagged = nltk.pos_tag(tkn1(), tagset='universal', lang='eng')
tagged_verb = list()
for key,val in tagged:
    if val == 'VERB':
        tagged_verb.append((key,val))
print(tagged_verb)
text_fd = nltk.FreqDist(val for (key, val) in tagged)
print(text_fd.most_common())
