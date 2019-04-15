from nltk.util import ngrams
from nltk.corpus import RegexpTokenizer

tokenizer = RegexpTokenizer("[a-zA-Z'`éèî]+")
sent = "le temps est un grand maître, dit-on, le malheur est qu'il tue ses élèves."
s_tokenized = tokenizer.tokenize(sent)
print(s_tokenized)

generated_grams = []
for word in s_tokenized:
    generated_grams.append(list(ngrams(word, 3, pad_left=True, pad_right=True, left_pad_symbol="_", right_pad_symbol="_")))
print(generated_grams)

#flatten
generated_grams = [word for sublist in generated_grams for word in sublist]
print(generated_grams)

ng_list = generated_grams
for idx, val in enumerate(generated_grams):
    ng_list[idx] = ''.join(val)
print(ng_list)

freq_grams = {}
for ngram in ng_list:
    if ngram not in freq_grams:
        freq_grams.update({ngram: 1})
    else:
        count = freq_grams[ngram]
        freq_grams.update({ngram: count + 1})

from operator import itemgetter
freq_grams_sorted = sorted(freq_grams.items(), key=itemgetter(1), reverse=True)
print(freq_grams_sorted)