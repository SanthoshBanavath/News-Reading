#include <bits/stdc++.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 8080

int main(){

	int sockfd;
	struct sockaddr_in seraddr;
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
    seraddr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr)<=0) 
    {
        perror("Invalid address/ Address not supported ");
        exit(EXIT_FAILURE);	
    }
  
    if (connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0)
    {
        perror("connect Failed");
        exit(EXIT_FAILURE);	
    }

   
    while(1){
    	string msg;
    	cout<<"Enter Message:";
    	getline(cin,msg);
	    send(sockfd,msg.c_str(),msg.length()+1,0);
	    //recv(sockfd,buffer,1024,0);
	    //cout<<"client received:"<<buffer<<endl;
		cout<<"Message sent:"<<msg<<endl;
    }



	return 0;
}
