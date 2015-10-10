# include "../include/global.h"
/*

*/
rt_table_line ntwrk_nodes[5];
rt_table_line neighbor_nodes[5];
int local_server_id;
int local_portnum  = 0;
static int ng_count = 0;

/**
 * THe function obtains the delimited string and ontains the data
 * values obtained
 * @param  data
 * @param option 
 */

char **getdlmtdstr(char *data,int optn){

 char *arg ;
 char *process_data;
 char **arglist;
 process_data = strtok(data,"\n");

 int arg_cnt=0;
 arg = strtok(process_data," "); 

 if(optn==0)
 arglist = (char **)malloc(4* sizeof(char*));
 else
 arglist = (char **)malloc(no_of_ngnodes * sizeof(char*));

 while(arg){
    arglist[arg_cnt] = (char *)malloc(50*sizeof(char)); 	
 	strcpy(arglist[arg_cnt],arg);
    arg_cnt++;
    arg = strtok(NULL," ");
 } 
   
 return arglist ;
}
/**
 * THe function adds the nodes to the network list
   * @param  f_line  - a line from the topology file
 */

void addnodetonetwork(char *f_line){

 int optn = 0;

 uint16_t serv_id,serv_port,serv_cost,next_hop_id;;
 uint32_t ip_addr ;
 struct sockaddr_in sa;
 char ipinput[INET_ADDRSTRLEN];
 
 //printf("FUNC1:%s",f_line);
 char **ntwrk_data  = getdlmtdstr(f_line,optn);
 //printf("THe network id:%s",ntwrk_data[1]);
 char *byte_order = malloc(sizeof(ntwrk_data[1]));
  
 /*save the server data nodes to a structure array
   index 1: server id   index 2: server ip
   index 3: server port
 
 */
    
 serv_id = atoi(ntwrk_data[0]);
 inet_pton(AF_INET, ntwrk_data[1], &(sa.sin_addr));
 inet_ntop(AF_INET,&(sa.sin_addr),byte_order,sizeof(ntwrk_data[1])*2);
 ipinput[strlen(ipinput) - 1] = 0;
 serv_port = atoi(ntwrk_data[2]);  
 serv_cost =  0 ;
 
 ntwrk_nodes[serv_id - 1].ip_addr  = sa.sin_addr.s_addr;
 ntwrk_nodes[serv_id - 1].serv_id  = serv_id;
 ntwrk_nodes[serv_id - 1].serv_port = serv_port;
 ntwrk_nodes[serv_id - 1].serv_cost = 0;
 ntwrk_nodes[serv_id - 1].next_hop_id = -1;
 ntwrk_nodes[serv_id - 1].serv_disable = 0;
 ntwrk_nodes[serv_id - 1].tick_count = 0 ;
 //printf("Server Address id-%d",ntwrk_nodes[serv_id - 1].serv_id);

}

void cleanupStrings ( char ** strArray, int numberOfStrings )
{
    int i;

    for ( i = 0; i < numberOfStrings; i ++ )
    {
        //Should be checking to see if this is a null pointer.
        free ( strArray [ i ] );
    }

    //Once the strings themselves are freed, free the actual array itself.
    free ( strArray );
}

/**
 * THe function updates the n/w node neighbor cost
  * @param  f_line - a line from the file
 
 */

void updt_ntwrk_cost(char *f_line){

 int optn = 1 ;
 char **ntwrk_data  = getdlmtdstr(f_line,optn); 
 uint16_t serv_id,ng_serv_id;
 
 serv_id = atoi(ntwrk_data[0]);
 local_server_id = serv_id; 
 ng_serv_id = atoi(ntwrk_data[1]);
 routing_table[local_server_id-1][local_server_id  - 1].next_hop_id = local_server_id;
 local_portnum = routing_table[local_server_id-1][local_server_id-1].serv_port;
 //printf("Local  port %d",local_portnum);
 routing_table[serv_id - 1][ng_serv_id - 1].serv_cost = atoi(ntwrk_data[2]);
 routing_table[serv_id - 1][ng_serv_id - 1].distance = atoi(ntwrk_data[2]);
 routing_table[serv_id - 1][ng_serv_id - 1].next_hop_id = ng_serv_id;
 routing_table[ng_serv_id - 1][serv_id - 1].serv_cost = atoi(ntwrk_data[2]);
 routing_table[ng_serv_id - 1][serv_id - 1].distance = atoi(ntwrk_data[2]);
 routing_table[ng_serv_id - 1][serv_id - 1].next_hop_id = ng_serv_id;
 neighbor_nodes[ng_count] = routing_table[local_server_id -1 ][ng_serv_id - 1];
 tick_count_nodes[ng_serv_id] = 3; 
 //neighbor_nodes[ng_count].serv_disable = 0 ;
 ng_count++;
 cleanupStrings(ntwrk_data,3);
}

