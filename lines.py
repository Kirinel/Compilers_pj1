import os

path = 'src/'

lines = 0

for filename in os.listdir(path):
    with open(path+filename) as f:
        for i in f:
            lines += 1

print("calculated lines:", lines)
