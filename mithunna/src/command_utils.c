#include "../include/global.h"
//#include "../include/logger.h"

/* THe method provides the delimited strings
   for processing
*/
commands getdlmtdcmdstr(char *cmd){

 char *arg ;
 char **arglist;
 commands cmd_pkt ; 
 int arg_cnt=0;
 arg = strtok(cmd," "); 

 if(arg == '\0' || strcmp(arg,"\n") == 0 ){

   //printf("[PA1]$:");
   cmd_pkt.no_of_args = -1;
   return cmd_pkt ;

 }
 
 arglist = (char **)malloc(4* sizeof(char*));

 while(arg){
  arglist[arg_cnt] = (char *)malloc(50*sizeof(char)); 	
  strcpy(arglist[arg_cnt],arg);
  arg_cnt++;
  arg = strtok(NULL," ");
} 
cmd_pkt.no_of_args = arg_cnt;  
cmd_pkt.arglist = arglist ;
cmd_pkt.cmnd = cmd ;
//cleanupStrings(arglist,arg_cnt - 1);
return cmd_pkt ;
}

/* This method is called when the command crash is executed by the user */
void gotosleep(){
  fflush(stdout);
  for(;;){

  }
}

rupdtpacket build_rtpkt_dump(int mode){

 // local_server_id
 rupdtpacket updt_packet;
 rupdtpacket *updt_pkt ; 
 int i;
 int j = 0;
 updt_pkt = &updt_packet;
 size_t size_ntwrk_node_pkt = 96 ;
 //dspl_rt_table();
 updt_packet.n_updtfiedls = htons(no_of_nodes);
 updt_packet.ip_addr = (routing_table[local_server_id - 1][local_server_id -1].ip_addr);
 updt_packet.serv_port = htons(routing_table[local_server_id -1][local_server_id-1].serv_port);

 i = 0;
 updt_packet.servers[i].serv_ipaddr = (routing_table[local_server_id-1][local_server_id-1].ip_addr) ;
 updt_packet.servers[i].serv_port   = htons(routing_table[local_server_id-1][local_server_id-1].serv_port);
 updt_packet.servers[i].filler = 0;
 updt_packet.servers[i].serv_id  = htons(routing_table[local_server_id -1][local_server_id-1].serv_id); 
 updt_packet.servers[i].serv_cost = htons(routing_table[local_server_id-1][local_server_id-1].distance);

 for(i=1;j< no_of_nodes;j++){
   if(j != (local_server_id - 1)){
   updt_packet.servers[i].serv_ipaddr = (routing_table[local_server_id-1][j].ip_addr) ;
   updt_packet.servers[i].serv_port   = htons(routing_table[local_server_id-1][j].serv_port);
   updt_packet.servers[i].filler = 0;
   updt_packet.servers[i].serv_id  = htons(routing_table[local_server_id -1][j].serv_id); 
   updt_packet.servers[i].serv_cost = htons(routing_table[local_server_id-1][j].distance);
   i++;
   }
   
 }

 if(mode == 1){
 j= 0;
 cse4589_dump_packet(&updt_packet,size_ntwrk_node_pkt * no_of_nodes); 
 }
 return updt_packet ;

}

void dspl_update_pkt(rupdtpacket r){
  int i =0;
  for(i=0;i<no_of_nodes;i++){
//    printf("\n -----Server-id :%d Server-cost :%d",r.servers[i].serv_id,r.servers[i].serv_cost);
  }
}

/**
 * THe function gets the commands and processes the command and calls the functions
 * values obtained
 * @param  cmd - command
 * @param  sockfd - the listening socket fd  
 */


