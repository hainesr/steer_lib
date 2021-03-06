/*
  The RealityGrid Steering Library

  Copyright (c) 2002-2010, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  Author: Andrew Porter
          Robert Haines
 */

/** @file ReG_Steer_Common.h
    @brief Header file for routines used in both Appside and Steerside

    Header file containing prototypes and datatype definitions for
    entities that are common to both the application- and steerer-side
    of the RealityGrid steering library.

    @author Andrew Porter
    @author Robert Haines
  */

#ifndef __REG_STEER_COMMON_H__
#define __REG_STEER_COMMON_H__

#include "ReG_Steer_types.h"

#ifdef __cplusplus
  #define PREFIX "C"
#else
  #define PREFIX
#endif

/** @internal Basic library configuration */
typedef struct {
  /** Markers to test for the implementation of transport code */
  char Samples_transport_string[REG_MAX_STRING_LENGTH];
  char Steering_transport_string[REG_MAX_STRING_LENGTH];

  /** Absolute path of directory we are executing in */
  char working_dir[REG_MAX_STRING_LENGTH];

  /** Where to write any temporary files such as logs */
  char scratch_dir[REG_MAX_STRING_LENGTH];

  /** Buffer used for string handling etc - size set in ReG_steer_types.h */
  char scratch_buffer[REG_SCRATCH_BUFFER_SIZE];

#if REG_DYNAMIC_MOD_LOADING
  void* samples_mod_handle;
  void* steering_mod_handle;
#endif
} Steer_lib_config_type;

/** @internal Used to log parameter values */
typedef struct {
  /** Handle of logged parameter */
  int  handle;
  /** Value of logged parameter */
  char value[REG_MAX_STRING_LENGTH];

} Param_log_entry_type;

/** @internal Used to log steering commands */
typedef struct {
  /** ID of the command */
  int   id;
  /** Any parameters associated with the command */
  char  params[REG_MAX_STRING_LENGTH];

} Cmd_log_entry_type;

/** @internal Used to log steering activity (both commands and
    parameter changes) */
typedef struct {

  /** No. of log entries for parameter updates */
  int   num_params;
  /** Array holding the log entries for parameter updates */
  Param_log_entry_type param[REG_MAX_NUM_STR_PARAMS];
  /** No. of other logged steering commands */
  int   num_cmds;
  /** Array holding logged steering commands */
  Cmd_log_entry_type cmd[REG_MAX_NUM_STR_CMDS];

} Steer_log_type;

/** @internal Used to hold information on a single checkpoint */
typedef struct {
  /** Key (UID) of this checkpoint */
  int                  key;
  /** Handle of the ChkType of this checkpoint */
  int                  chk_handle;
  /** Tag given this checkpoint */
  char                 chk_tag[REG_MAX_STRING_LENGTH];
  /** No. of parameter values logged with this checkpoint */
  int                  num_param;
  /** Array of logged parameters */
  Param_log_entry_type param[REG_MAX_NUM_STR_PARAMS];
  /** Whether or not (REG_TRUE or REG_FALSE) this entry has been
      sent to the attached steering client(s) */
  int                  sent_to_steerer;

} Chk_log_entry_type;

/** @internal Logs can be for checkpoints or parameter values */
typedef enum {PARAM, CHKPT} log_type_type ;

/** @internal
    The checkpoint log table */
typedef struct {

  /** What sort of log this is */
  log_type_type       log_type;
  /** Current no. of entries in table */
  int                 num_entries;
  /** Max. no. of entries table can currently hold (is dynamic) */
  int                 max_entries;
  /** No. of entries we have that haven't been sent to the steerer */
  int                 num_unsent;
  /** Next primary-key value */
  int                 primary_key_value;
  /** Array of entries */
  Chk_log_entry_type *entry;
  /** Ptr to file containing previous log entries */
  FILE               *file_ptr;
  /** Name of this file */
  char                filename[REG_MAX_STRING_LENGTH];
  /** Flag to indicate whether or not all entries must be sent to
      steerer, IRRESPECTIVE of the value of their individual
      sent_to_steerer flags.  This used when a client detaches and
      another one attaches some time later. */
  int                 send_all;
  /** Array of flags indicating whether or not the log for each
      registered parameter has been sent to the steering client */
  int                 param_send_all[REG_MAX_NUM_STR_PARAMS];
  /** Flag to indicate whether a send of the log data (read in on
      a previous occasion) is still in progress.  This feature
      prevents a deluge of log messages being emitted.
      @b OBSOLETE now? */
  int                 emit_in_progress;
  /** Count of how many log messages we've sent in this current call
      of Steering_control. If this reaches REG_MAX_NUM_LOG_MSG then
      we set the emit_in_progress flag and continue next time. */
  int                 num_sent;
  /** Ptr to buffer holding contents of log buffer for log
      emits spread over several calls to Emit_log */
  char               *file_content;
  /** Ptr to buffer containing logged steering cmds */
  char               *pSteer_cmds;
  /** Ptr to next free space in this buffer */
  char               *pSteer_cmds_slot;
  /** No. of bytes currently allocated to buffer */
  int                 steer_cmds_bytes;

} Chk_log_type;


