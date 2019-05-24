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

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> /*communications stuffs by socket - also used by bluetooth */
#include <bluetooth/bluetooth.h> /*lib for bluetooth comm*/
#include <bluetooth/hci.h> /*lib for bluetooth comm*/
#include <bluetooth/hci_lib.h> /*lib for bluetooth comm*/
#include <bluetooth/rfcomm.h> /*lib for bluetooth comm*/
#include <bluetooth/l2cap.h>/*lib for bluetooth l2acp*/
#include <iostream>
using namespace std;

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
    vector<string> dest;
    int sock[10];
    int status[10];
    vector<int> tmp_status;

  public:

    /*header methods*/
    BluetoothAction();
    //BluetoothAction(int len_buff = 0, int num_bt_dv = 0);
    ~BluetoothAction();
    /**
     * [sendBluetoothMessage description]
     * @param  int  [description]
     * @param  char [description]
     * @return      [description]
     */
    bool sendBluetoothMessage(const unsigned int, const unsigned char []);
    /**
     * [closeBluetooths description]
     * @param int [description]
     */
    void closeBluetooths(int);
    /**
     * [initBluetoothById description]
     * @param  int [description]
     * @return     [description]
     */
    int initBluetoothById(int);
    /**
     * [initBluetoothDevices description]
     * @param  int [description]
     * @return     [description]
     */
    int initBluetoothDevices(int);//init devices based RFCOMM protocol
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
    vector<string> getDest(void);
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
    string getBluetoothAddr(int);

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
     * [setBluetoothAddr description]
     * @param string [description]
     */
    void setBluetoothAddr(string);
    /**
     * [getStatus description]
     * @return [description]
     */
    vector<int> getStatus();
};

#endif
