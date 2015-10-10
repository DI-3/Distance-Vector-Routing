// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h> 
// #include <stdint.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <stddef.h>
// #include "../include/global.h"

// // typedef struct 
// // {
// //    uint16_t serv_port ;
// //    uint16_t filler;
// //    uint16_t serv_id ;
// //    uint16_t serv_cost ;

// // }serv_infpkt ;


// // typedef struct 
// // {
// //    uint16_t n_updtfiedls;
// //    uint16_t serv_port;
// //    struct in_addr;   
// //    serv_infpkt servers[5];
 
// // }rupdtpacket;


// // typedef struct rt_table_line
// // {
// // 	uint16_t serv_id;
// // 	uint16_t serv_port;
// // 	uint16_t serv_cost ;
// // 	int sock_desc;
// //   struct in_addr ;
// //   struct rt_table_line *next ;
// // };
// rt_table_line routing_table[5][5];

// point points[2][2];
// int no_of_nodes = 0;
// int no_of_ngnodes = 0;

// void initialize1(){

//  int i = 0;

//  int j = 0;

//  for (i=0 ;i <=2 ;i++){
        
//         for(j=0;j<=2;j++){

//           points[i][j].x = 5;
//           points[i][j].y = 5;
//         }

//  }
// }
//  void printvals1(){
//   int i,j;
//   for (i=0 ;i <=2 ;i++){
        
//         for(j=0;j<=2;j++){

//           printf("\t%d : %d",points[i][j].x,points[i][j].y);

//         }

//         printf("\n");
//  }
// }


// void initialize(){

//  int i = 0;

//  int j = 0;

//  for (i=0 ;i <=2 ;i++){
        
//         for(j=0;j<=2;j++){

//           routing_table[i][j].serv_id = 6;
//           routing_table[i][j].serv_cost = 6;
//         }

//  }
// }
//  void printvals(){
//   int i,j;
//   for (i=0 ;i <=2 ;i++){
        
//         for(j=0;j<=2;j++){

//           printf("\t%d : %d",routing_table[i][j].serv_id,routing_table[i][j].serv_cost);

//         }

//         printf("\n");
//  }
// }


// void testmain(){

//  // char *str ;
//  // rupdtpacket *rpacket;; ;
//  // rpacket = (rupdtpacket *)malloc(sizeof(rupdtpacket));
//  // printf("aaasThe dragon started coding\n");
//  // rpacket->n_updtfiedls = 5;
//  // rpacket->serv_port = 9041;
//  // //rpacket->filler =0x0;
//  // str = "asdf";
//  // initialize();
//  printvals();
//  //read_topology(str);
// }