#include <iostream>
#include "commands.hpp"

int main(int argc, char* argv[]){

    printf("C: started\n");
    int port = 8080;

    if(argc < 1){
        printf("Error argc\n");
        exit(0);
    }else{
        //printf("C: else %d\n", port);
        port = atoi(argv[0]);
        printf("C: port == %s\n", argv[0]);
        //printf("%d - port\n", port);
    }

    zmq::context_t context(1);
	zmq::socket_t ASocket(context, ZMQ_REP);
    zmq::socket_t BSocket(context, ZMQ_REQ);


	port = connectSocket(ASocket, port);
    printf("C connected to %d\n", port);

    port = bindSocket(BSocket, port + 2);
    printf("C binded to %d\n", port);

    std::string rep, repB;



    for(;;){
        rep = receiveMessage(ASocket);
        printf("C recieved from A \"%s\"\n", rep.c_str());
        
        sendM(ASocket, "ok");

        sendM(BSocket, rep);
        std::cout<<"C sent to B"<<std::endl;
        repB = receiveMessage(BSocket);
        printf("C recieved from B \"%s\"\n", repB.c_str());
        
        if(rep == "end"){
        std::cout<<"C: ending..."<<std::endl;
        exit(0);
        }
    
    }

return 0;
}