# https://matplotlib.org/3.1.1/users/installing.html
# Codigo em python3
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import numpy as np
import os

omega_max = 2214.822754
omega_ref = 0.5*omega_max

def func(x,K,a):
    return K*(1.0 - np.exp(-a*x))

for p,_,files in os.walk('etc'):
    pass
files.remove('_pyplotter.py')
if len(files) == 0:
    print("Nao ha dados para visualizar... Faça a aquisição de dados primeiro.\n")
    exit()

files.sort()
for i in range(0,len(files)):
    print(i, '- ', files[i])

# OBS.: Para o caso de mais de um grafico o primeiro index informado sera o utilizado para a interpolacao
index = list(map(lambda x:int(x), (input("index do(s) arquivo(s) para plotter?\t").split(' '))))

first = True
for plotID in index:

    dados = open(p+'/'+files[plotID],'r').read().split(',')
    dados = list(map(float, dados))

    size    = int(dados[0])
    timeout = dados[1]

    t = np.linspace(0,timeout, size)
    y = np.array(dados[3:])

    if first:
        popt, pcov = curve_fit(func, t, y, bounds=([-10000., 0.1], [10000., 1.0/0.001]))

        K = popt[0]
        a = popt[1]

        y_reg = func(t, K, a);
        plt.plot(t, y_reg, 'k--', label=r'$\omega(t) = %.3f[1 - \exp(-\frac{1}{%.3f}t)]$'%(K,1.0/a))
        # first = False

    plt.plot(t,y, label= files[plotID].replace('_', ' ').replace('.out',''))


plt.plot(t, np.full(t.size,omega_ref), label='referência')

plt.xlabel('t(s)')
plt.ylabel(r'$\omega(rad/s)$')
plt.grid()
plt.legend()
# plt.title(str(files[index_file]).replace('_',' ').split('.')[0])
plt.xlim(0,t[-1]);
plt.show()
