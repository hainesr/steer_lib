/*---------------------------------------------------------------------------
  This file contains a very simple example of a steering-enabled 
  application.

  (C) Copyright 2002, 2004, University of Manchester, United Kingdom,
  all rights reserved.

  This software is produced by the Supercomputing, Visualization and
  e-Science Group, Manchester Computing, University of Manchester
  as part of the RealityGrid project (http://www.realitygrid.org),
  funded by the EPSRC under grants GR/R67699/01 and GR/R67699/02.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
  CORRECTION.

  Authors........: Andrew Porter, Robert Haines

---------------------------------------------------------------------------*/
#include "ReG_Steer_Appside.h"
#include <unistd.h> /* for 'sleep' */

int main(){

  /* No. of 'simulation' loops to do */
  const int nloops = 500000;

  /* For steering */
  int    status;
  int    numCommands;
  int    commands[REG_INITIAL_NUM_CMDS];
  int    num_recvd_cmds;
  int    recvd_cmds[REG_MAX_NUM_STR_CMDS];
  char** recvd_cmd_params;
  int    num_params_changed;
  char** changed_param_labels;

  float temp     = 55.6;
  int   finished = REG_FALSE;
  int   i, icmd;
  
  /*---------- End of declarations ------------*/

  /* Initialise & enable the steering library */

  Steering_enable(REG_TRUE);

  numCommands = 2;
  commands[0] = REG_STR_STOP;
  commands[1] = REG_STR_PAUSE_INTERNAL;
  status = Steering_initialize("simple v.1.0", numCommands, commands);

  if(status != REG_SUCCESS){
    return 1;
  }

  /* Use library utility routines to allocate arrays of strings
     for passing in to Steering_control */
  changed_param_labels = Alloc_string_array(REG_MAX_STRING_LENGTH,
					    REG_MAX_NUM_STR_PARAMS);
  recvd_cmd_params = Alloc_string_array(REG_MAX_STRING_LENGTH,
					REG_MAX_NUM_STR_CMDS);

  if(!changed_param_labels || !recvd_cmd_params){
    printf("Failed to allocate string arrays :-(\n");
    return 1;
  }

  /* Register a steerable parameter */
  status = Register_param("TEMP", REG_TRUE, (void *)(&temp),
			  REG_FLOAT, "", "");
  if(status != REG_SUCCESS){
    printf("Failed to register parameter\n");
  }

  /* Enter main 'simulation' loop */

  for(i=0; i<nloops; i++){

    /* This is where the code would do some physics... */
    sleep(1);
    /* ...but we just pretend in this case */

    printf("\ni = %d\n", i);

    /* Talk to the steering client (if one is connected) */
    status = Steering_control(i,
			      &num_params_changed,
			      changed_param_labels,
			      &num_recvd_cmds,
			      recvd_cmds,
			      recvd_cmd_params);

    if(status == REG_SUCCESS){

      /* Print out the current value of steered parameter */
      printf("temp  = %f\n", temp);

      if(num_recvd_cmds > 0){
  
    	printf("Received %d steerer cmds\n", num_recvd_cmds);
  
    	for(icmd=0; icmd<num_recvd_cmds; icmd++){
  
 	  switch (recvd_cmds[icmd]){

 	  case REG_STR_STOP:
    	    finished = REG_TRUE;
 	    break;

 	  default:

	    /* Deal with user-defined IO types etc. if any */
 	    break;
 	  }

	  /* Break out if steerer told us to stop */  
 	  if(finished)break; /* out of loop over recv'd commands */
    	}
	if(finished)break; /* out of main sim. loop */
      }
    }
    else{
      printf("Call to Steering_control failed\n");
    }
  } /* End of main loop */

  /* Clean up the steering library */
  Steering_finalize();

  return 0;
}
