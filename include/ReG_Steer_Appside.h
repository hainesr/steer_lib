/*----------------------------------------------------------------------------
    Header file defining public routines used in the construction
    of a steering interface for an application.

    (C)Copyright 2002 The University of Manchester, United Kingdom,
    all rights reserved.

    This software is produced by the Supercomputing, Visualization &
    e-Science Group, Manchester Computing, the Victoria University of
    Manchester as part of the RealityGrid project.

    This software has been tested with care but is not guaranteed for
    any particular purpose. Neither the authors, nor the University of
    Manchester offer any warranties or representations, nor do they
    accept any liabilities with respect to this software.

    This program must not be used for commmercial gain without the
    written permission of the authors.
    
    Supercomputing, Visualization & e-Science Group
    Manchester Computing
    University of Manchester
    Manchester M13 9PL

    email:  csar-advice@cfs.ac.uk.
    Tel:    +44 161 275 6824/5997
    Fax:    +44 161 275 6040    
    
    Date          Version    Updates                            Author
    ----          -------    -------                            ------
    23.7.2002       0.1                                         A Porter

---------------------------------------------------------------------------*/

#include "ReG_Steer_types.h"

#ifdef REG_STR_CPP
  #define PREFIX "C"
#else
  #define PREFIX 
#endif

/*-------------- Application-side function prototypes -------------*/

/* Set global flag to enable/disable steering.  This flag is checked
   at start of Steering_control and causes routine to simply return
   if not enabled.  Flag not used in other routines in order to 
   ensure that internal tables remain in a consistent state. */
extern PREFIX void Steering_enable(const int EnableSteer);

/* Initialise the internal library tables and advertise application
   as being steerable. */
extern PREFIX int Steering_initialize(int  NumSupportedCmds,
				      int *SupportedCmds);

/* Register the specified IO types.  This routine performs some sort of
   mapping between the supplied label and a physical destination for
   data (e.g. a filename and location), possibly using the component
   framework. Returns an integer handle for each IO type. */
extern PREFIX int Register_IOTypes(int    NumTypes,
				   char* *IOLabel,
				   int   *type,
				   int   *support_auto_io,
				   int  **IOFrequency,
				   int   *IOType);

/* Register the specified parameters. Since labels are used to 
   identify parameters, they must be unique and not contain
   any white space. */
extern PREFIX int Register_params(int    NumParams,
				  char* *ParamLabels,
				  int   *ParamSteerable,
				  void **ParamPtrs,
				  int   *ParamTypes);

/* Un-register the parameters identified by the given labels. */
extern PREFIX int Unregister_params(int    NumParams,
				    char* *ParamLabels);

/* Main steering routine to be called at application breakpoint.
   Takes <SeqNum> (an indication of application progress) as input
   and returns the labels of any parameters changed by the steerer
   as well as any commands that the app. must handle. This routine
   does not block - if there are no waiting messages from the steerer
   then it returns immediately. */
extern PREFIX int Steering_control(int     SeqNum,
				   int    *NumSteerParams,
				   char*  *SteerParamLabels,
				   int    *NumSteerCommands,
				   int    *SteerCommands);

/* Open the specified IOType (as returned by a call to Register_IOTypes)
   ready for output. <SeqNum> provides a measure of the application's
   progress at this point. */
extern PREFIX int Emit_start(int               IOType,
			     int               SeqNum,
			     REG_IOHandleType *IOHandle);

/* Open the specified IOType (as returned by a call to Register_IOTypes)
   ready for input. */
extern PREFIX int Consume_start(int               IOType,
				REG_IOHandleType *IOHandle);

/* Close the specified IOType and complete the emission process. */
extern PREFIX int Emit_stop(REG_IOHandleType *IOHandle);

/* Close the specified IOType and complete the consumption process. 
   Frees any memory used during the consumption. */
extern PREFIX int Consume_stop(REG_IOHandleType *IOHandle);

/* Must be called following a call to Emit_start.  Emits <Count> items
   of type <DataType> as pointed to by <pData>. */
extern PREFIX int Emit_data_slice(REG_IOHandleType  IOHandle,
				  int               DataType,
				  int               Count,
				  void             *pData);

/* Must be called following a call to Consume_start. Returns a ptr
   to <Count> items of type <DataType>. This pointer is temporary - the
   data should be copied from the memory to which it points. */
extern PREFIX int Consume_data_slice(REG_IOHandleType  IOHandle,
				     int              *DataType,
				     int              *Count,
				     void            **pData);

/* Called once all steering activity is complete.  Disconnects from
   steerer (if any), removes the 'I am steerable' advertisement and 
   cleans up internal tables. */
extern PREFIX int Steering_finalize();

/* Intended to be called by the application in response to a 'pause'
   command from the steerer.  Blocks until a 'resume' or 'stop' 
   command received.  Returns all commands that immediately followed
   a 'resume' command as well as the labels of any parameters edited
   in that particular message.  (Parameters can be edited while the
   application is paused because this routine continually calls
   Consume_control until it receives 'resume' or 'stop'.) */
extern PREFIX int Steering_pause(int   *NumSteerParams,
				 char **SteerParamLabels,
				 int   *NumCommands,
				 int   *SteerCommands);
