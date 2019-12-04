# https://matplotlib.org/3.1.1/users/installing.html
# Codigo em python3
import numpy as np
import matplotlib.pyplot as plt
import os

for p,_,files in os.walk('.'):
    pass
files.remove('_pyplotter.py')


if len(files) == 0:
    print("Nao ha dados para visualizar... Faça a aquisição de dados primeiro.\n")
    exit()
for i in range(0,len(files)):
    print(i, '- ', files[i])
index_file = int(input("index do arquivo para plotter?\t"))

print(p)
dados  = open(p+'/'+files[index_file],'r').read().split(',')
for i in range(0,len(dados)):
    dados[i] = float(dados[i])

size    = int(dados[0])
timeout = dados[1]
x = np.linspace(0,timeout, size)
y = dados[3:]

print("Timeout:", timeout," size:", size, "\n")

plt.plot(x,y, 'b')
plt.xlabel('t(s)')
plt.ylabel('Omega(rad/s)')
plt.grid();
plt.title(str(files[index_file]))
plt.show()
