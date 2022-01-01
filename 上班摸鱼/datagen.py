# 用来生产测试数据的脚本
import random

# configuration
size = int(1e5)
m_size = int(1e5)
xlim = int(1e8)
ylim = int(1e8)
file_name = 'data1.txt'
to_plot = False  # whether to plot the data after generating

f = open(file_name, 'w')

# generate data
x = [random.randint(0, xlim) for i in range(size)]
y = [random.randint(0, ylim) for i in range(size)]
target_x = [random.randint(0, xlim) for i in range(m_size)]
target_y = [random.randint(0, ylim) for i in range(m_size)]
k = [random.randint(1, size) for i in range(m_size)]


# write data to file
f.write(str(size) + ' ' + str(m_size) + '\n')
for i in range(size):
    f.write(str(x[i]) + ' ' + str(y[i]) + '\n')
for i in range(m_size):
    f.write(str(target_x[i]) + ' ' + str(target_y[i]) + ' ' + str(k[i]) + '\n')


# plot the generated data
if to_plot is True:
    import matplotlib.pyplot as plt
    plt.scatter(x, y)
    plt.scatter(target_x, target_y)
    plt.show()