//reference - http://stackoverflow.com/questions/4085372/how-to-return-a-string-array-from-a-function

rupdtpacket cnvrt_to_hostbyte(rupdtpacket *r_updt_pkt){

  rupdtpacket rcvd_updt_pkt;
    int i;
  rcvd_updt_pkt.n_updtfiedls = ntohs(r_updt_pkt->n_updtfiedls);
 rcvd_updt_pkt.ip_addr = ntohl(r_updt_pkt->ip_addr);
 rcvd_updt_pkt.serv_port = ntohs(r_updt_pkt->serv_port);

 for(i=0;i< no_of_nodes;i++){
   rcvd_updt_pkt.servers[i].serv_ipaddr = ntohl(r_updt_pkt->servers[i].serv_ipaddr) ;
   rcvd_updt_pkt.servers[i].serv_port   = ntohs(r_updt_pkt->servers[i].serv_port);
   rcvd_updt_pkt.servers[i].filler = 0;
   rcvd_updt_pkt.servers[i].serv_id  = ntohs(r_updt_pkt->servers[i].serv_id); 
   rcvd_updt_pkt.servers[i].serv_cost = ntohs(r_updt_pkt->servers[i].serv_cost);
 } 

   return rcvd_updt_pkt ;
}

void sort_updt_packet(rupdtpacket updt_pkt){

  serv_infpkt swap_server;
  int i = 0;
  int j= 0; 

   for(i = 0 ;i < (no_of_nodes - 1) ;i++){
    for(j = 0 ;j < (no_of_nodes -i - 1) ; j++){

      if(updt_pkt.servers[j].serv_id > updt_pkt.servers[j+1].serv_id){
        swap_server = updt_pkt.servers[j];
        updt_pkt.servers[j] = updt_pkt.servers[j+1] ;
        updt_pkt.servers[j+1] = swap_server ;

      }
    }
   }
 for(i = 0;i<no_of_nodes;i++){
 //printf("%-15d%-15d\n",updt_pkt.servers[i].serv_id,updt_pkt.servers[i].serv_cost);
 cse4589_print_and_log("%-15d%-15d\n",updt_pkt.servers[i].serv_id,updt_pkt.servers[i].serv_cost);
 }
}

void dsplay_updtinfo(rupdtpacket updt_pkt){
 int i=0;
 //printf("RECEIVED A MESSAGE FROM SERVER %d\n",updt_pkt.servers[0].serv_id);
 cse4589_print_and_log("RECEIVED A MESSAGE FROM SERVER %d\n",updt_pkt.servers[0].serv_id);
 // sort the update packet
 sort_updt_packet(updt_pkt);

 // for(i = 0;i<no_of_nodes;i++){
 // //printf("%-15d%-15d\n",updt_pkt.servers[i].serv_id,updt_pkt.servers[i].serv_cost);
 // cse4589_print_and_log("%-15d%-15d\n",updt_pkt.servers[i].serv_id,updt_pkt.servers[i].serv_cost);
 // }

}


void dsplay_rt_table(){
  int i,j;
  //printf("\n");
  for(i=0;i<no_of_nodes;i++){
    if(i == local_server_id - 1){
    
    for(j=0;j< no_of_nodes;j++){
     //printf("%-15d%-15d%-15d\n",routing_table[local_server_id - 1][j].serv_id,
     //routing_table[local_server_id-1][j].next_hop_id,routing_table[local_server_id -1][j].distance);
     cse4589_print_and_log("%-15d%-15d%-15d\n",routing_table[local_server_id - 1][j].serv_id,
     routing_table[local_server_id-1][j].next_hop_id,routing_table[local_server_id -1][j].distance);
     // printf("\n Server Id: %d Server Port :%d, Server cost: %d, Next Hop: %d,Server distance : %d",routing_table[i][j].serv_id,
     // routing_table[i][j].serv_port,routing_table[i][j].serv_cost,routing_table[i][j].next_hop_id,routing_table[i][j].distance);
    }
}
}
}

/**
 * THe function validates if the node is a n/w neighbor node 
  * @param  cmds - pointer to command arguments passed
  * optn   - the option to select
 
 */

