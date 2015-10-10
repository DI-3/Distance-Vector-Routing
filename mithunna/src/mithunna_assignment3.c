/**
 * @mithunna_assignment3
 * @author  Mithun Nagesh <mithunna@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include "../include/global.h"
#include "../include/logger.h"

/* The pointer to routing table */
rt_table_line** routing_table;
int timeout_val;
int no_of_nodes = 0;
int no_of_ngnodes = 0;

int isNumber(char *input)
{
    while (*input){
        if (!isdigit(*input))
            return 0;
        else
            input += 1;
    }

    return 1;
}

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
	
    char *file_path;
    
    int opt ; 
    int sock_fd;
	/*Init. Logger*/
	cse4589_init_log();

	/*Clear LOGFILE and DUMPFILE*/
	fclose(fopen(LOGFILE, "w"));
	fclose(fopen(DUMPFILE, "wb"));

	/*Start Here*/
	  //process the command line arguments
    if(argc != 5){
   	fprintf(stderr, "Missing arguments\n");
	  printf("Usage: %s -t topology file -i timeouut\n", argv[0]);
   	return -1;
   }
    /* 
     Get the command line arguments and process it     
    */

     while((opt = getopt(argc, argv,"t:i:")) != -1){
       switch (opt){
           case 't':   if(isNumber(optarg)){
                       
                          fprintf(stderr,"Invalid value for -t\n");;
           			   }

                       // strcpy(file_path,optarg);   //to do add validatin for string
                       file_path = strdup(optarg);
                       break;

           case 'i':   if(!isNumber(optarg)){
                           fprintf(stderr, "Invalid value for -i\n");
                           return -1;
                       }
                       timeout_val = atoi(optarg);
                       break;

           case '?':   break;

           default:    printf("Usage: %s -t topology file -i timeout\n", argv[0]);
                       return -1;
       }
   }
  
  read_topology(file_path);
  sock_fd = initialize_server();
  listen_process(sock_fd);
	return 0;
  
}
