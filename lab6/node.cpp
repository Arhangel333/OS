#include "commands.hpp"


typedef struct MD
{
	int clientId;
	int messageNumber;
	char message[128];
} MessageData;

int myId = 0;
int lport = 0;

void NodeWork(std::string &str, zmq::socket_t &rSocket){
	std::string cmd;
	int id, pid;
	getinfo(str, cmd, id, pid);
	if(cmd == "ok"){
		return;
	}else if(cmd == "create"){
		if(pid == myId){
			
			int childid = fork();
			if (childid == -1)
			{ //error
				perror("Bad fork1\n");
				exit(0);
			}
			if(childid == 0)//for child
			{
				printf("PREEXEC\n");
				std::string cid = std::to_string(id);
				execl("child", "child", cid.c_str(), NULL);
			}
			str = "chcreate " + std::to_string(id) + " " + std::to_string(pid);
			std::cout<<str<<"<<<<<<<"<<std::endl;
		}else{
			//sendM(rSocket, str);
			printf("Node %d: message sent to next\n", getpid());
			return;
		}
	} else if(cmd == "chcreate"){
		myId == id? printf("Node %d: true child id\n", getpid()) : printf("Node %d: false child id\n", getpid());
	}else if(cmd == "end"){
		sendM(rSocket, "end");
		printf("Node %d: sent msg to next and finishing\n", getpid());
		exit(0);
	}
}

int main(int argc, char const *argv[])
{

	printf("Node %d: Execute works %s\n", getpid(), argv[1]);
	myId = atoi(argv[1]);
	lport = atoi(argv[2]);
    zmq::context_t context(1);
	printf("Node %d:  context done\n", getpid());
    zmq::socket_t lSocket(context, ZMQ_REP);
	printf("Node %d:  Socket done\n", getpid());

	zmq::socket_t rSocket(context, ZMQ_REQ);
	
	//int rport = 0;
	lport = bindSocket(rSocket, lport);
	try{
		lport = connectSocket(lSocket);
		std::cout<<"Node "<<getpid()<<": Node connected to "<<lport<<std::endl;
			//lSocket.connect(getlPortName(8080));
		}catch(...){
			printf(" %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
	
    printf("Node id:%d is connected\n", getpid());
	

	std::string message("ok");
	
	
	for (;;)
	{
		//MessageData message;
		printf("Node %d: reciving\n", getpid());
		message = receiveMessage(lSocket);
		NodeWork(message, rSocket);
		printf("Node %d: Message from claster:_%s_\n", getpid(), message.c_str());
		std::string reply("ok");
        sendM(lSocket, reply);
	}
    printf("Node id:%d ended his work\n", getpid());
	return 0;
}
