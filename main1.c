#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>


int indx;
char *IP[2]; 
int sock;
struct sockaddr_in local;
struct sockaddr_in remote;
int  length;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t count_threshold_cv;
#define clear() printf("\033[H\033[J")

void keyboard(LIST *out_list){
	char *buffer;
	while(1){

		int number;
		buffer = (char *)malloc(256*sizeof(char));
    	
    	printf(
        	"Enter your meassage in the box below:\n"
    	);
		bzero(buffer,256);
		fgets(buffer,255,stdin);
	
		if (strcmp("!\n",buffer)==0){
			 exit(0);
			}
		else{
			 int result = ListPrepend(out_list,buffer);
			 }	
		 }
}



void screen(LIST *rec_list){
	char *item;
	int i = ListCount(rec_list);
	
	 while(1){
		
		if(ListCount(rec_list)>0){
			pthread_mutex_lock (&mutex2);
			item = ListTrim(rec_list);
			pthread_mutex_unlock (&mutex2);
			printf("\n");
			printf("Received a datagram:");
			printf("%s",item); 
			printf("Enter your meassage in the box below:\n");
		}
	}
	free(item);
	pthread_mutex_destroy(&mutex2);
}




void recieve(LIST *rec_list){
	int remotelen,n;
	char *item;  	
  	local.sin_port = htons(atoi(IP[0]));
  	remotelen = sizeof(struct sockaddr_in);

  	while(1){
  		item = (char *)malloc(256*sizeof(char));
  		n = recvfrom(sock,item,sizeof(item),0,(struct sockaddr *)&remote,&remotelen);
  		ListPrepend(rec_list,item);
  		
	}
	
 }




 void sending(LIST *out_list){
  	
  	int length,n;
  	char *item;

	while(1){
		pthread_mutex_lock (&mutex1);
  		item = ListTrim(out_list);
	  	pthread_mutex_unlock (&mutex1);

  		if (sock < 0){
    		printf("socket");
  		}

  		remote.sin_port = htons(atoi(IP[1]));
  		remote.sin_family = AF_INET;
  		length = sizeof(struct sockaddr_in);

		n = sendto(sock,item,sizeof(item),0,&remote,length);
  		
 	}
 	free(item);
 	pthread_mutex_destroy(&mutex2);
 }




int main(int argc,char *argv[])
{
	int success;
	IP[0]=argv[1];
	IP[1]=argv[3];

	struct hostent *hp;
	LIST *rec_list=ListCreate();
	LIST *out_list=ListCreate();
	pthread_t thread1, thread2,thread3,thread4;


	if(argc !=4)
	{
		fprintf(stderr,"ERROR, no port provided\n");
	}
	clear();
	sock = socket(AF_INET,SOCK_DGRAM,0);
	length = sizeof(local);
	bzero(&local,length);
	local.sin_family=AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(atoi(argv[1]));
	

	hp = gethostbyname(argv[2]);
	bcopy((char *)hp->h_addr,(char *)&remote.sin_addr,hp->h_length);

	remote.sin_port = htons(atoi(argv[1]));
	success = bind(sock,(struct sockaddr *)&local, length);
	if(success == 0){
		printf("%s\n","Connected");
	}
	else{
		printf("%s\n","Connection Failed");
		exit(0);
	}
 
 	int a = pthread_create( &thread1, NULL, keyboard, (void*) out_list);
 	int b = pthread_create( &thread2, NULL, sending, (void*) out_list);
 	int c = pthread_create( &thread3, NULL, recieve, (void*) rec_list);
 	int d = pthread_create( &thread4, NULL, screen, (void*) rec_list);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);
	pthread_join( thread4, NULL);
}






