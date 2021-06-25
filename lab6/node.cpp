#include "commands.hpp"


typedef struct MD
{
	int clientId;
	int messageNumber;
	char message[128];
} MessageData;

int myId = 0;
int port = 0;
int havechild = 0;

void NodeWork(std::string &str, zmq::socket_t &rSocket){
	std::string cmd;
	int id, pid;
	getinfo(str, cmd, id, pid);
	if(cmd == "ok"){
		return;
	}else if(cmd == "create"){
		if(pid == myId){
			if(havechild == 0){
			havechild = 1;
		}
			int childid = 13;
			childid = fork();
			if (childid == -1)
			{ //error
				perror("Bad fork1\n");
				exit(0);
			}
			if(childid == 0)//for child
			{
				printf("Node %d: PREEXEC %d\n", myId, getpid());
				std::string cid = std::to_string(id);
				execl("child", "child", cid.c_str(), std::to_string(port).c_str(), NULL);
			}

			str = "create " + std::to_string(id) + " " + std::to_string(pid);
			std::cout<<getpid()<<" is my id after execute"<<"<<<<<<<"<<std::endl;
		}else if(id != myId){
			sendM(rSocket, str);
			printf("Node %d: message sent to next\n", myId);
			str = receiveMessage(rSocket);
			printf("Node %d: reply from child: _%s_\n", myId, str.c_str());
		}else{
			printf("I've just bornt\n");
		}
		
		return;
	}else if(cmd == "end"){
		std::cout<<havechild<<" havech"<<std::endl;
		 if(havechild == 1){
			sendM(rSocket, "end");
			printf("Node %d: sent msg to next and finishing\n", myId);
		 }else{
			printf("Node %d: without children finishing\n", myId);
		 }
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
	myId = atoi(argv[1]);
	port = atoi(argv[2]);
	printf("Node %d(%d): Execute works %s\n", myId, getpid(), argv[1]);
	
    zmq::context_t context(1);
	printf("Node %d:  context done\n", myId);
    zmq::socket_t lSocket(context, ZMQ_REP);
	

	zmq::socket_t rSocket(context, ZMQ_REQ);
	printf("Node %d:  Sockets done\n", myId);

	
	try{
		port = connectSocket(lSocket, port);
		std::cout<<"Node "<<myId<<": Node connected to "<<port<<std::endl;
			port++;
		}catch(...){
			printf(" %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
	port = bindSocket(rSocket, port);
	
    printf("Node id:%d is connected\n", myId);
	

	std::string message("ok");
	
	
	for (;;)
	{
		//MessageData message;
		printf("Node %d: reciving\n", myId);
		message = receiveMessage(lSocket);
		printf("Node %d: Message from left:_%s_\n", myId, message.c_str());
		NodeWork(message, rSocket);
		
		std::string reply("ok");
        sendM(lSocket, reply);
	}
    printf("Node id:%d ended his work\n", myId);
	return 0;
}
