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

void NodeWork(std::string &str, zmq::socket_t &lSocket, zmq::socket_t &rSocket, zmq::context_t &context)
{
	std::string cmd, cfc = "0";
	
	int id, pid;
	getinfo(str, cmd, id, pid);
	if (cmd == "ok")
	{
		return;
	}
	else if (cmd == "create")
	{
		if (pid == myId)
		{
			if (havechild == 0) //если нет ребёнка
			{
				havechild = 1;
			}
			else{ //если есть ребёнок
				
				sendM(rSocket, swich);
				str = receiveMessage(rSocket);
				cfc = "1";
			}

			int childid = 13;
			childid = fork();
			if (childid == -1)
			{ //error
				perror("Bad fork1\n");
				exit(0);
			}
			if (childid == 0) //for child
			{
				//printf("Node %d: PREEXEC %d\n", myId, getpid());
				std::string cid = std::to_string(id);
				printf("Node %d : execute: port = %d\n", myId, port);
				execl("child", "child", cid.c_str(), std::to_string(port).c_str(), cfc.c_str(), NULL);
				
			}
			cfc = "0";
			sendM(rSocket, str);
			str = receiveMessage(rSocket);
			if(str != "ok"){
				std::cout<<"Node "<<myId<<": Child have problems"<<std::endl;
			}
			//str = "create " + std::to_string(id) + " " + std::to_string(pid);
			std::cout << getpid() << " is my id after execute"
					  << "<<<<<<<" << std::endl;
		}
		else if (id != myId)
		{
			sendM(rSocket, str);
			printf("Node %d: message sent to next\n", myId);
			str = receiveMessage(rSocket);
			printf("Node %d: reply from child: _%s_\n", myId, str.c_str());
		}
		else
		{
			printf("I've just bornt\n");
		}

		return;
	}
	else if (cmd == "end")
	{
		//std::cout << havechild << " havech" << std::endl;
		if (havechild == 1)
		{
			sendM(rSocket, "end");
			printf("Node %d: sent msg to next and finishing\n", myId);
		}
		else
		{
			printf("Node %d: without children finishing\n", myId);
		}
		exit(0);
	}
	else if(cmd == "swich"){
		if(havechild == 1){
			sendM(rSocket, swich);
			str = receiveMessage(rSocket);
		}
		lSocket.close();
		rSocket.close();
		//zmq::socket_t lSocket1(context, ZMQ_REP);
		//zmq::socket_t rSocket1(context, ZMQ_REQ);

		port = connectSocket(lSocket, port);
		//std::cout<<"Node "<<myId<< ": reconnected to port "<<port<<std::endl;
		port++;
		port = bindSocket(rSocket, port);
		//std::cout<<"Node "<<myId<< ": rebinded to port "<<port<<std::endl;
		return;
	}else if(cmd == "remove"){
		if(myId == id){
		//std::cout << havechild << " havech" << std::endl;
		if (havechild == 1)
		{
			sendM(rSocket, "end");
			printf("Node %d: sent msg to next and finishing\n", myId);
		}
		else
		{
			printf("Node %d: without children finishing\n", myId);
		}
		str = "bloodmail";
		sendM(lSocket, str);
		exit(0);
		}else 
		{
			sendM(rSocket, str);
			str = receiveMessage(rSocket);
			if(str == "bloodmail"){
				printf("Node %d: recieved \"bloodmail\"\n", myId);
				havechild = 0;
			}
			return;
		}
	}
	return;
}

int main(int argc, char const *argv[])
{
	myId = atoi(argv[1]);
	port = atoi(argv[2]);
	havechild = atoi(argv[3]);
	printf("Node %d(%d): Start working: %s %s %s\n", myId, getpid(), argv[1], argv[2], argv[3]);

	zmq::context_t context(1);

	zmq::socket_t lSocket(context, ZMQ_REP);

	zmq::socket_t rSocket(context, ZMQ_REQ);
	printf("Node %d:  Sockets and context done\n", myId);

	try
	{
		port = connectSocket(lSocket, port);
		std::cout << "Node " << myId << ": Node connected to " << port << std::endl;
		port++;
	}
	catch (...)
	{
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
		printf("Node %d:havechild1 = %d\n", myId, havechild);
		NodeWork(message, lSocket, rSocket, context);
		printf("Node %d:havechild2 = %d\n", myId, havechild);
		std::string reply("ok");
		sendM(lSocket, reply);
	}
	return 0;
}
