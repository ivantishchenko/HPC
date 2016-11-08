import matplotlib.pyplot as plt

max_x = []
min_x = []
avg_x = []
cores_y = []

#max min avg
with open('build/data.txt', 'r') as f:
    content = f.readlines()
    for each_line in content:
        words = each_line.split()
        cores_y.append(words[0])
        max_x.append(words[1])
        min_x.append(words[2])
        avg_x.append(words[3])
    print(min_x)

    labels = ['max', 'min', 'avg']
    #plt.legend(labels)
    
    plt.plot(cores_y, max_x,'ro', label =labels[0])
    plt.plot(cores_y, min_x,'go', label =labels[1])
    plt.plot( cores_y, avg_x, 'bo', label =labels[2])
    plt.legend()
    
    plt.show()
f.closed