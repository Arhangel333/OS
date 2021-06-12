#include <time.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include "commands.hpp"
#include <zmq.hpp>
#include <unistd.h>

typedef struct MD
{
	int clasterId;
	int messageNumber;
	char message[128];

MD operator=(MD m){
	this->clasterId = m.clasterId;
	this->messageNumber = m.messageNumber;
	for (int i = 0; i < 128; i++)
		{
			this->message[i] = m.message[i];
		}
}

MD operator=(std::string s){
		MD m;
		for (int i = 0; i < 128; i++)
		{
			m.message[i] = s[i];
		}
		return m;
	}
} MessageData;

int main(int argc, char const *argv[])
{

	std::vector<int> vectr(7);
	zmq::context_t context(1);
	srand(time(0));
	int clasterId = getpid();
	printf("Clasterid %d Starting...\n", clasterId);
	std::string msg("Test message");
	std::string cmd;

	while(1){//для ввода и отправки сообщений цикл


	while(1==true){
        getline(std::cin, cmd);
		//std::cout<<cmd<<std::endl;
        if(cmd == "create"){//топология - список, добавление: [create id pid]
			int cid = fork();
			if (cid == -1)
			{ //error
				perror("Bad fork1\n");
				exit(0);
			}
			if(cid > 0)//for parent
			{
				printf("PREEXEC\n");
				execl("child", "child", NULL);
			}
			msg = "create";
			break;
        }
        else if(cmd == "remove"){// [remove id]

        }
        else if(cmd == "exec"){//локальный целочисленный словарь [exec id name value]     //map

        }
        else if(cmd == "heartbit"){//проверка работоспособности узлов [heartbit 2000]
			execl("ps", "ps", NULL);
        }
        else if(cmd == "ping"){//проверка работоспособности узла [ping id]

        }
		else if(cmd == "end"){//завершение программы
			if(kill(-1, -9) == -1){
				perror("kill foalt\n");
				//break;
			}
			return 0;
        }
    }

	

	zmq::socket_t senderSocket(context, ZMQ_REQ);
	bindSocket(senderSocket);
	printf("Connection\n");
	//getPortName(zmq_connect(senderSocket, "tcp://127.0.0.1:8080");
	int count = 0;

	//for (;;){
		/* MessageData md;
		md.clasterId = clasterId;
		md.messageNumber = count;
		memcpy(md.message, &msg, msg.size());
		zmq_msg_t zmqMessage;
		zmq_msg_init_size(&zmqMessage, sizeof(MessageData));
		memcpy(zmq_msg_data(&zmqMessage), &md, sizeof(MessageData)); */

		printf("Sending: - %d\n", count);
		try{
			sendM(senderSocket, msg);
		}catch(...){
			printf("Claster:error: %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
		
		//int send = zmq_msg_send(&zmqMessage, senderSocket, 0);
		printf("sent\n");
		//zmq_msg_close(&zmqMessage);

		std::string reply;
		//zmq_msg_init(&reply);
		printf("Claster: reciving reply\n");
		reply = receiveMessage(senderSocket);
		//zmq_msg_recv(&reply, senderSocket, 0);
		//size_t repSize = zmq_msg_size(&reply);
		printf("Received:_%s_\n", reply.c_str());
		//zmq_msg_close(&reply);
		return 0;
		//sleep(1000);
		count++;

	}
}