/** @internal
    Holds information on a single parameter */
typedef struct {
  /** Label of the parameter, as supplied at its registration */
  char  label[REG_MAX_STRING_LENGTH];
  /** Whether (REG_TRUE) or not (REG_FALSE) param is steerable */
  int   steerable;
  /** Type of parameter (according to types in ReG_Steer_types.h */
  int   type;
  /** Handle of parameter (as assigned to it by the library) */
  int   handle;
  /** Pointer to the location of the program variable that this
      parameter represents */
  void *ptr;
  /** Only for parameters of type REG_BIN; pointer to start of
      buffer containing the raw data */
  void *ptr_raw;
  /** Only for parameters of type REG_BIN; size of buffer containing
      raw data */
  unsigned int raw_buf_size;
  /** Most recent value of the parameter obtained from @p ptr */
  char  value[REG_MAX_STRING_LENGTH];
  /** Whether param has been modified by steerer (steering side only) */
  int   modified;
  /** Whether param is for internal use by the library */
  int   is_internal;
  /** Whether param has a valid minimum value */
  int   min_val_valid;
  /** The minimum allowed value of this parameter (if any) */
  char  min_val[REG_MAX_STRING_LENGTH];
  /** Whether param has a valid maximum value */
  int   max_val_valid;
  /** The maximum allowed value of this parameter (if any) */
  char  max_val[REG_MAX_STRING_LENGTH];
  /** Pointer to array containing logged values (cast to double) */
  double* log;
  /** Current position in @p log */
  int     log_index;
  /** Current size of array pointed to by @p log */
  int     log_size;
  /** Whether logging is on for this parameter */
  int     logging_on;

} param_entry;

/** @internal
    Holds information on all registered parameters */
typedef struct {
  /** How many parameters have been registered */
  int          num_registered;
  /** The maximum no. of entries we can currently hold */
  int          max_entries;
  /** Value of the handle to assign to the next parameter to
      be registered */
  int          next_handle;
  /** Whether or not to log @e all parameter values */
  int          log_all;
  /** Array of parameter descriptions */
  param_entry *param;

} Param_table_type;

/** @internal
    Information on a single supported command */
typedef struct {
  /** The ID of this command */
  int               cmd_id;
  /** Params associated with this command */
  Param_table_type  cmd_params;

}supp_cmd_entry;

/** @internal
    Typedef for table holding information on all supported commands */
typedef struct {
  /** No. of registered supported commands */
  int             num_registered;
  /** The maximum no. of entries we can currently store */
  int             max_entries;
  /** Array of registered supported commands */
  supp_cmd_entry *cmd;

} Supp_cmd_table_type;

/** @internal
    Type definition for variable describing block distribution of
    slices being emitted */
typedef struct {

  /** Whether array data is in F90 ordering */
  int is_f90;
  /** Extent of whole array */
  int totx, toty, totz;
  /** Extent of sub-array */
  int nx, ny, nz;
  /** Origin of sub-array within whole */
  int sx, sy, sz;

} Array_type;

/** @internal
    Description of a single IOType */
typedef struct {
  /** Label of IOType (supplied by user) */
  char				label[REG_MAX_STRING_LENGTH];
  /** Handle of IOType - generated by us & returned to user */
  int				handle;
  /** Whether input (REG_IO_IN), output (REG_IO_OUT) */
  int				direction;
  /** Variable holding the interval with which emission/consumption is
      to occur (@e i.e. every @p internal steps) */
  int                           frequency;
  /** Handle of the (steerable) @p frequency in the parameter table */
  int				freq_param_handle;
  /** Pointer to buffer to hold data */
  void			       *buffer;
  /** How much data there is currently in @p buffer */
  int                           buffer_bytes;
  /** Size of the @p buffer */
  int				buffer_max_bytes;
  /** Whether or not to encode non-ASCII data as XDR (set in Emit_start) */
  int				use_xdr;
  /** How many bytes of xdr data to read (to avoid having to work-out/guess
      how many it will be from the type) */
  int                           num_xdr_bytes;
  /** Details on incoming array (if available) */
  Array_type                    array;
  /** Whether or not (1 or 0) we'll need to convert the ordering of
      the array */
  int                           convert_array_order;
  /** Whether IOType is enabled or not (for sockets - whether socket
     has been created) */
  int                           is_enabled;
  /** Index of the input channel - used in mapping to the details
      held by the SGS/SWS (when steering via SOAP) */
  int                           input_index;
  /** Whether or not this IOType (direction REG_IO_OUT) cares about getting
      an acknowledgement before trying to emit the next sample */
  int                           use_ack;
  /** Whether or not we need to check for an acknowledgement before
     attempting to emit the next data set. Setting @p use_ack to REG_FALSE
     OVERRIDES this flag. */
  int                           ack_needed;
  /** Whether (REG_TRUE) or not (REG_FALSE) we are in the process of
      consuming data.  For use with ioProxy in event of unexpected
      shut down */
  int                           consuming;
  /** For use with IOProxy - specifies label by which proxy knows the data
      that we want to read - for REG_IO_IN channels only */
  char                          proxySourceLabel[REG_MAX_STRING_LENGTH];

} IOdef_entry;

