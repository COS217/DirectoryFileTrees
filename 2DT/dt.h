/*--------------------------------------------------------------------*/
/* dt.h                                                               */
/* Author: Christopher Moretti and Vikash Modi '23                    */
/*--------------------------------------------------------------------*/

#ifndef DT_INCLUDED
#define DT_INCLUDED

#include <stddef.h>
#include "a4def.h"

/*
  A Directory Tree is a representation of a hierarchy of directories.
*/

/*
   Inserts a new directory into the DT with absolute path pcPath.
   Returns SUCCESS if the new directory is inserted successfully.
   Otherwise, returns:
   * INITIALIZATION_ERROR if the DT is not in an initialized state
   * BAD_PATH if pcPath does not represent a well-formatted path
   * CONFLICTING_PATH if the root exists but is not a prefix of pcPath
   * ALREADY_IN_TREE if pcPath is already in the DT
   * MEMORY_ERROR if memory could not be allocated to complete request
*/
int DT_insert(const char *pcPath);

/*
  Returns TRUE if the DT contains a directory with absolute path
  pcPath and FALSE if not or if there is an error while checking.
*/
boolean DT_contains(const char *pcPath);


/*
  Removes the DT hierarchy (subtree) from the directory with absolute
  path path pcPath. Returns SUCCESS if found and removed.
  Otherwise, returns:
  * INITIALIZATION_ERROR if the DT is not in an initialized state
  * BAD_PATH if pcPath does not represent a well-formatted path
  * CONFLICTING_PATH if the root exists but is not a prefix of pcPath
  * NO_SUCH_PATH if absolute path pcPath does not exist in the DT
  * MEMORY_ERROR if memory could not be allocated to complete request
*/
int DT_rm(const char *pcPath);

/*
  Sets the DT data structure to an initialized state.
  The data structure is initially empty.
  Returns INITIALIZATION_ERROR if already initialized,
  and SUCCESS otherwise.
*/
int DT_init(void);

/*
  Removes all contents of the data structure and
  returns it to an uninitialized state.
  Returns INITIALIZATION_ERROR if not already initialized,
  and SUCCESS otherwise.
*/
int DT_destroy(void);

/*
  Returns a string representation of the
  data structure, or NULL if the structure is
  not initialized or there is an allocation error.

  The representation is depth-first, with nodes
  at any given level ordered lexicographically.

  Allocates memory for the returned string,
  which is then owned by client!
*/
char *DT_toString(void);

#endif
