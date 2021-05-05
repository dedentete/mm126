this = open('scores.txt', 'r')
lines = this.readlines()
scores = []
for line in lines:
    score = line.split()
    scores.append(float(score[2]))
compare = open('compare.txt', 'r')
lines = compare.readlines()
avg = 0.0
i = 0
for line in lines:
    score = line.split()
    avg += scores[i] / float(score[2])
    i += 1
print(avg / i)
