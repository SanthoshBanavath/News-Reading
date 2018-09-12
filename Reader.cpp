#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <arpa/inet.h>
using namespace std;

#define READER_PORT 8081

int main(){

	int sockfd;
	struct sockaddr_in seraddr,cliaddr;
	int addrlen=sizeof(seraddr);
	int option=1;
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
	seraddr.sin_port = htons(READER_PORT);

	if(bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 10) < 0)
	{
	    perror("listen failed");
	    exit(EXIT_FAILURE);
	}
int newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr,(socklen_t*)&addrlen);
 int pid=fork(); 
 while(1)
 {
	recv(newsockfd,buffer,1024,0);
		  if(pid==0)
		  {
		    cout<<"Msg received from editor to reader1:"<<buffer<<endl;
		  }
		  else 
		  {
		    cout<<"Msg received from editor to reader2:"<<buffer<<endl;
		  }
 }

	return 0;
}
