#include "../include/global.h"
#define MAXBUFFERLEN 50

/* This method intialises the connections and sets the listening port */

fd_set master ;    
fd_set read_fds ;
int fdmax;
int local_listener ;
struct addrinfo *addr_inf;
int packet_count;
int tick_count[5];

/**
 * THe function initialises the server listener port
 * @return 1 EXIT_SUCCESS
 */

int initialize_server(){

  int yes = 1;
  int rv;
  struct addrinfo hints,*ai,*p ;
  int listener ;
  char portnum[33];

  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  
  memset(&hints,0,sizeof hints);
  hints.ai_family =AF_INET ;
  hints.ai_socktype = SOCK_DGRAM ;
  hints.ai_flags = AI_PASSIVE ;
  sprintf(portnum,"%d",local_portnum);
  if((rv = getaddrinfo(NULL,portnum,&hints,&ai)) !=0 ){
    //error
    fprintf(stderr,"selectserver %s\n",gai_strerror(rv));
    exit(1);
  }

  for(p = ai; p!=NULL;p= p->ai_next){

   listener = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
   if(listener < 0){
    continue ;
  }

  // check 1 to dosetsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

  if(bind(listener,p->ai_addr,p->ai_addrlen) < 0) {
    close(listener);
    continue ;
  } 
  break ;
}
    //printf("Debug point2 %d",listener);
    //
addr_inf  = p ;
if(p == NULL) {

 fprintf(stderr,"selectserver  failed to bind%s\n",gai_strerror(rv));
 exit(2);

} 

freeaddrinfo(ai);

    //listen

// if(listen(listener,10) == -1){

//  perror("listen");
//  exit(3); 
// }

 // add the listtener to master set 

FD_SET(listener,&master);
FD_SET(0,&master);
fdmax = listener ;
//printf("\n [PA3]$: The server id: %d , Server Port: %d",local_server_id,local_portnum);
//printf("\n[PA3]$:");
//fflush(stdout);
return listener;  

}

/**
 * THe function listens in the listening port and delegates the function as per the 
 * values obtained
 * @param  sockfd 
 */

void listen_process(int sockfd){

 int i=0;
 int server_mode;
 //char *command_line;
 char cmd[100];   
 int length=0;
 int numbytes;
 char buffer[MAXBUFFERLEN];
 struct sockaddr_storage addr;
 socklen_t  addr_len;
 char adrs[INET_ADDRSTRLEN];
 rupdtpacket rcv_updt_pkt;
 rupdtpacket updt_pkt;
 struct timeval timeout;
 int it=0;  
 printf("[PA3]:");
 fflush(stdout);

 for(;;){
    
    read_fds = master;
    if(it == 0){
    timeout.tv_sec =  timeout_val ;
    timeout.tv_usec = 0;
    }
    /* if the select returns the value 0 then timeout has occurred 
    Call the method to send the update packet*/ 
    it++;
    server_mode = select(fdmax+1,&read_fds,NULL,NULL,&timeout); //sset the timer here 
    // if(select(fdmax + 1,&read_fds,NULL,NULL,NULL= -1){
    // perror("select");
    // exit(4);
    // }
    if(server_mode == 0){

    printf("Timeout received cool Dragon");
    update_neighbor_tick();
    displticker();
    send_update_packet_nnodes(sockfd);
  
    timeout.tv_sec =  timeout_val;
    timeout.tv_usec = 0;
    }
    
    char* res;
    for(i=0;i<=fdmax;i++){
   
     if(FD_ISSET(i,&read_fds)){

          //1.Read from the command from the input  
          if(i==0){

             if((res = fgets(cmd,sizeof cmd,stdin)) != NULL){
             length = strlen(cmd);
             cmd[length-1] = '\0';
             process_cmd(cmd,sockfd);
             //printf("\n[PA3]$:");
             //fflush(stdout);
             printf("[PA3]:\n");
             fflush(stdout);
            }
          }else{
          
          //2.Read from the socket
           if( i== sockfd ){
             addr_len = sizeof addr;  
             // if((numbytes = recvfrom(i,buffer,MAXBUFFERLEN-1,0,(struct sockaddr *)&addr,&addr_len))== -1){
             //  perror("Receive error");
             //  exit(1);
             // }
             size_t pkt_header_size = 8 ;
             size_t inf_pkt_size = 12 ;
             size_t pkt_size = no_of_nodes * inf_pkt_size + pkt_header_size;

             if((numbytes = recvfrom(i,&rcv_updt_pkt,pkt_size,0,(struct sockaddr *)&addr,&addr_len))== -1){
              perror("Receive error");
              exit(1);
             }
             if(numbytes > 0)
              updt_pkt = cnvrt_to_hostbyte(&rcv_updt_pkt);
              packet_count ++ ;
              dsplay_updtinfo(updt_pkt);
              set_routing_table(updt_pkt);  
              compute_bellman_ford();
             
            }
           } 
           

     }  

    } //end of the selection loop  
   
    
 } // end of the wait loop
   
 

}