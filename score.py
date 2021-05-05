f = open('scores.txt', 'r')
lines = f.readlines()
ans = 0.0
for line in lines:
    t = line.split()
    ans += float(t[2])
print(ans)