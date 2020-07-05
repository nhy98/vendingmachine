#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct Inventory_t {
	char machine_name[100];
	char item_name[100];
	int num_items;
} Inventory;

int generateNumToSeek(int num) {
	int count = 0;
	while(num != 0) {
        num /= 10;
        ++count;
    }
    return count+1;
}

int main(int argc, char const *argv[])
{
	/* code */

	int c, pid, message_size = 0, number = 0;
	int socket_server, socket_client;
	struct sockaddr_in server,client;
	char *item_name, *machine_name, *num_items;
	char message_client[200];
	char success[] = "Got your item successully",fail[] ="Got your item failed";


	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	bind(socket_server, (struct sockaddr *) &server, sizeof(server));

	listen(socket_server,3);
							clock_t last = clock();

	while(1) {
		printf("last: %u\n",last);
		clock_t current = clock();
				printf("current: %u\n",current);
					if (current >= (last + 10 * CLOCKS_PER_SEC)) {
						printf("10 secondssss\n");
						last = current;
					}

		int socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c );
		pid = fork();

		if(pid<0) {
			close(socket_client);
			continue;
		} else if(pid>0){
            close(socket_client);
            // counter++;
            printf("A client connected\n");
            continue;
        } 
		else {
			if(socket_client < 0) {
				perror("Accept failed");
			}
			while(1) {

				
				int confirmed = 0;
				FILE *f;
				int num_seek,num_seek_last;
				char str[1000], machine[1000], item[1000],num_items[1000]="0";

				message_size = recv(socket_client, message_client, 32, 0);
				machine_name = strtok(message_client,";");
				item_name = strtok(NULL,"\n");

				f = fopen("inventory.txt","r+");
				while(!feof(f)) {
					fgets(str, 1000, f);
					if(feof(f)) {
						break;
					}
					char *sysmachine = strtok(str,";");
					char *sysitem = strtok(NULL,";");
					char *sysnum = strtok(NULL,"");
					int num = atoi(sysnum);
					
		
					if(strcmp(machine_name,sysmachine) == 0 && strcmp(item_name,sysitem)==0) {
						char currnum[1000];
						--num;
						write(socket_client, success, strlen(success)+1);

						//chuyen item den machine
						if(num<3) {
							char client_message[1000];
							printf("Delivery....\n");
                            sprintf(client_message,"10 %s are added to machine %s\n",sysitem,sysmachine);
                            send(socket_client,client_message,strlen(client_message),0);
                            sleep(5);

                            num+=10;
							strcpy(num_items,"");

                            printf("Done!......\n");
						}
						if(num>=10) {
							strcpy(num_items,"");
						}
						sprintf(currnum,"%d",num);
						strcat(num_items,currnum);
						fseek(f,-3,SEEK_CUR);
						fputs(num_items,f);


						confirmed = 1;
						
						//Ghi vao file history
						FILE *history;
						time_t t = time(NULL);
    					struct tm tm;
						history = fopen("history.txt","a");
						tm = *localtime(&t);
						printf("Sold %s in machine %s at %d-%d-%d %d:%d:%d, remain %s\n", sysitem, sysmachine, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num_items );
                        fprintf(history,"%s\t%s\t%d-%d-%d %d:%d:%d\n",sysmachine,sysitem,  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
						fclose(history);
					
						break;
					}
					// printf("****\n");
				}
				fclose(f);
				if(confirmed == 0) {
					write(socket_client,fail,strlen(fail)+1);
					break;
				}
			}
			close(socket_client);
		}
	}

	return 0;
}