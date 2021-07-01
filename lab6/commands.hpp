//#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>
#include <sys/types.h>
#include <signal.h>
#include <cstring>
#include <vector>
#include <map>
#include <time.h>
#include <vector>

std::string swich = "swich";

// send message to the particular socket
bool sendM(zmq::socket_t &socket, const std::string &message_string)
{
    //printf("Doing\n");
    // message size init
    zmq::message_t message(message_string.size());
    // message content init
    //printf("Done 1\n");
    memcpy(message.data(), message_string.c_str(), message_string.size());
    //std::cout<<"Size = "<<message_string.size()<<" Message = "<< message_string.c_str()<<std::endl;
    //printf("Done 2\n");
    return socket.send(message);
}

std::string receiveMessage(zmq::socket_t &socket)
{
    zmq::message_t message;
    int recResult;
    // receiving message from socket 
    try {
        recResult = (int)socket.recv(&message);
        if (recResult < 0) {
            perror("socket.recv()");
            exit(1);
        }
    }
    catch (...) {
        recResult = 0;
    }
    // transform to string
    std::string recieved_message((char *)message.data(), message.size());
    //std::cout<<"Closer to message: "<<recieved_message<<std::endl;
    if (recieved_message.empty() || !recResult) {
        //printf("| m.size = %d, recResult = %d |\n",recieved_message.size(), (int)recResult);
        return "Error: Node is not available";
    }
    return recieved_message;
}

std::string getPortName(int port)
{
    return "tcp://127.0.0.1:" + std::to_string(port);
}

int bindSocket(zmq::socket_t &socket, int port = 8080)
{
    //int port = 8080;
    // create endpoint and bind it to the socket
    while (true) {
        try {
            socket.bind(getPortName(port));
            //printf("%d: %d - Free bind\n", getpid(), port);
            break;
        }
        catch (...) {
            //printf("%d: %d - Занято bind\n", getpid(), port);
            port++;
        }
    }
    return port;
}

int connectSocket(zmq::socket_t &socket, int port = 8080)
{
    //int port = 8080;
    // create endpoint and bind it to the socket
    while (true) {
        try {
            socket.connect(getPortName(port));
            //printf("%d: %d - Free connect\n", getpid(), port);
            break;
        }
        catch (...) {
            //printf("%d: %d - Занято connect\n", getpid(), port);
            port++;
        }
    }
    return port;
}

std::string word(std::string &str){
    
    int size = str.size();
    //printf("size = %d\n", size);
    if(size != 0){
        std::string word, newstr("");
        int i = 0;
        while((str[i] != ' ')&&(size - i != 0)){
            //printf("%c[%d]\n",str[i], i);
            word += str[i];
            i++;
        }
        //printf("Newline\n");
        i++;
        while(size - i > 0){
            //printf("%c[%d]\n",str[i], i);
            newstr += str[i];
            i++;
        }
        //std::cout<<newstr<<std::endl;
        str = newstr;
        return word;
    }
    std::string s(" ");
    return s;
}

template<typename T>
bool inVector(T n, std::vector<T> vect){
    for (int i = 0; i < vect.size(); i++)
    {
        if(vect[i] == n){
            return true;
        }
    }
    return false;
}

void getinfo(std::string const msg, std::string &cmd, int &id, int &pid){
    std::string nm = msg;
    cmd = word(nm);
    id = atoi(word(nm).c_str());
	pid = atoi(word(nm).c_str());
    return;
}
template<class T>
T find(T id, std::vector<T> vec){
    int i;
    for(i = 0;i < vec.size();i++){
        if(vec[i] == id){
            return i;
        }
    }
    return i;
}
