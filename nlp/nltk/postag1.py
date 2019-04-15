from nltk.corpus import treebank
from nltk.tag import tnt

train_data = treebank.tagged_sents()[:3000]
test_data = treebank.tagged_sents()[3000:]

tnt_pos_tagger = tnt.TnT()
tnt_pos_tagger.train(train_data)
result = tnt_pos_tagger.evaluate(test_data)

print(result)