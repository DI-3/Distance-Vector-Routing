#include "../include/global.h"

/**
 * The function send update packets to the neighboring nodes.
 *
 * @param  sockfd The socket FD of the node to which update is to be sent
 * @return 1 EXIT_SUCCESS
 */
int send_update_packet_nnodes(int sockfd){
	int index;
	int ip_address;
	char portnum[16];
	rupdtpacket rt_upd_pkt;  
	struct sockaddr_in sa;
	char *ip_address_str = (char *)malloc(16);

	rt_upd_pkt = build_rtpkt_dump(1);
//loop through the neighbouring nodes and send the UDP packet to all the nodes
	for(index = 0 ; index < no_of_ngnodes; index++){
        
		if(neighbor_nodes[index].serv_disable == 0){
		ip_address = neighbor_nodes[index].ip_addr ;
		sa.sin_addr.s_addr = neighbor_nodes[index].ip_addr ;
		inet_ntop(AF_INET,&(sa.sin_addr),ip_address_str,32);
		//printf("THe server address where data  is :%s",ip_address_str);
		sprintf(portnum,"%d",neighbor_nodes[index].serv_port);
		send_update_packet(ip_address_str,portnum,sockfd,rt_upd_pkt);

		}
	}

	return 1;
}

/**
 * THe function sends the data to the node whose IP and portnum is given
 *
 * @param  ipaddress 
 * @param  sockfd 
 * @param  rupdtpacket -- the update packet 
 * @return 1 EXIT_SUCCESS
 */

