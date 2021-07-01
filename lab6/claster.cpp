#include "commands.hpp"

#define SAFETY

std::vector<int> vector(7);
int havechild = 0;

auto start_heartbeat = std::chrono::high_resolution_clock::now();
auto stop_heartbeat = std::chrono::high_resolution_clock::now();
auto time_heartbeat = 0;

void stringWarker(std::string &msg, zmq::socket_t &senderSocket)
{

	std::string cmd, line, k, cfc = "0";
	printf("Claster: Введите: <command> <pid> <id>\n");
	getline(std::cin, line);
	std::cout << line << std::endl;
	int id, pid;
	k = line;
	cmd = word(k);
	/* printf("Vector: ");
	for (int i = 0; i < vector.size(); i++)
	{
		printf("%d ", vector[i]);
	}
	printf("\n"); */
	if (cmd == "remove")
	{
		id = atoi(word(k).c_str());
		if (inVector(id, vector))
		{
			//std::cout << *(vector.begin() + find(id, vector)) << " is erasing\n";
			vector.erase(vector.begin() + find(id, vector), vector.end());
		}
		else
		{
			printf("Claster: Element can`t be removed, becouse it doesn`t exist\n");
			msg = "abort";
			return;
		}

		std::cout << id << " id\n";
		if (id == -1)
		{
			goto end;
		}
		msg = "remove " + std::to_string(id) + " -1"; //-1 для корректной работы getinfo()
		return;
	}
	else if (cmd == "execute")
	{
		id = atoi(word(k).c_str());
		if (inVector(id, vector))
		{
			msg = line;
		}
		else
		{
			printf("Claster: Node doesn`t exist\n");
			msg = "abort";
		}
		return;
	}
	else if (cmd == "heartbeat")
	{
		int time = atoi(word(k).c_str());
		if (time < 0)
		{
			std::cout << "Claster: error: Time is above zero" << std::endl;
			msg = "abort";
			return;
		}
		if (havechild == 0)
		{
			std::cout << "Claster: error: no children" << std::endl;
			msg = "abort";
			return;
		}
		msg = "heartbeat";
		printf("Claster: Start heartbeat\n");
			start_heartbeat = std::chrono::high_resolution_clock::now();
			do
			{
				stop_heartbeat = std::chrono::high_resolution_clock::now();
				time_heartbeat = std::chrono::duration_cast<std::chrono::milliseconds>(stop_heartbeat - start_heartbeat).count();
			} while (time_heartbeat < time);

			time_heartbeat = 0;

		return;
	}
	getinfo(line, cmd, id, pid); //cmd = word(line);
	//std::cout << cmd << "_" << std::endl;
	if (cmd == "create")
	{
		//id = atoi(word(line).c_str());
		//pid = atoi(word(line).c_str());

		//printf("printf: %d %d\n", id, pid);
		if (inVector(id, vector))
		{
			printf("Claster: %d-node is existing already\n", id);
			msg = "abort";
			return;
		}
		else if (!inVector(pid, vector))
		{
			printf("Claster: Parent node is not existing\n");
			msg = "abort";
			return;
		}
		else
		{
#ifdef SAFETY
			if ((pid != vector[vector.size() - 1]))
			{
				msg = "abort";
				printf("Claster: safty modul is blocking addition not into the end\n");
				return;
			}
#endif
			printf("Claster: Node can be created\n");

			vector.push_back(id);
		}
		if (havechild != 1)
		{
			havechild = 1;
			int childid = fork();
			if (childid == -1)
			{ //error
				perror("Claster: error: Bad fork1\n");
				exit(0);
			}
			if (childid == 0) //for child
			{
				//printf("Claster(%d): PREEXEC\n", getpid());
				std::string cid = std::to_string(id);
				execl("child", "child", cid.c_str(), "8080", cfc.c_str(), NULL);
			}
		}
		else if ((havechild == 1) && (pid == -1))
		{
			sendM(senderSocket, swich);
			line = receiveMessage(senderSocket);
			cfc = "1";

			int childid = fork();
			if (childid == -1)
			{ //error
				perror("Claster: error: Bad fork1\n");
				exit(0);
			}
			if (childid == 0) //for child
			{
				//printf("Claster(%d): PREEXEC\n", getpid());
				std::string cid = std::to_string(id);
				execl("child", "child", cid.c_str(), "8080", cfc.c_str(), NULL);
			}
		} //(havechild == 1)&&(pid == -1)

		msg = "create " + std::to_string(id) + " " + std::to_string(pid);
		return;
	}
	else if ((cmd == "finish") || (cmd == "end") || (cmd == "exit"))
	{
	end:
		msg = "end";
		if (havechild > 0)
			try
			{
				sendM(senderSocket, msg);
			}
			catch (...)
			{
				printf("Claster:error: %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
				exit(0);
			}
		printf("Claster: Working is finishing...\n");
		exit(0);
	}
	else
	{
		msg = "abort";
		return;
	}
}

int main(int argc, char const *argv[])
{
	vector.push_back(-1);
	int count = 1;
	std::vector<int> vectr(7);
	zmq::context_t context(1);
	zmq::socket_t senderSocket(context, ZMQ_REQ);
	printf("Claster(%d): Connection\n", getpid());
	bindSocket(senderSocket);
	srand(time(0));

	std::string msg("Test message");

	while (1)
	{ //для ввода и отправки сообщений цикл

		stringWarker(msg, senderSocket); //читает комманду и создаёт сообщение соотв.
		//std::cout << msg << "<<<<<<<msg from claster" << std::endl;

		if (msg != "abort")
		{ //значит что не произошла ошибка в stringWarker()

			printf("Claster: Sending: - %d\n", count);
			try
			{
				sendM(senderSocket, msg);
			}
			catch (...)
			{
				printf("Claster:error: %d: %s\n", zmq_errno(), zmq_strerror(zmq_errno()));
				exit(0);
			}

			//printf("Claster: sent\n");

			std::string reply;
			//printf("Claster: reciving reply\n");
			reply = receiveMessage(senderSocket);
			printf("Claster: Received:_%s_\n", reply.c_str());
			count++;
		} //abortend

	} //whileend
} //mainend
