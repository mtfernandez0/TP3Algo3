import numpy as np

np.random.seed(1337)

for power in range(6, 12):
    n = 2 ** power
    f = open("inputs/input_ralos_" + str(n), "w")
    f.write(str(1) + "\n")

    # m = np.random.randint(((n - 1) * n / 2) - (n % 5), (n-1)*n/2)
    m = np.random.randint(n - (n % 5), n + (n % 5))
    k = np.random.randint(1, 300)
    s = np.random.randint(1, n + 1)
    t = np.random.choice([i for i in range(1, n + 1) if i != s])
    f.write(str(n) + " " + str(m) + " " + str(k) + " " + str(s) + " " + str(t) + "\n")

    edges = [[-1] * (n) for _ in range(n)]
    count = 0
    while count < m:
        u = np.random.randint(1, n + 1)
        v = np.random.choice([i for i in range(1, n + 1) if i != u])
        l = np.random.randint(1, 1001)
        if edges[u-1][v-1] == -1:
            edges[u-1][v-1] = 0
            f.write(str(u) + " " + str(v) + " " + str(l) + "\n")
            count = count + 1
        elif edges[v-1][u-1] == -1:
            edges[v-1][u-1] = 0
            f.write(str(v) + " " + str(u) + " " + str(l) + "\n")
            count = count + 1

    while k > 0:
        u = np.random.randint(1, n + 1)
        v = np.random.choice([i for i in range(1, n + 1) if i != u])
        l = np.random.randint(1, 1001)
        f.write(str(u) + " " + str(v) + " " + str(l) + "\n")
        k = k - 1

    f.close()