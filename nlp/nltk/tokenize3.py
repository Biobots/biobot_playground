from nltk.corpus import wordnet
antonyms = []
for syn in wordnet.synsets("small"):
    #print(syn.lemmas())
    for l in syn.lemmas():
        if l.antonyms():
            print(l)
            print(l.antonyms())
            antonyms.append(l.antonyms()[0].name())
print(antonyms)