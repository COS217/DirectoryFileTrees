/*--------------------------------------------------------------------*/
/* path.h                                                             */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef PATH_INCLUDED
#define PATH_INCLUDED

#include <stddef.h>
#include "a4def.h"

/* An object representing an absolute path in a tree */
typedef const struct path * Path_T;

/*
  Creates a new path object representing the absolute path in pcPath.
  Returns an int SUCCESS status and sets *poPResult to be the new path
  if successful. Otherwise, sets *poPResult to NULL and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * BAD_PATH if the string argument is the empty string
             or begins with or ends with a '/'
             or contains consecutive '/' delimiters
*/
int Path_new(const char *pcPath, Path_T *poPResult);

/*
  Creates a "deep copy" of oPPath, duplicating all its contents.
  Returns an int SUCCESS status and sets *poPResult to be the new path
  if successful. Otherwise, sets *poPResult to NULL and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * NO_SUCH_PATH if oPPath's depth is 0
*/
int Path_dup(Path_T oPPath, Path_T *poPResult);

/*
  Creates a new path object representing a prefix (i.e., ancestor) of
  oPPath with depth ulDepth. In the case that ulDepth is the same as
  oPPath's depth, this is equivalent to Path_dup.
  Returns an int SUCCESS status and sets *poPResult to be the new path
  if successful. Otherwise, sets *poPResult to NULL and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * NO_SUCH_PATH if ulDepth is 0 or is greater than oPPath's depth
*/
int Path_prefix(Path_T oPPath, size_t ulDepth, Path_T *poPResult);

/* Destroys and frees all memory allocated for oPPath. */
void Path_free(Path_T oPPath);

/* Returns the string representation of the absolute path oPPath. */
const char *Path_getPathname(Path_T oPPath);

/*
  Returns the length (not including trailing '\0') of the string
  representation of the absolute path oPPath.
*/
size_t Path_getStrLength(Path_T oPPath);

/*
  Compares oPPath1 and oPPath2 lexicographically based on pathname.
  Returns <0, 0, or >0 if oPPath1 is "less than", "equal to", or
  "greater than" oPPath2, respectively.
*/
int Path_comparePath(Path_T oPPath1, Path_T oPPath2);

/*
  Compares oPPath's pathname with pcStr lexicographically.
  Returns <0, 0, or >0 if oPPath is "less than", "equal to", or
  "greater than" pcStr, respectively.
*/
int Path_compareString(Path_T oPPath, const char *pcStr);

/*
  Returns the number of separate levels (components) in oPPath.
  For example, the absolute path "someRoot" has depth 1, and
  "someRoot/aChild/aGrandChild/aGreatGrandChild" has depth 4.
*/
size_t Path_getDepth(Path_T oPPath);

/*
  Returns the length, in components, of the longest prefix shared by
  oPPath1 and oPPath2. For example the absolute paths
  "Charles/William/George" and "Charles/Harry/Archie" have a shared
  prefix depth of 1 (just Charles), whereas "Charles/William/George"
  and "Charles/William/Charlotte" have a shared prefix depth of 2.
*/
size_t Path_getSharedPrefixDepth(Path_T oPPath1, Path_T oPPath2);

/*
  Returns the string version of the component of oPPath at level
  ulLevel. This count is from 0, so with level 0 the root of oPPath
  would be returned.
  Returns NULL if ulLevel is greater than oPPath's maxium level.
*/
const char *Path_getComponent(Path_T oPPath, size_t ulLevel);

#endif
