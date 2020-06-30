/*
 * Cpp Code File: mainBluetooth.cpp
 * Very small Soccer - Bluetooth Team 2015.
 * Descrição: Programa principal de comunicação que envia dados para cada robô (N robôs) atraves do bluetooth de uma workstatioN (computador).
 * Decription: Main program of communication that  send data to each robots (N robots) by bluetooth from one workstation (PC).
 *
 * autor: valbercesar@gmail.com
 * Natal, 17/09/2015, 16:00h
 */


#ifndef __BLUETOOTHACTION_H__
#define __BLUETOOTHACTION_H__

// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
#include <vector>
#include <string>
#include <sys/socket.h> /*communications stuffs by socket - also used by bluetooth */

#include <bluetooth/bluetooth.h> /*lib for bluetooth comm*/
#include <bluetooth/hci.h> /*lib for bluetooth comm*/
#include <bluetooth/hci_lib.h> /*lib for bluetooth comm*/
#include <bluetooth/rfcomm.h> /*lib for bluetooth comm*/
#include <bluetooth/l2cap.h>/*lib for bluetooth l2acp*/

/*Class body*/
/**
 * Classe de comunicação, que envia dados para cada robô (N robôs) atraves do bluetooth de uma workstatioN (computador).
 */
class BluetoothAction {
  private:
    /*constants variables*/
    int LEN_BUFFER;
    int NUM_BT_DEVICES;
    int LEN_ADDR_BT_DEVICES;

    /*socket variables*/
    struct sockaddr_rc addr; //RFCOMM
    struct sockaddr_l2 addr2;
    std::vector<std::string> dest;
    int sock[10];
    int status[10];
    std::vector<int> tmp_status;

  public:

    /*header methods*/
    BluetoothAction();
    //BluetoothAction(int len_buff = 0, int num_bt_dv = 0);
    ~BluetoothAction();
    /**
     * Envia uma mensagem para o idBt com tamanho maximo de LEN_BUFFER
     * @param  int id id do destino.
     * @param  char message mensagem que sera enviada.
     * @return      true em caso de falha, false caso contrario.
     */
    bool sendBluetoothMessage(const unsigned int, const unsigned char []);
    /**
     * Envia uma mensagem para o idBt com tamanho maximo de LEN_BUFFER
     * @param  int id id do destino.
     * @param  int  id do destino.
     * @param  char message mensagem que sera enviada.
     * @return      true em caso de falha, false caso contrario.
     */
     bool sendBluetoothMessage(const unsigned int id, const uint8_t* message, const int len_buffer);

    // int recvBluetoothMessage(const int idBt, uint8_t*buffer, int lengthMax);
    int recvBluetoothMessage(const int idBt, uint8_t*buffer, int lengthMax, int timeout = 20);
    // int recvBluetoothMessage();
    /**
     * Encerra multiplas conexoes, do id 0 até bt.
     * @param int bt último idBt fechado.
     */
    void closeBluetooths(int);
    /**
     * Encerra uma conexao.
     * @param int idBt bluetooth ID que sera fechado.
     */
    void closeBluetoothById(int);
    /**
     * Metodo para se conectar com um dos MAC destinos por meio do ID retornado pelo
     * metodo setBluetoothAddr.
     * @param  int idBt id do destino.
     */
    void initBluetoothById(int);
    /**
     * Tenta realizar conexões com todos os NUM_BT_DEVICES adicionados.
     */
    void initBluetoothDevices();//init devices based RFCOMM protocol
    /**
     * [initBluetoothDevicesL2CAP description]
     * @param  int [description]
     * @return     [description]
     */
    int initBluetoothDevicesL2CAP(int);//init devices based L2CAP protocol
    /**
     * [findActiveBluetoothDevice description]
     * @return  [description]
     */
    int findActiveBluetoothDevice(void);
    //getters
    /**
     * [getDest description]
     * @return  [description]
     */
    std::vector<std::string> getDest(void);
    /**
     * [getLenBuffer description]
     * @return  [description]
     */
    int getLenBuffer(void);
    /**
     * [getNumBTDevices description]
     * @return  [description]
     */
    int getNumBTDevices(void);
    /**
     * [getLenAddrBTDevices description]
     * @return  [description]
     */
    int getLenAddrBTDevices(void);
    /**
     * [getBluetoothAddr description]
     * @param  int [description]
     * @return     [description]
     */
    std::string getBluetoothAddr(int);

    /*setters*/
    void setLenBuffer(int);
    /**
     * [setNumBTDevices description]
     * @param int [description]
     */
    void setNumBTDevices(int);
    /**
     * [setLenAddrBTDevices description]
     * @param int [description]
     */
    void setLenAddrBTDevices(int);
    /**
     * Adiciona um novo endereco de destino.
     * @param std::string Endereco MAC do destino.
     * @return id do sock.
     */
    int setBluetoothAddr(std::string);
    /**
     * [getStatus description]
     * @return [description]
     */
    std::vector<int> getStatus();
};
#endif
