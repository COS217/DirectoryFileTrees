/*--------------------------------------------------------------------*/
/* dt_client.c                                                        */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dt.h"

/* Tests the DT implementation with an assortment of checks.
   Prints the status of the data structure along the way to stderr.
   Returns 0. */
int main(void) {
  char* temp;

  /* Before the data structure is initialized:
     * insert, rm, and destroy should each return INITIALIZATION_ERROR
     * contains should return FALSE
     * toString should return NULL
  */
  assert(DT_insert("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert(DT_contains("1root/2child/3grandchild") == FALSE);
  assert(DT_rm("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert((temp = DT_toString()) == NULL);
  assert(DT_destroy() == INITIALIZATION_ERROR);

  /* After initialization, the data structure is empty, so
     contains should still return FALSE for any non-NULL string,
     and toString should return the empty string.
  */
  assert(DT_init() == SUCCESS);
  assert(DT_contains("") == FALSE);
  assert(DT_contains("1root") == FALSE);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* A valid path must not:
     * be the empty string
     * start with a '/'
     * end with a '/'
     * have consecutive '/' delimiters.
  */
  assert(DT_insert("") == BAD_PATH);
  assert(DT_insert("/1root/2child") == BAD_PATH);
  assert(DT_insert("1root/2child/") == BAD_PATH);
  assert(DT_insert("1root//2child") == BAD_PATH);

  /* After insertion, the data structure should contain every prefix
     of the inserted path, toString should return a string with these
     prefixes, trying to insert it again should return
     ALREADY_IN_TREE, and trying to insert some other root should
     return CONFLICTING_PATH.
  */
  assert(DT_insert("1root") == SUCCESS);
  assert(DT_insert("1root/2child/3grandchild") == SUCCESS);
  assert(DT_contains("1root") == TRUE);
  assert(DT_contains("1root/2child") == TRUE);
  assert(DT_contains("1root/2child/3grandchild") == TRUE);
  assert(DT_contains("anotherRoot") == FALSE);
  assert(DT_insert("anotherRoot") == CONFLICTING_PATH);
  assert(DT_contains("anotherRoot") == FALSE);
  assert(DT_contains("1root/2second") == FALSE);
  assert(DT_insert("1root/2child/3grandchild") == ALREADY_IN_TREE);
  assert(DT_insert("anotherRoot/2nope/3noteven") == CONFLICTING_PATH);

  /* Trying to insert a third child should succeed, unlike in BDT */
  assert(DT_insert("1root/2second") == SUCCESS);
  assert(DT_insert("1root/2third") == SUCCESS);
  assert(DT_insert("1root/2ok/3yes/4indeed") == SUCCESS);
  assert(DT_contains("1root") == TRUE);
  assert(DT_contains("1root/2child") == TRUE);
  assert(DT_contains("1root/2second") == TRUE);
  assert(DT_contains("1root/2third") == TRUE);
  assert(DT_contains("1root/2ok") == TRUE);
  assert(DT_contains("1root/2ok/3yes") == TRUE);
  assert(DT_contains("1root/2ok/3yes/4indeed") == TRUE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 1:\n%s\n", temp);
  free(temp);

  /* Children of any path must be unique, but individual directories
     in different paths needn't be
  */
  assert(DT_insert("1root/2child/3grandchild") == ALREADY_IN_TREE);
  assert(DT_contains("1root/2second/3grandchild") == FALSE);
  assert(DT_insert("1root/2second/3grandchild") == SUCCESS);
  assert(DT_contains("1root/2child/3grandchild") == TRUE);
  assert(DT_contains("1root/2second/3grandchild") == TRUE);
  assert(DT_insert("1root/2second/3grandchild") == ALREADY_IN_TREE);
  assert(DT_insert("1root/2second/3grandchild/1root") == SUCCESS);
  assert(DT_contains("1root/2second/3grandchild/1root") == TRUE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 2:\n%s\n", temp);
  free(temp);

  /* calling rm on a path that doesn't exist should return
     NO_SUCH_PATH, but on a path that does exist should return
     SUCCESS and remove entire subtree rooted at that path
  */
  assert(DT_contains("1root/2second/3grandchild/1root") == TRUE);
  assert(DT_contains("1root/2second/3second") == FALSE);
  assert(DT_rm("1root/2second/3second") == NO_SUCH_PATH);
  assert(DT_contains("1root/2second/3second") == FALSE);
  assert(DT_rm("1root/2second") == SUCCESS);
  assert(DT_contains("1root") == TRUE);
  assert(DT_contains("1root/2child") == TRUE);
  assert(DT_contains("1root/2second") == FALSE);
  assert(DT_contains("1root/2second/3grandchild") == FALSE);
  assert(DT_contains("1root/2second/3grandchild/1root") == FALSE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 3:\n%s\n", temp);
  free(temp);

  /* removing the root doesn't uninitialize the structure */
  assert(DT_rm("1anotherroot") == CONFLICTING_PATH);
  assert(DT_rm("1root") == SUCCESS);
  assert(DT_contains("1root/2child") == FALSE);
  assert(DT_contains("1root") == FALSE);
  assert(DT_rm("1root") == NO_SUCH_PATH);
  assert(DT_rm("1anotherroot") == NO_SUCH_PATH);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* children should be printed in lexicographic order, depth first */

  /* Debugging: you may want to add this line before any failing
     assert(!strcmp(...)) line in the code below:
     fprintf(stderr, "Checkpoint Promotion:\n%s\n", temp);
  */
  assert(DT_insert("a/y") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/y\n"));
  free(temp);
  assert(DT_insert("a/x") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\na/y\n"));
  free(temp);
  assert(DT_rm("a/y") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\n"));
  free(temp);
  assert(DT_insert("a/y2") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\na/y2\n"));
  free(temp);
  assert(DT_insert("a/y2/GRAND1") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\na/y2\na/y2/GRAND1\n"));
  free(temp);
  assert(DT_insert("a/y/Grand0") == SUCCESS);
  assert(DT_insert("a/y/Grand2") == SUCCESS);
  assert(DT_insert("a/y/Grand1/Great_Grand") == SUCCESS);
  assert(DT_insert("a/x/Grandx/Great_GrandX") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 4:\n%s\n", temp);
  free(temp);

  assert(DT_destroy() == SUCCESS);
  assert(DT_destroy() == INITIALIZATION_ERROR);
  assert(DT_contains("a") == FALSE);
  assert((temp = DT_toString()) == NULL);

  return 0;
}