int send_update_packet(char *ipaddress,char* portnum,int sockfd,rupdtpacket rt_upd_pkt){


	struct addrinfo hints,*servinfo,*p;
	int rv=0;
	int numbytes;
	char buffer[200];
	char *str;
	str = "The dragon code execute";
	strcpy(buffer, str);
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if(rv = getaddrinfo(ipaddress, portnum, &hints,&servinfo)){

		fprintf(stderr,"getaddrinfo:%s\n",gai_strerror(rv));
		return 1;
	}

	for(p=servinfo;p!=NULL;p=p->ai_next){

		if((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1){

			perror("Client Socket");
			continue;
		}

		break;
	}

	if(p==NULL){

		fprintf(stderr,"Failed to bind to the socke\n");
		return 2;
	}

	size_t pkt_header_size = 8 ;
	size_t inf_pkt_size = 12 ;
	size_t pkt_size = no_of_nodes * inf_pkt_size + pkt_header_size;

//sendto(int __fd, const void *__buf, size_t __n, int __flags, const struct sockaddr *__addr, socklen_t __addr_len)
	if((numbytes = sendto(sockfd,&rt_upd_pkt,pkt_size,0,p->ai_addr,p->ai_addrlen)) == -1){

		perror("sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("THe client sent %d  bytest to the %s\n",numbytes,ipaddress);
	close(sockfd);
	return 0;

}

/**
 * THe function sends the data to the node whose IP and portnum is given
 *
 * @param  ipaddress 
 * @param  sockfd 
 * @param  rupdtpacket -- the update packet 
 * @return 1 EXIT_SUCCESS
 */

void compute_new_route(rupdtpacket updt_pkt){


	int i,j;
	int i_curr_distance,i_calc_distance ;
	serv_infpkt V;
	int neighbor_id ;

   	V = updt_pkt.servers[0]; 
	neighbor_id = updt_pkt.servers[0].serv_id;

    if(ticker_set[neighbor_id] == 0)
    	ticker_set[neighbor_id] = 1 ;
    else{
    	tick_count_nodes[neighbor_id] = 3;
    }

   
	for(i=1 ;i <= no_of_nodes;i++){
		if(i != local_server_id ){
			i_curr_distance = routing_table[local_server_id - 1][i-1].distance;
			for(j=0 ; j < updt_pkt.n_updtfiedls ; j++){
        		if( i == (updt_pkt.servers[j].serv_id)){
					break;
				} 
			}
			i_calc_distance = routing_table[local_server_id -1 ][neighbor_id -1 ].serv_cost + updt_pkt.servers[j].serv_cost;
			if(i_calc_distance < i_curr_distance){
				i_curr_distance = i_calc_distance;
				routing_table[local_server_id - 1][i-1].distance = i_calc_distance; 
				routing_table[local_server_id - 1][i-1].next_hop_id = neighbor_id ;
				routing_table[i-1][local_server_id -1].distance = i_calc_distance ;
				routing_table[i-1][local_server_id -1].next_hop_id = neighbor_id ;
			}

		} 
	} 


}


void compute_new_route_all(rupdtpacket updt_pkt){


	int i,j;
	int i_curr_distance,i_calc_distance ;
	serv_infpkt V;
	int neighbor_id ;
	int k;

    //tick_count_nodes[5];
    //ticker_set[5];


	V = updt_pkt.servers[0]; 
	neighbor_id = updt_pkt.servers[0].serv_id;

    if(ticker_set[neighbor_id] == 0)
    	ticker_set[neighbor_id] = 1 ;
    else{
    	tick_count_nodes[neighbor_id] = 3;
    }
   int ind=0;
   
   for(k= 0 ;k < no_of_nodes  ;k++){
   	ind = 1;
	for(i=0 ;i < no_of_nodes ;i++){
	   // if(i != k ){
			i_curr_distance = routing_table[k][i].distance;
			for(j=0 ; j < updt_pkt.n_updtfiedls ; j++){
        		if( ind == (updt_pkt.servers[j].serv_id)){
					break;
				} 
			}
			i_calc_distance = routing_table[k][neighbor_id -1 ].serv_cost + updt_pkt.servers[j].serv_cost;
			if(i_calc_distance < i_curr_distance){
				i_curr_distance = i_calc_distance;
				routing_table[k ][i].distance = i_calc_distance; 
				routing_table[k ][i].next_hop_id = neighbor_id ;
				routing_table[i][k ].distance = i_calc_distance ;
				routing_table[i][k ].next_hop_id = neighbor_id ;
			}

		//} 
		ind++;
	} 
   }

}


void compute_routing_locally(){

int k,j;
int i_curr_distance,i_calc_distance ;
//for(i=0;i<no_of_nodes;i++){

 for(j=0 ;j< no_of_nodes ;j++){
   i_curr_distance = routing_table[local_server_id -1][j].distance; 
   for(k=0 ;k<no_of_nodes;k++){
      
      i_calc_distance =  routing_table[local_server_id -1][k].serv_cost + routing_table[k][j].distance ;
      if(i_calc_distance < i_curr_distance){
   	   i_curr_distance = i_calc_distance;
   	   routing_table[local_server_id -1][j].distance = i_calc_distance ;
      }

   }

  }
}

 
void set_routing_table(rupdtpacket updt_pkt){

int i,j,k;
int n_node_id ;

for(k=0;k < no_of_nodes;k++){
	if(ntwrk_nodes[k].serv_port == updt_pkt.serv_port && htonl(ntwrk_nodes[k].ip_addr) == updt_pkt.ip_addr){
  
      n_node_id = ntwrk_nodes[k].serv_id ;
		
	}
}

//n_node_id = updt_pkt.servers[0].serv_id ;
int index = 0 ;

if(ticker_set[n_node_id] == 0)
    	ticker_set[n_node_id] = 1 ;
else{
    	tick_count_nodes[n_node_id] = 3;
}

for(i = 1;i <= no_of_nodes ;i++){
    //index = 1;
    for(j=0; j< updt_pkt.n_updtfiedls ;j++){

         if(i == updt_pkt.servers[j].serv_id){
         	break;
         }   
    }
    routing_table[n_node_id -1][i - 1].distance =  updt_pkt.servers[j].serv_cost;  
 }  
    //dsplay_rt_table();

}

void compute_bellman_ford(){

int i,j,ng_node_id; 
int i_current_distance;
int i_calc_distance,next_hop_id;

for(i=0; i < no_of_nodes;i++){
  
  if(i == local_server_id -1)
  continue;
  i_current_distance = UINT16_MAX ;
  for(j = 0 ; j < no_of_ngnodes ;j++) 
  {
       ng_node_id = neighbor_nodes[j].serv_id ;
       i_calc_distance = routing_table[local_server_id - 1][ng_node_id - 1].serv_cost + 
                                     routing_table[ng_node_id - 1][i].distance;
       
       if(i_calc_distance <  i_current_distance){
         i_current_distance = i_calc_distance ;
         next_hop_id = ng_node_id ;
       }  
        
  }
  
  routing_table[local_server_id - 1][i].distance = i_current_distance ;
  routing_table[local_server_id -1][i].next_hop_id =next_hop_id;
  if(routing_table[local_server_id - 1][i].distance == UINT16_MAX)
   routing_table[local_server_id -1][i].next_hop_id = -1;
  
  routing_table[i][local_server_id - 1].distance = i_current_distance ;
  routing_table[i][local_server_id -1].next_hop_id =next_hop_id;
}

  //dsplay_rt_table();

}