/*--------------------------------------------------------------------*/
/* bdt.h                                                              */
/* Author: Christopher Moretti and Vikash Modi '23                    */
/*--------------------------------------------------------------------*/

#ifndef BDT_INCLUDED
#define BDT_INCLUDED

#include <stddef.h>
#include "a4def.h"

/*
  A Binary Directory Tree is a representation of a hierarchy of
  directories, in which each directory may have at most two children.
*/

/*
   Inserts a new directory into the BDT with absolute path pcPath.
   Returns SUCCESS if the new directory is inserted successfully.
   Otherwise, returns:
   * INITIALIZATION_ERROR if the BDT is not in an initialized state
   * BAD_PATH if pcPath does not represent a well-formatted path
   * CONFLICTING_PATH if the root exists but is not a prefix of pcPath
                      or if inserting the new directory would require
                      some ancestor to have a third child.
   * ALREADY_IN_TREE if pcPath is already in the BDT
   * MEMORY_ERROR if memory could not be allocated to complete request
*/
int BDT_insert(const char *pcPath);

/*
  Returns TRUE if the BDT contains a directory with absolute path
  pcPath and FALSE if not or if there is an error while checking.
*/
boolean BDT_contains(const char *pcPath);

/*
  Removes the BDT hierarchy (subtree) from the directory with absolute
  path path pcPath. Returns SUCCESS if found and removed.
  Otherwise, returns:
  * INITIALIZATION_ERROR if the BDT is not in an initialized state
  * BAD_PATH if pcPath does not represent a well-formatted path
  * CONFLICTING_PATH if the root exists but is not a prefix of pcPath
  * NO_SUCH_PATH if absolute path pcPath does not exist in the BDT
  * MEMORY_ERROR if memory could not be allocated to complete request
*/
int BDT_rm(const char *pcPath);

/*
  Sets the BDT data structure to an initialized state.
  The data structure is initially empty.
  Returns INITIALIZATION_ERROR if already initialized,
  and SUCCESS otherwise.
*/
int BDT_init(void);

/*
  Removes all contents of the data structure and
  returns it to an uninitialized state.
  Returns INITIALIZATION_ERROR if not already initialized,
  and SUCCESS otherwise.
*/
int BDT_destroy(void);

/*
  Returns a string representation of the
  data structure, or NULL if the structure is
  not initialized or there is an allocation error.

  The representation is depth-first, with nodes
  at any given level ordered first ("left") child 
  before second ("right") child.

  Allocates memory for the returned string,
  which is then owned by client!
*/
char *BDT_toString(void);

#endif
