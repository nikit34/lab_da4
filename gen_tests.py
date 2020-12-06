import numpy as np
import string
from tqdm import tqdm


test_data = ['CAT', 'cat', 'dog', 'Dog', 'CAT', 'Dog', 'T', 'cAT', 'dog', 'Dog', 'Cat', 'cat', 'dog', 'Dog', 'CAT', 'sausage', 'lEna', 'x', 'Cat']

with open('test1.txt', 'w') as f:
    for _ in tqdm(range(10000)):
        line = ''
        for _ in range(np.random.randint(10) + 5):
            line += str(np.random.choice(test_data, 1)[0]) + ' '
        f.write(line + '\n')
