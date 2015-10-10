#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/logger.h"

extern int no_of_nodes ;
extern int no_of_ngnodes ;
extern int local_server_id;
extern int local_portnum;
extern fd_set master ;    
extern fd_set read_fds ;
extern int fdmax;
extern int local_listener ;

typedef struct 
{
   uint32_t serv_ipaddr;
   uint16_t serv_port ;
   uint16_t filler;
   uint16_t serv_id ;
   uint16_t serv_cost ;

}serv_infpkt ;


typedef struct 
{
   uint16_t n_updtfiedls;
   uint16_t serv_port;
   uint32_t ip_addr;   
   serv_infpkt servers[5];
 
}rupdtpacket;


typedef struct 
{
	uint16_t serv_id;
	uint16_t serv_port;
	uint16_t serv_cost ;
	uint32_t ip_addr ;
  int16_t next_hop_id;
  uint16_t distance ;
  uint16_t serv_disable; 
  uint16_t tick_count; 
}rt_table_line;

typedef struct {

  int no_of_args;
  char **arglist ;
  char *cmnd;
}commands;


//extern rt_table_line routing_table[5][5];
extern rt_table_line ntwrk_nodes[5];
extern int tick_count_nodes[6];
extern int ticker_set[6];
extern int packet_count; 
extern int timeout_val ;
rt_table_line** routing_table;
rupdtpacket build_rtpkt_dump(int mode);
extern struct addrinfo *addr_inf;
extern rt_table_line neighbor_nodes[5]; 
void addnodetonetwork(char *f_line);
void addnodetonglist(char *f_line); 
int initialize_server();
int read_topology();
int process_cmd(char *cmd,int sockfd);
int send_update_packet();
void listen_process(int sockfd);
int send_update_packet_nnodes();
void display_rt_table();
rupdtpacket cnvrt_to_hostbyte(rupdtpacket *r_updt_pkt);
int validate_nodes(char** cmd,int optn);
void update_rout_info(char **cmd);
int disable_link(char** cmd); 
void update_neighbor_tick();
void update_link_infinity(int ng_id);
void displticker();
void set_routing_table(rupdtpacket updt_pkt); 
void update_nodes_nxthop(int ng_node);
#endif
