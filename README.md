# Universidade Federal do Rio Grande do Norte - Departamento de Engenharia de Computação e Automação - DCA

# Time Poti de Futebol de Robôs

> Para utilização do sistema do Time Poti é necessário a utilização de um sistema operacional Linux. 

> A manipulação do sistema é feita via Terminal do Linux, tendo a interface gráfica apenas o módulo de calibração.

Getting Started
---------------

### Instalação de bibliotecas e pacotes

Inicialmente, é interessante que seu sistemas esteja atualizado. Para isso, deve ser utilizado o comando:

```
  sudo apt-get update
```

Use o seguinte comando para a instalação de pacotes essenciais
```
sudo apt-get install build-essential
```

Comando para instalação de pacotes para utilização do bluetooth
```
sudo apt-get install libbluetooth-dev
```

Deve ser instalada a versão 4 do Qt, logo, basta utilizar o comando a seguir:
```
sudo apt-get install libqt4-dev qt4-dev-tools
```

### Make ...
Agora, utilizamos o comando a seguir para compilar o programa
```
make
```

Após essas etapas, você pode fazer a `calibração` do campo com seus objetos ou fazer um jogo. 
Caso opte em calibração, deve-se verificar se a câmera a ser usada está referenciada corretamente.
```
### Calibração e Jogo

Para fazer a calibração, basta executar em `program/calibrador`
```
./calibrador
```

Ou 

Para fazer um jogo, basta executar em `program/main`
```
./main
```
> Divirta-se!!
