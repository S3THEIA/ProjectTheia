import random
from mnist import MNIST

mndata = MNIST('data')

#images, labels = mndata.load_training()
# or
images, labels = mndata.load_testing()

#index = random.randrange(0, len(images))  # choose an index ;-)
for i in range(3):
    #print(mndata.display(images[i]))
    #print()
    for k in range(28):
        for j in range(28):
            print(images[i][k*28+j], end=' ')
            # if images[i][k*28+j] == 0:
            #     print('.', end='')
            # else:
            #     print('▢', end='')
        print()
    print()