/** @internal Type for table holding all IOTypes */
typedef struct {

  /** The number of IOTypes that have been registered */
  int          num_registered;
  /** The max. number of IOTypes we can currently store */
  int          max_entries;
  /** The value of the handle to assign to the next IOType to be
      registered */
  int          next_handle;
  /** Whether or not to create the socket (if using them) when an
      IOType is registered */
  int          enable_on_registration;
  /** Count of @e input channels registered - used to
      map to details on data sources held by SGS/SWS */
  int          num_inputs;
  /** Array of registered IOTypes */
  IOdef_entry *io_def;

}IOdef_table_type;

/** Holds truncated details of a single iotype */
struct iotype_detail {
  /** The label given to this IOType at registration */
  char label[REG_MAX_STRING_LENGTH];
  /** The direction of this IOType (REG_IN or REG_OUT) */
  int  direction;
};

/** Holds truncated details of the IOTypes of a steerable app */
struct reg_iotype_list {
  /** Number of entries in this list */
  int numEntries;
  /** Array of entries */
  struct iotype_detail *iotype;
};

/** @internal
    Holds details required for secure access to a service
    using either SSL or WSSE */
struct reg_security_info {
  /** Whether or not this structure contains info. for use with
      ssl (REG_TRUE) or WSSE (REG_FALSE) */
  int  use_ssl;
  /** Path to directory containing CA certificates (ssl) */
  char caCertsPath[REG_MAX_STRING_LENGTH];
  /** Full path to pem file containing user's key and certificate
      @e concatenated together (ssl) */
  char myKeyCertFile[REG_MAX_STRING_LENGTH];
  /** The user's DN or other username (ssl or WSSE) */
  char userDN[REG_MAX_STRING_LENGTH];
  /** Passphrase (ssl - for key, wsse - for service) */
  char passphrase[REG_MAX_STRING_LENGTH];
};

/*-------- Function prototypes --------*/

/** @internal
    @param directory Full path to directory to check

    Checks whether the specified directory is valid and that we
    can write to it */
extern PREFIX int Directory_valid(char *directory);

/** @internal
    Routine to get (& check validity of) steering scratch directory
    from REG_STEER_DIRECTORY env. variable.  Used even when steering
    is NOT file based (because some log files @e etc. are stored in that
    directory too). */
extern PREFIX int Get_scratch_directory();

/** @internal
    @param table Pointer to table of registered parameters

    Looks up the next free index in the parameter table (might not
    be at the end because parameters can, in theory, be deleted). */
extern PREFIX int Next_free_param_index(Param_table_type *table);

/** @internal
    @param table Pointer to table of registered parameters
    @param ParamHandle Handle of parameter to get index for
    @return The index of the parameter in the table or
    REG_PARAM_HANDLE_NOTSET if no matching handle found.

    A look-up function - return the index of the specified parameter */
extern PREFIX int Param_index_from_handle(Param_table_type *table,
					  int ParamHandle);

/** @internal
    @param param Pointer to parameter entry to initialize

    Initializes the supplied parameter entry. */
extern PREFIX void Init_param_entry(param_entry *param);

/** @internal
    @param table Pointer to table of registered IOTypes
    @param IOdefHandle Handle of IOType to search for
    @return Index of the IOType in the table or
    REG_IODEF_HANDLE_NOTSET if no matching handle found

    A look-up function - return the index of the IOdef with handle
    IOdefHandle in the table pointed to by @p table. */
extern PREFIX int IOdef_index_from_handle(IOdef_table_type *table,
					  int IOdefHandle);

/** @internal
    @param table Pointer to table of registered parameters
    @return REG_SUCCESS, REG_FAILURE

    Increment the count of parameters registered and allocate
    more memory if required. */
extern PREFIX int Increment_param_registered(Param_table_type *table);

/** @internal
    @param table Pointer to table of supported commands

    Increment the count of commands registered and allocate
    more memory if required. */
extern PREFIX int Increment_cmd_registered(Supp_cmd_table_type *table);

/** @internal
    @param table Pointer to table of IOTypes

    Increment the count of IOTypes registered and allocate
    more memory if required. */
