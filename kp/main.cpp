#include <iostream>
#include "commands.hpp"

using namespace std;



int main(){
zmq::context_t context(1);
zmq::socket_t SocketToC(context, ZMQ_REQ);
zmq::socket_t SocketToB(context, ZMQ_REQ);

int port = 8080;
int port1 = port + 1;

int status;


port = bindSocket(SocketToC);
port1 = bindSocket(SocketToB, port1);


int id = fork();
if(id == 0){//child
    int id1 = fork();
    if (id1 == 0)//child2
    {
        execl("child", std::to_string(port1).c_str(), NULL);
    }
    
    execl("childc", std::to_string(port).c_str(), NULL);
}
printf("A binded to %d\n", port);
printf("A binded to %d\n", port1);

std::string word, word1, word2;
while(1 == true){
    getline(cin, word);
    //cin>>word;
    cout<<"end of entering"<<endl;
    sendM(SocketToC, word);
    
    cout<<"A: Message sent"<<endl;
    word1 = receiveMessage(SocketToC);
    cout<<"A: msg from C recieved"<<endl;
    
    sendM(SocketToB, word);
    
    //sleep(1);
    word2 = receiveMessage(SocketToB);
    cout<<"A: From B: "<<word2<<endl;
    if(word == "end"){
        cout<<"A: ending..."<<endl;
        exit(0);
    }

}







    return 0;
}
