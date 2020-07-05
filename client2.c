#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

char** getInventory(){
    FILE *f;
    char str[1000];
    char** listMachine = malloc(10 * sizeof(char*));

    // char listMachine[3][20];
    f = fopen("inventory.txt","r+");
    int i = 3,j=0;
    while(!feof(f)) {
		fgets(str, 1000, f);
        if(feof(f)) {
		    break;
		}
		char *sysmachine = strtok(str,";");
        if(i==3){
            i = 0;
            listMachine[j] = malloc(100 * sizeof(char));
            strcpy(listMachine[j],sysmachine);
            ++j;
        }
        if(i==0){
            printf("%s: ",sysmachine);
            printf("\n\n");
        }
		char *sysitem = strtok(NULL,";");
        char *sysnum = strtok(NULL,"");
        printf("Item: %s", sysitem );             printf("\tQuantity: %s\n",sysnum);
        i++;
    }
    fclose(f);
    return listMachine;
}

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
        char** listMachine = malloc(10 * sizeof(char*));

        listMachine = getInventory();
		///nhap hang va ten may
		int q,z=0; 
		printf("Select machine:\n> ");
		scanf("%s",machine_name);
        while(z<3){
            if(strcmp(machine_name,listMachine[z])==0) break;
            z++;
        }
        if(z==3){
            printf("Error! You must choose one of our three machine\n> ");
            continue;
        }
		printf("Please select item:\n> ");
		scanf("%s",item_name);

		//tao message
		strcpy(message, machine_name);
		strcat(message,";");
		strcat(message, item_name);
		strcat(message,"\n");
        		printf("Message: %s\n",message);
		send(sock, message, strlen(message)+1,0);
	
		recv(sock, server_reply, 2000, 0);
		printf("%s\n", server_reply);
		printf("Press any key to continue or Enter 1 to exit\n");
		scanf("%d",&q);
		getchar();
		if(q==1) break;
	}
	close(sock);
	return 0;
}