extern PREFIX int Increment_iodef_registered(IOdef_table_type *table);

/** @internal
    @param log Pointer to table of logs

    Increment the count of log entries and allocate
    more memory if required. */
extern PREFIX int Increment_log_entry(Chk_log_type *log);

/** @internal
    @param name Name of message
    @return Corresponding ENUM value

    Enquiry function - converts from XML tag name to ENUM value
    as defined in ReG_Steer_types.h */
extern PREFIX int Get_message_type(const char *name);

/** @internal
    @param pchar Pointer to buffer in which to put header
    @param bytes_free How long the supplied buffer is. This is
    modified with the number of bytes_free after the header
    has been written.

    Write ReG-specific XML header information into supplied message
    buffer  */
extern PREFIX int Write_xml_header(char** pchar, int* bytes_free);

/** @internal
    @param pchar Pointer to buffer in which to put footer
    @param bytes_free Amount of free space in supplied buffer

    Write ReG-specific XML footer information into supplied message
    buffer */
extern PREFIX int Write_xml_footer(char **pchar,
				   int bytes_free);

/** @internal
    @param filename Name of file to read
    @param buf Buffer containing contents of file
    @param size Size of @p buf (because it is malloc'd by this routine)
    @param retain_newlines If REG_TRUE then routine retains any newline
    ('\n') characters

    Read the specified ASCII file and return the contents in the
    buffer pointed to by @p buf.  It is the caller's responsibility to
    free() the memory pointed to by @p buf. */
extern PREFIX int Read_file(const char *filename,
			    char **buf, int *size,
			    const int retain_newlines);

/** @internal
    @param now On return, the current time

    Return the time since the epoch in seconds */
extern PREFIX int Get_current_time_seconds(double *now);

/** @internal
    @param io Pointer to entry describing IOType
    @param type Type of data
    @param count No. of data elements
    @param pData Pointer to data to re-order

    Intended to take array of data and re-order (from F90 to C or
    vice versa) and decode it (from XDR).  @b NOT @b USED. */
extern PREFIX int Reorder_decode_array(IOdef_entry *io,
				       int          type,
				       int          count,
				       void        *pData);

/** @internal
    Return the current (GMT) date and time as a string in the format
    YYYY-MM-DDTHH:MM:SSZ suitable for inclusion in XML documents */
extern PREFIX char *Get_current_time_string();

/** @internal
    Resets all entries in the struct
    @param sec Pointer to the reg_security_info structure to reset */
extern PREFIX void Wipe_security_info(struct reg_security_info *sec);

/**
    @param configFile Location of RealityGrid security config file or NULL/emptry string to use default of ~/.realitygrid/security.conf.
    @param sec Pointer to reg_security_info struct to populate

    Reads the specified RealityGrid security configuration file to get
    location of the PEM file containing BOTH the user's key and certificate
    and the path to the directory containing CA certificates. Parses
    user's certificate to get their DN.
    The security config. file is of the form: @n
    <tt>
    @verbatim
    <?xml version="1.0"?>
    <Security_config>
      <caCertsPath value="/etc/grid-security/certificates"/>
      <privateKeyCertFile value="/home/me/.globus/mycertnkey.pem"/>
    </Security_config>
    @endverbatim
    </tt>
 */
extern PREFIX int Get_security_config(const char               *configFile,
				      struct reg_security_info *sec);
/** @internal
    Prints out details of the specified signal - called by both the application-
    and steerer-side parts of the library
    @param aSignal The signal that has been caught */
void Common_signal_handler(int aSignal);

/** @internal
    Removes any trailing white space from the supplied character array
    @param pChar Pointer to character array to trim
    @return pointer to trimmed string, NULL otherwise. */
char *trimWhiteSpace(char *pChar);

/**
   @param list Pointer to reg_iotype_list to clean up

   Frees memory associated with the reg_iotype_list and resets
   member variables.
*/
int Delete_iotype_list(struct reg_iotype_list *list);

/** @internal
    @param dir The directory to search
    @param num_tags Number of tags to search for
    @param tags Elements of the filenames that we are searching for
    @param num_files Number of filenames being returned
    @param filenames Array of char* holding names of files found

    Searches for files in directory @p dir matching the tags specified.
    If any are found, @p filenames is malloc'd to point to an array of
    char* and each entry in this array is malloc'd and set to the
    relevant filename. These ptrs MUST be free'd. @p num_files can be
    zero even if the routine returns REG_SUCCESS. */
int Get_file_list(const char* dir, int num_tags, char** tags,
		  int* num_files, char*** filenames);

/** @internal
    @param str1 The first string to be compared
    @param str2 The second string to be compared

    A string comparison function for use with qsort
    to sort the filenames returned by Get_file_list */
int cmpstrs(const void* str1, const void* str2);

#endif
