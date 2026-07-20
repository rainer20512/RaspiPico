/**
  ******************************************************************************
  * @file    com.h 
  * @author  Rainer
  * @brief   Debug Input handling and command interpreter 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CMDLINE_H
#define __CMDLINE_H

#include "config/config.h"

#include <string.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* Public defines -----------------------------------------------------------*/
#define MAX_WORDS     10  /* max number of cmdline elements we can handle    */

/* Public vars -----------------------------------------------------------*/
/*
 * total number of words and actual word in command line
 */
extern uint16_t word_num, act_word; 

/* Public types---- ---------------------------------------------------------*/
typedef struct InterpreterModule InterpreterModuleT;

/*
 * Function type to return a submenus prompt
 */
typedef const char * ( *pFnGetPrompt) (void);

/*
 * Function type to execute a command, to handle input and to print prompt
 */
typedef bool ( *pFnInterpreterExec )  ( char *cmd, size_t len, const void * arg );
typedef void (*pFnLineHandler)        ( char *cmdline, size_t len );
typedef void (*pFnPrompt)             ( void );

/*
 * Enumeration for different types of menu entries
 */
typedef enum {
  ctype_fn  = 0,            // Normal call of execution function
  ctype_sub = 1,            // push a new submenu
} CEnumT;

/*
 * execution type according to cType
 */
typedef union {
  pFnInterpreterExec fn;          // Normal function for cType == ctype_fn
  const InterpreterModuleT *sub;  // submenu for cType = ctype_sub
} CExecT;
    
typedef struct CommandSet {
  const char * const command;
  const CEnumT cType;
  const CExecT exec;
  const void * arg;
  const char * const helptext;
} CommandSetT;

typedef struct InterpreterModule {
  const pFnGetPrompt       prompt; 
  const CommandSetT * const commandlist;
  const uint32_t           num_cmd;
} InterpreterModuleT;

void      CMD_word_to_uc ( char *word, size_t wordlen );
bool      CMD_is_helpword( char *word, size_t wordlen );
bool      CMD_is_numeric ( char *word, size_t wordlen );
bool      CMD_parse_line ( char *cmdline, size_t len );
uint32_t  CMD_compare_words( const char * word1, size_t word1len,  const char * word2, size_t word2len );

bool CMD_Push ( const InterpreterModuleT * mdlNew );
void CMD_Pop ( void );
bool CMD_Handler_Push (pFnLineHandler linehnd, pFnPrompt prompt);
void CMD_Handler_Pop ( void );

bool CMD_Init ( void );
bool CMD_get_one_word ( char **start, size_t *len );
uint16_t CMD_argc ( void );
int32_t CMD_to_number ( char *word, size_t wordlen );

void task_handle_com(uint32_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CMDLINE_H */
