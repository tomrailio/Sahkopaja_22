# this is a test of pocketsphinx live speech

from pocketsphinx import LiveSpeech

speech = LiveSpeech(kws='keyphrases.txt',
                    kws_threshold=5e-9,
                    remove_noise=True,
                    maxwpf=1
                    )

for phrase in speech:
    print(phrase)
