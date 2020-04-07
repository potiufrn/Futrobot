# https://matplotlib.org/3.1.1/users/installing.html
# Codigo em python3
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import numpy as np
import os

ref  = 1.0

def func(x,K,a):
    return K*(1.0 - np.exp(-a*x))

def linearReg(x, y):
  Sxx = 0.0
  Sxy = 0.0

  n = len(x)

  for i in range(0,n):
    Sxx = Sxx +  x[i]*x[i];
    Sxy = Sxy +  x[i]*y[i];
  return Sxy/Sxx

def print_analizy(name, ref, K, tau, x, y):
    var = ((y[300:] - K)**2).sum()/(len(y[300:]) - 1.0)
    print(15*'#',name,15*'#')
    print('Ganho do sistema              | {0:.8f}'.format(K/ref))
    print('Tempo de subida(5% - 95%)[ms] | {0:.8f} ms'.format(2.95*tau*1000.0))
    print('Tempo de acomodação(5%)[ms]   | {0:.8f} ms'.format(3.0*tau*1000.0))
    print('Polo do sistema               | {0:.8f}'.format(-1.0/tau))
    print('Variancia                     | {0:.8f}'.format(var))
    print((32+len(name))*'#', '\n')

for p,_,files in os.walk('etc'):
    pass
files.remove('_pyplotter.py')
if len(files) == 0:
    print("Nao ha dados para visualizar... Faça a aquisição de dados primeiro.\n")
    os.system('pause')
    exit()

files.sort()
for i in range(0,len(files)):
    print(i, '- ', files[i])

# OBS.: Para o caso de mais de um grafico o primeiro index informado sera o utilizado para a interpolacao
index = list(map(lambda x:int(x), (input("index do(s) arquivo(s) para plotter?\t").split(' '))))

os.system('clear')

first = True
for plotID in index:
    dados = open(p+'/'+files[plotID],'r').read().split(',')
    dados = list(map(float, dados))
    name  = files[plotID].replace('_', ' ').replace('.out','')

    size    = int(dados[0])
    timeout = dados[1]
    omegaRef= dados[2]

    t = np.linspace(0,timeout, size-1)
    y = np.array(dados[3:])

    plt.plot(t,y,label= name)

    if first:
        popt, pcov = curve_fit(func, t, y, bounds=([-10000., 0.1], [10000., 1.0/0.001]))
        K  = popt[0]
        a  = popt[1]
        y_reg = func(t, K, a);
        plt.plot(t, y_reg, 'k--', label=r'$\omega(t) = %.3f[1 - \exp(-\frac{1}{%.3f}t)]$'%(K,1.0/a))
        print_analizy(name,ref, K, 1.0/a, t, y);
        # first = False


plt.plot(t, np.full(t.size,omegaRef), label=r'$\omega_{ref} = %.3f$'%(omegaRef))

plt.xlabel('t(s)')
plt.ylabel(r'$\omega(rad/s)$')
plt.grid()
plt.legend()
plt.title("")
plt.xlim(0,t[-1]);
plt.show()
