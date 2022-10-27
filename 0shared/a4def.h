/*--------------------------------------------------------------------*/
/* a4def.h                                                            */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef A4DEF_INCLUDED
#define A4DEF_INCLUDED

/* Return statuses */
enum { SUCCESS,
       INITIALIZATION_ERROR,
       ALREADY_IN_TREE,
       NO_SUCH_PATH, CONFLICTING_PATH, BAD_PATH,
       NOT_A_DIRECTORY, NOT_A_FILE,
       MEMORY_ERROR
};

/* In lieu of a proper boolean datatype */
enum bool { FALSE, TRUE };
/* Make enumeration "feel" more like a builtin type */
typedef enum bool boolean;

#endif
