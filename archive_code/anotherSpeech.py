from pocketsphinx import LiveSpeech

def visiiri(text):
    sliced = text.split(' ')
    # OPEN UP and CLOSE IT are right now the most recognizable words
    for word in sliced:
        if word in ['open', 'pop', 'up', 'bob', 'op', 'oh', 'and', 'huh', 'ha']:
            return "OPEN"
        elif word in ['close', 'down', 'now', 'those', 'don\'t', 'not', 'who', 'knows', 'dallas', ]:
            return "CLOSE"
        else:
            return "UNKNOWN"
for phrase in LiveSpeech():
    print(phrase)
    print(visiiri(str(phrase)))

