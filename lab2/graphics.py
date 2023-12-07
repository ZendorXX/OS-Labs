import subprocess
import matplotlib.pyplot as plt

threads_counts = []
execution_time = []
acceleration = []
efficiency = []

max_thread_count = int(input('Введите количество потоков: '))

f = open('execution_data.txt', 'w')

#subprocess.run(['make'], universal_newlines=True)
for i in range(1, max_thread_count + 1):
    output = subprocess.check_output(['./main.out', str(i)], universal_newlines=True)

    exec_time = float(output.split('\n')[0])

    print(i)

    threads_counts.append(i)
    execution_time.append(exec_time)
    acceleration_value = execution_time[0] / exec_time
    acceleration.append(acceleration_value)
    efficiency.append(acceleration_value / i)

    f.write(f'{threads_counts[i - 1]} {execution_time[i - 1]} {acceleration[i - 1]} {efficiency[i - 1]}\n')

f.close()

plt.plot(threads_counts, execution_time, marker='o', linestyle='-')
plt.title('Time taken')

plt.xlabel('Threads')
plt.ylabel('Time taken, seconds')

plt.savefig('plot_execution_time.png')

plt.figure()

plt.plot(threads_counts, acceleration, marker='o', linestyle='-')
plt.title('Acceleration')

plt.xlabel('Threads')
plt.ylabel('Acceleration')

plt.savefig('plot_acceleration.png')

plt.figure()

plt.plot(threads_counts, efficiency, marker='o', linestyle='-')
plt.title('Efficiency')

plt.xlabel('Threads')
plt.ylabel('Efficiency')

plt.savefig('plot_efficiency.png')

plt.show()
    