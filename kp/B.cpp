#include <iostream>
#include "commands.hpp"

int main(int argc, char* argv[]){

    printf("B started\n");
    int port = 8080;

    if(argc < 1){
        printf("Error argc\n");
        exit(0);
    }else{
        printf("B else %d\n", port);
        port = atoi(argv[0]);
        printf("B: port == %s\n", argv[0]);
        printf("%d - port\n", port);
    }

    zmq::context_t context(1);
	zmq::socket_t ASocket(context, ZMQ_REP);
    zmq::socket_t CSocket(context, ZMQ_REP);


	port = connectSocket(ASocket, port);
    printf("B connected to %d\n", port);
    port = connectSocket(CSocket, port + 1);
    printf("B connected to %d\n", port);

    std::string rep, ok = "ok";



    for(;;){
        rep = receiveMessage(ASocket);
        printf("B: recieved from A message size:\"%d\"\n", rep.size());
        sendM(ASocket, ok);

        rep = receiveMessage(CSocket);
        printf("B: recieved from C message size:\"%d\"\n", rep.size());
        sendM(CSocket, ok);
        
        if(rep == "end"){
        std::cout<<"B: ending..."<<std::endl;
        exit(0);
        }
    }

return 0;
}