int process_cmd(char *cmd,int sockfd){

 char * msg;
 commands cmd_pkt;
 char *cmds;
 cmds =strdup(cmd);
 cmd_pkt = getdlmtdcmdstr(cmd);

 if(cmd_pkt.no_of_args == -1)
  return 0;
 //printf("%s:%s\n",cmd_pkt.cmnd,"");
 /*call method to validate the nodes*/
 if(strcasecmp(cmd_pkt.arglist[0],"UPDATE")==0){
    msg ="Invalid Arguments.Usage - update <server-ID1> <server-ID2> <Link Cost>";
   if(cmd_pkt.no_of_args != 4 ){
     //printf("%s:%s\n",cmd_pkt.cmnd,msg);
     cse4589_print_and_log("%s:%s\n",cmds,msg);
     //printf("\n[PA3]$:Invalid Arguments.Usage - update <server-ID1> <server-ID2> <Link Cost>");
     return -1;
   }
   
   if(validate_nodes(cmd_pkt.arglist,0) == -1){
     msg ="Invalid Arguments.server-ID1 - Node Id server-ID2:Must be neighbor node Id";
     cse4589_print_and_log("%s:%s\n",cmds,msg);
   
   return -1;
   }
   else{
    update_rout_info(cmd_pkt.arglist); 
    compute_bellman_ford();
  }
   //printf("%s:SUCCESS\n",cmd_pkt.cmnd);
   cse4589_print_and_log("%s:SUCCESS\n",cmds);
   
 }/*call method to send the update packet */ 
   else if(strcasecmp(cmd_pkt.arglist[0],"STEP")==0){
    if(cmd_pkt.no_of_args > 1 ){
     msg ="Invalid Arguments.Usage - step"; 
     cse4589_print_and_log("%s:%s\n",cmds,msg);
     //printf("\n[PA3]$:Invalid Arguments.Usage - step");
     return -1;
   }
   send_update_packet_nnodes(sockfd);
   //printf("%s:SUCCESS\n",cmd_pkt.cmnd);
   cse4589_print_and_log("%s:SUCCESS\n",cmds); 

 }
 else if(strcasecmp(cmd_pkt.arglist[0],"PACKETS")==0){
  if(cmd_pkt.no_of_args > 1 ){
   msg ="Invalid Arguments.Usage - packets";
   cse4589_print_and_log("%s:%s\n",cmds,msg);
   //printf("\n[PA3]$:Invalid Arguments.Usage - packets");
   return -1;
 }
  //display the packets received
  //printf("\n The number of packets sent : %d",packet_count);
  cse4589_print_and_log("%s:SUCCESS\n",cmds);
  cse4589_print_and_log("%d\n",packet_count);
  //printf("%s:SUCCESS\n",cmd_pkt.cmnd);
  packet_count = 0 ;

}else if(strcasecmp(cmd_pkt.arglist[0],"DISPLAY")==0){
  if(cmd_pkt.no_of_args > 1 ){
    
   msg ="Invalid Arguments.Usage - display";
   cse4589_print_and_log("%s:%s\n",cmds,msg);
   //printf("\n[PA3]$:Invalid Arguments.Usage - display");
   return -1;
 }
  cse4589_print_and_log("%s:SUCCESS\n",cmds);
  dsplay_rt_table();
}else if(strcasecmp(cmd_pkt.arglist[0],"DISABLE")==0){
  if(cmd_pkt.no_of_args != 2  ){
   msg ="Invalid Arguments.Usage - disable <server-id>"; 
   //printf("\n[PA3]$:Invalid Arguments.Usage - disable <server-ID>");
   cse4589_print_and_log("%s:%s\n",cmds,msg);
   return -1;
 } 
  int iret = 0;
  iret = disable_link(cmd_pkt.arglist); 
    //printf("%s:SUCCESS\n",cmd_pkt.cmnd);
  msg ="Invalid Arguments.Usage - Server Id has to be a neighbor node id"; 
  if(iret == 1)
  cse4589_print_and_log("%s:SUCCESS\n",cmds);
  else
  cse4589_print_and_log("%s:%s\n",cmds,msg);

}else if(strcasecmp(cmd_pkt.arglist[0],"CRASH")==0){
 if(cmd_pkt.no_of_args > 1 ){
   msg ="Invalid Arguments.Usage - crash";
   cse4589_print_and_log("%s:%s\n",cmds,msg);
   //printf("\n[PA3]$:Invalid Arguments.Usage - crash");
   return -1;
 }
  gotosleep();
 
}else if(strcasecmp(cmd_pkt.arglist[0],"DUMP")==0){
  if(cmd_pkt.no_of_args > 1 ){
   msg ="Invalid Arguments.Usage - dump"; 
   //printf("\n[PA3]$:Invalid Arguments.Usage - dump");
   cse4589_print_and_log("%s:%s\n",cmds,msg);
   return -1;
 }
 build_rtpkt_dump(1);
 //printf("%s:SUCCESS\n",cmd_pkt.cmnd);
 cse4589_print_and_log("%s:SUCCESS\n",cmds);
}else if(strcasecmp(cmd_pkt.arglist[0],"academic_integrity")== 0)
{
  //printf("\n I have read and understood the course academic integrity policy located at http://www.cse.buffalo.edu/faculty/dimitrio/courses/cse4589_f14/index.html#integrity");
  cse4589_print_and_log("I have read and understood the course academic integrity policy located at http://www.cse.buffalo.edu/faculty/dimitrio/courses/cse4589_f14/index.html#integrity");
}else{
 printf("\n[PA3]$:Command not found");
 fflush(stdout);//cse4589_print_and_log("Command not found\n");
 return -1;
}
free(cmds);
return 1;
}


