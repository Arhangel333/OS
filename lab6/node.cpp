#include "commands.hpp"


typedef struct MD
{
	int clientId;
	int messageNumber;
	char message[128];
} MessageData;

void NodeWork(std::string &str){

}

int main(int argc, char const *argv[])
{
	printf("Execute works\n");
	
    zmq::context_t context(1);
	printf("context done\n");
    zmq::socket_t nodeSocket(context, ZMQ_REP);
	printf("Socket done\n");
	
	try{
		std::cout<<"Node connected to "<<connectSocket(nodeSocket)<<std::endl;
			//nodeSocket.connect(getPortName(8080));
		}catch(...){
			printf(" %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
	
    printf("Node id:%d is connected\n", getpid());
	
	
	
	
	for (;;)
	{
		//MessageData message;
		printf("Node: reciving\n");
		std::string message(receiveMessage(nodeSocket));
		printf("Message from claster:_%s_\n", message.c_str());
		std::string reply("ok");
        sendM(nodeSocket, reply);
		break;
	}
    printf("Node id:%d ended his work\n", getpid());
	return 0;
}
