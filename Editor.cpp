#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080
#define BACKLOG 10
int reader_fd = 0;
int live_id=0;
#define READER_PORT 8081

void *serve(void *arg){
	int newsockfd = *((int *)(arg));
	while(1){
		char buf[1024] = {0};
		string buffer;
		//  2|live|hello
	
		recv(newsockfd,buf,1024,0);
		buffer = string(buf);
		int readerno = 0,i=0;
		while(i<buffer.length()&&buffer[i]!='|') i++;
		readerno = atoi(buffer.substr(0,i).c_str());i++;
		string type;int j=i;
		cout<<"buffer.length():"<<buffer.length()<<"and buffer is:"<<buffer<<endl;
		while(j<buffer.length()&&buffer[j]!='|')j++;
		cout<<"i and j are:"<<i<<j<<endl;
		type = buffer.substr(i,j-i);j++;
		cout<<"TYPE IS:"<<type<<";\n";
		if(type == "docu"){
			ofstream fout;
			fout.open("output.txt",ios::app);
		 	string msg = buffer.substr(0,i-1) + " sent message: " + buffer.substr(j,buffer.length()-j);
			msg+="\n";
			fout<<msg;
			cout<<"MESSAGE SENT"<<endl;
			fout.close();
		}
		else if(type == "live") {struct sockaddr_in seraddr1,cliaddr1;
				int sockfd1,newsockfd1;
				char message[1024]={0};int option=1;
				int addrlen1=sizeof(seraddr1);
			//string msg = buffer.substr(j,buffer.length()-j);
			//recv(live_id,LIVE_PORT,1024,0);
			int LIVE_PORT=8083;
			if((sockfd1 = socket(AF_INET, SOCK_STREAM,0))<0){
				perror("socket failed");
				exit(EXIT_FAILURE);
			}

			if(setsockopt(sockfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))){
				perror("setsockopt failed");
				exit(EXIT_FAILURE);	
			}
			seraddr1.sin_family = AF_INET;
			seraddr1.sin_addr.s_addr = INADDR_ANY;
			seraddr1.sin_port = htons(LIVE_PORT);

			if(bind(sockfd1,(struct sockaddr *)&seraddr1,sizeof(seraddr1)) < 0){
				perror("bind failed");
				exit(EXIT_FAILURE);
			}

			if (listen(sockfd1, 10) < 0)
			{
			    perror("listen failed");
			    exit(EXIT_FAILURE);
			}
			if ((newsockfd1 = accept(sockfd1, (struct sockaddr *)&cliaddr1,(socklen_t*)&addrlen1))<0){
											perror("accept failed");exit(EXIT_FAILURE);}
			while(1){
					recv(newsockfd1,message,1024,0);
					if(message!=NULL){
					cout<<"RECEIVED LIVE DATA:"<<message<<endl;				
					send(reader_fd,message,strlen(message)+1,0);
			}}
		}
		else if(type == "news"){
			string msg = buffer.substr(j,buffer.length()-j);
			cout<<"sending:"<<msg<<endl;
			send(reader_fd,msg.c_str(),msg.length()+1,0);
		}
		else{
			cout<<"Not recognised msg"<<endl;
		}
	}



}

int main(){

	int sockfd,newsockfd;
	struct sockaddr_in seraddr,cliaddr;//cliaddr2;
	int option=1;
	int addrlen=sizeof(seraddr);
	char buffer[1024] = {0};
	
	int ret;


	if((sockfd = socket(AF_INET, SOCK_STREAM,0))<0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}


	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))){
		perror("setsockopt failed");
		exit(EXIT_FAILURE);	
	}


	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = INADDR_ANY;
	seraddr.sin_port = htons(PORT);

	if(bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, BACKLOG) < 0)
	{
	    perror("listen failed");
	    exit(EXIT_FAILURE);
	}


	if((reader_fd = socket(AF_INET, SOCK_STREAM,0))<0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(READER_PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr)<=0) 
    {
        perror("Invalid address/ Address not supported ");
        exit(EXIT_FAILURE);	
    }
  
    if (connect(reader_fd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0)
    {
        perror("connect Failed");
        exit(EXIT_FAILURE);	
    }


	

    while(1){
    	if ((newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr,(socklen_t*)&addrlen))<0){
											perror("accept failed");exit(EXIT_FAILURE);
										}
	pthread_t t1;
	pthread_create(&t1,NULL,serve,(void*)(&newsockfd));
    }

	return 0;
}
