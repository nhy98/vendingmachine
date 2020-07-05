#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char const *argv[]) {
	char item_name[100], machine_name[100];
	char message[100];
	int sock, connectMain;
	struct sockaddr_in server;
	char server_reply[1000];

	//tao socket ket noi den server
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		perror("Create socket error");
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	connectMain=connect(sock , (struct sockaddr *)&server , sizeof(server));

	while(1) {
		///nhap hang va ten may
		int q; 
		printf("Select machine:\n> ");
		scanf("%s",machine_name);
		printf("Please select item:\n> ");
		scanf("%s",item_name);

		//tao message
		strcpy(message, machine_name);
		strcat(message,";");
		strcat(message, item_name);
		strcat(message,"\n");

		send(sock, message, strlen(message)+1,0);
	
		recv(sock, server_reply, 2000, 0);
		printf("%s\n", server_reply);
		printf("Enter 1 to exit\n");
		scanf("%d",&q);
		getchar();
		if(q==1) break;
	}
	close(sock);
	return 0;
}
