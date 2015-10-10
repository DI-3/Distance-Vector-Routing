#include "../include/global.h"
#define MAXNODE 15;


int tick_count_nodes[6];
int ticker_set[6];


bool  str_to_uint16(const char *str, uint16_t *res) {
  char *end;
  errno = 0;
  long val = strtol(str, &end, 10);
  if (errno || end == str || *end != '\0' || val < 0 || val >= 0x10000) {
    return false;
  }
  *res = (uint16_t)val;
  return true;
}

void dspl_rt_table(){
  int i,j;
  for(i=0;i<no_of_nodes;i++){
    //if(i == local_server_id - 1){
    printf("\n The server %d data :",i+1);
    for(j=0;j<5;j++){

     printf("\n Server Id: %d Server Port :%d, Server cost: %d, Next Hop: %d,Server distance : %d",routing_table[i][j].serv_id,
     routing_table[i][j].serv_port,routing_table[i][j].serv_cost,routing_table[i][j].next_hop_id,routing_table[i][j].distance);
    //}
}
}
struct sockaddr_in sa;
char *byte_order = (char *)malloc(16);

for(i= 0 ;i < no_of_ngnodes;i++){
  sa.sin_addr.s_addr = neighbor_nodes[i].ip_addr ;
  inet_ntop(AF_INET,&(sa.sin_addr),byte_order,32);
  printf("\nServer id :%d ,Server Port :%d , server cost:%d server ip:%s",neighbor_nodes[i].serv_id,
  neighbor_nodes[i].serv_port,neighbor_nodes[i].serv_cost,byte_order);
}
}



void crt_rt_tbl(){

  //routing_table[5][5];
  int i = 0;
  int j = 0;
  int cntr = 0;

  //routing_table arglist = (char **)malloc(4* sizeof(char*));
  routing_table = (rt_table_line **)malloc(5 * sizeof(rt_table_line));

  for(i=0; i < no_of_nodes ;i++){
    cntr = 0;
    routing_table[i] = (rt_table_line *)malloc(5*sizeof(rt_table_line));
    for(j=0 ; j < no_of_nodes  ;j++){

      routing_table[i][j] = ntwrk_nodes[cntr];
      if(i == j){
        routing_table[i][j].distance = 0;
        routing_table[i][j].serv_cost = 0;
        
      }
      else{
        routing_table[i][j].distance =  UINT16_MAX ; 
        routing_table[i][j].serv_cost =  UINT16_MAX ;
        
      }
        
      cntr++ ;
    }
    cntr = 0;
  }

   
}

int read_topology(char *argv){

  FILE * fp ; 
  char f_line[256];
  int line_cntr = 0;

  fp = fopen(argv,"r");
  if(fp == NULL ){
    printf("\n[PA1]$:File Error .Trying to request an invalid file.");
    return 0 ;
  }
  
  while(fgets(f_line,sizeof(f_line),fp)){

   printf("%s",f_line);

   if(line_cntr == 0)
     no_of_nodes = atoi(f_line);

   if(line_cntr == 1)
    no_of_ngnodes = atoi(f_line);

  if(line_cntr >= 2 && line_cntr <  (2 + no_of_nodes) ){
    addnodetonetwork(f_line); 
    if(line_cntr == (2 + no_of_nodes)-1)
      crt_rt_tbl();
  }

    //create the routing table data here 
  if(line_cntr >= (2 + no_of_nodes)  && line_cntr < (2 + no_of_nodes+no_of_ngnodes)){
    updt_ntwrk_cost(f_line);
    
  }
  //routing_table[local_server_id-1][local_server_id  - 1].next_hop_id = 0;
  line_cntr++ ;
}
 fclose(fp);
//dspl_rt_table();
//build_rtpkt_dump();
return 1 ;
}


