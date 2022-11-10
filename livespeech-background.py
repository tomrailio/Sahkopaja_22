# this is a test of pocketsphinx live speech
# recognizes words listed in 'keyphrases.txt' in background
# pocketsphinx does not use an internet connection and may be faster than google
# parameter kws_threshold controls recognition sensitivity, range of 1e-6 to 1e-10 seems OK
# maxwpf=1 prevents the program from recognizing more than 1 word 

from pocketsphinx import LiveSpeech

speech = LiveSpeech(kws='keyphrases.txt',
                    kws_threshold=5e-9,
                    remove_noise=True,
                    maxwpf=1
                    )

for phrase in speech:
    print(phrase)
