#include "commands.hpp"

/* typedef struct MD
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
} MessageData; */

std::vector<int> vector(7);
int havechild = 0;

void stringWarker(std::string &msg, zmq::socket_t &senderSocket){
	std::string cmd, line;
	printf("Claster: Введите: command  <pid> <id>\n");
	getline(std::cin, line);
	std::cout<<line<<std::endl;
	int id, pid;
	getinfo(line, cmd, id, pid);//cmd = word(line);
	std::cout<<cmd<<"_"<<std::endl;
	if(cmd == "create"){		
		//id = atoi(word(line).c_str());
		//pid = atoi(word(line).c_str());
		
		printf("printf: %d %d\n", id, pid);
		if(inVector(id, vector)){
			printf("Claster: %d-node is existing already\n", id);
			msg = "abort";
			return;		
		}else if(!inVector(pid, vector)){
			printf("Parent node is not existing\n");
			msg = "abort";
			return;	
		}else{
			printf("Claster: Node can be created\n");
			vector.push_back(id);
		}
			if(havechild != 1){
				havechild = 1;
			int childid = fork();
			if (childid == -1)
			{ //error
				perror("Bad fork1\n");
				exit(0);
			}
			if(childid == 0)//for child
			{
				printf("Claster(%d): PREEXEC\n", getpid());
				std::string cid = std::to_string(id);
				execl("child", "child", cid.c_str(), "8080", NULL);
			}
			}//havechild == 0
			msg = "create " + std::to_string(id) + " " + std::to_string(pid);
			std::cout<<msg<<"<<<<<<<"<<std::endl;
		
	}else if((cmd == "finish")||(cmd == "end")||(cmd == "exit")){
		msg = "end";
		if(havechild > 0)
		try{
			sendM(senderSocket, msg);
		}catch(...){
			printf("Claster:error: %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
		printf("Claster: Working is finishing...\n");
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
    vector.push_back(-1);
	int count = 0;
	std::vector<int> vectr(7);
	zmq::context_t context(1);
	zmq::socket_t senderSocket(context, ZMQ_REQ);
	printf("Claster(%d): Connection\n", getpid());
	bindSocket(senderSocket);
	srand(time(0));

	std::string msg("Test message");
	

	while(1){//для ввода и отправки сообщений цикл

	stringWarker(msg, senderSocket);//читает комманду и создаёт сообщение соотв.
	if(msg != "abort"){//значит что не произошла ошибка в stringWarker()
	
	
	

		printf("Claster: Sending: - %d\n", count);
		try{
			sendM(senderSocket, msg);
		}catch(...){
			printf("Claster:error: %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
			exit(0);
			}
		
		printf("Claster: sent\n");

		std::string reply;
		printf("Claster: reciving reply\n");
		reply = receiveMessage(senderSocket);
		printf("Claster: Received:_%s_\n", reply.c_str());
		count++;
	}//abortend

	}//whileend
}//mainend


