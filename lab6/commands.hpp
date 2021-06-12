//#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>
#include <sys/types.h>
#include <signal.h>

// send message to the particular socket
bool sendM(zmq::socket_t &socket, const std::string &message_string)
{
    //printf("Doing\n");
    // message size init
    zmq::message_t message(message_string.size());
    // message content init
    //printf("Done 1\n");
    memcpy(message.data(), message_string.c_str(), message_string.size());
    std::cout<<"Size = "<<message_string.size()<<" Message = "<< message_string.c_str()<<std::endl;
    printf("Done 2\n");
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

int bindSocket(zmq::socket_t &socket)
{
    int port = 8080;
    // create endpoint and bind it to the socket
    while (true) {
        try {
            socket.bind(getPortName(port));
            break;
        }
        catch (...) {
            port++;
        }
    }
    return port;
}

int connectSocket(zmq::socket_t &socket)
{
    int port = 8080;
    // create endpoint and bind it to the socket
    while (true) {
        try {
            socket.connect(getPortName(port));
            break;
        }
        catch (...) {
            port++;
        }
    }
    return port;
}

/* void createNode(int id, int port)
{
    // new node process
    execl("./node.cpp", "child", std::to_string(id).c_str(), std::to_string(port).c_str(), NULL);
} */