int validate_nodes(char** cmds,int optn){

 //Check if the argument 1 is the local server Id and argument 2 is the neighbor id
   int i=0;
   int flag = 0;
   int ng_node = 0;
   if(optn == 0){
   if(atoi(cmds[1]) != local_server_id ){
      printf("\nThe server Id is invalid");
      return -1;
   }}
   if(optn==0)
    ng_node = atoi(cmds[2]);
   else
    ng_node = atoi(cmds[1]);  
   for(i=0;i<no_of_ngnodes;i++){

       if(neighbor_nodes[i].serv_id == ng_node){
        
        flag = 1 ;

       }

   }

   if(flag == 0){
        printf("\nThe server ID - 2 has to be a neighbor node");
        return -1;    
   }
   return 1 ;
}

void update_nodes_nxthop(int ng_node){

  int i = 0; 
  for(i=0 ;i< no_of_nodes ;i++){

    if(routing_table[local_server_id - 1][i].next_hop_id == ng_node){
      routing_table[local_server_id -1][i].distance = UINT16_MAX ;
      routing_table[local_server_id -1][i].next_hop_id = -1;
    }
  }
}
  
void update_rout_info(char** cmds){

 int ng_index = atoi(cmds[2]);
 int new_cost =atoi(cmds[3]);
 routing_table[local_server_id -1][ng_index - 1].serv_cost = new_cost ;
 dspl_rt_table();


}  

int disable_link(char **cmd){
  int retval = 0;
  int ng_node = 0 ;
  int index;
  if((retval =validate_nodes(cmd,1)) == 1){
    ng_node = atoi(cmd[1]);
    printf("\nDisable the link to %d",ng_node);
    for(index = 0 ; index < no_of_ngnodes ;index++){
      if(neighbor_nodes[index].serv_id == ng_node){
       neighbor_nodes[index].serv_disable = 1;
       routing_table[local_server_id -1][ng_node -1].serv_cost = UINT16_MAX ;
       routing_table[local_server_id -1][ng_node -1].distance = UINT16_MAX ;
       routing_table[local_server_id -1][ng_node -1].next_hop_id = -1 ;                
       //call the method to set the distance vector for nodes where neighboring node is  next hop to inf
      update_nodes_nxthop(ng_node); 
      compute_bellman_ford();
      }
    }

    // for(index = 0 ; index < no_of_ngnodes ; index++){
    //   printf("THe disable field value %d :%d ",neighbor_nodes[index].serv_id,neighbor_nodes[index].serv_disable);
    // }
    return 1;
  }else
  return -1;

}

void update_neighbor_tick(){
  
 int i=0 ; 
 for(i=1;i<=no_of_nodes;i++){
   if(ticker_set[i] == 1){
    tick_count_nodes[i] -= 1 ;
    if(tick_count_nodes[i] == 0)
    update_link_infinity(i);

   }
   
 }

}

void update_link_infinity(int ng_server_id){
   int i = 0;
   printf("\n the tick update set %d,%d,%d,%d,%d",ticker_set[1],ticker_set[2],ticker_set[3],ticker_set[4],ticker_set[5]); 
   printf("\n the tick update set %d,%d,%d,%d,%d",tick_count_nodes[1],tick_count_nodes[2],tick_count_nodes[3],tick_count_nodes[4],tick_count_nodes[5]); 
   routing_table[local_server_id - 1][ng_server_id -1].serv_cost = UINT16_MAX ;
   routing_table[local_server_id - 1][ng_server_id -1].next_hop_id = - 1;
   for(i = 0; i < no_of_ngnodes; i ++){
    if(neighbor_nodes[i].serv_id == ng_server_id){

     neighbor_nodes[i].serv_disable = 1 ;

     break;
   }}

    for(i=0 ; i< no_of_nodes ; i++){
      if(ntwrk_nodes[i].serv_id == ng_server_id )
      {
        routing_table[local_server_id - 1][i].next_hop_id = -1 ;
        routing_table[local_server_id - 1][i].serv_cost = UINT16_MAX ;
      }
    }
   //neighbor_nodes[ng_server_id - 1].serv_disable = 1 ;
   update_nodes_nxthop(ng_server_id);  
   compute_bellman_ford();
   build_rtpkt_dump(0);
   printf("\n Changed the link cost %d to %d to infinity",local_server_id,ng_server_id);
   
}

void displticker(){
  printf("\n the tick update set %d,%d,%d,%d,%d",ticker_set[1],ticker_set[2],ticker_set[3],ticker_set[4],ticker_set[5]); 
   printf("\n the ticker count set %d,%d,%d,%d,%d",tick_count_nodes[1],tick_count_nodes[2],tick_count_nodes[3],tick_count_nodes[4],tick_count_nodes[5]); 
}