/*--------------------------------------------------------------------*/
/* bdt_client.c                                                       */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdt.h"

/* Tests the BDT implementation with an assortment of checks.
   Prints the status of the data structure along the way to stderr.
   Returns 0. */
int main(void) {
  char* temp;

  /* Before the data structure is initialized:
     * insert, rm, and destroy should each return INITIALIZATION_ERROR
     * contains should return FALSE
     * toString should return NULL
  */
  assert(BDT_insert("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert(BDT_contains("1root/2child/3grandchild") == FALSE);
  assert(BDT_rm("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert((temp = BDT_toString()) == NULL);
  assert(BDT_destroy() == INITIALIZATION_ERROR);

  /* After initialization, the data structure is empty, so
     contains should still return FALSE for any non-NULL string,
     and toString should return the empty string.
  */
  assert(BDT_init() == SUCCESS);
  assert(BDT_contains("") == FALSE);
  assert(BDT_contains("1root") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* A valid path must not:
     * be the empty string
     * start with a '/'
     * end with a '/'
     * have consecutive '/' delimiters.
  */
  assert(BDT_insert("") == BAD_PATH);
  assert(BDT_insert("/1root/2child") == BAD_PATH);
  assert(BDT_insert("1root/2child/") == BAD_PATH);
  assert(BDT_insert("1root//2child") == BAD_PATH);

  /* After insertion, the data structure should contain every prefix
     of the inserted path, toString should return a string with these
     prefixes, trying to insert it again should return
     ALREADY_IN_TREE, and trying to insert some other root should
     return CONFLICTING_PATH. */
  assert(BDT_insert("1root") == SUCCESS);
  assert(BDT_insert("1root/2child/3grandchild") == SUCCESS);
  assert(BDT_contains("1root") == TRUE);
  assert(BDT_contains("1root/2child") == TRUE);
  assert(BDT_contains("1root/2child/3grandchild") == TRUE);
  assert(BDT_contains("anotherRoot") == FALSE);
  assert(BDT_insert("anotherRoot") == CONFLICTING_PATH);
  assert(BDT_contains("anotherRoot") == FALSE);
  assert(BDT_contains("1root/2second") == FALSE);
  assert(BDT_insert("1root/2child/3grandchild") == ALREADY_IN_TREE);
  assert(BDT_insert("anotherRoot/2nope/3noteven") == CONFLICTING_PATH);

  /* Trying to insert a third child should fail with CONFLICTING_PATH */
  assert(BDT_insert("1root/2second") == SUCCESS);
  assert(BDT_insert("1root/2third") == CONFLICTING_PATH);
  assert(BDT_insert("1root/2no/3nay/4never") == CONFLICTING_PATH);
  assert(BDT_contains("1root") == TRUE);
  assert(BDT_contains("1root/2child") == TRUE);
  assert(BDT_contains("1root/2second") == TRUE);
  assert(BDT_contains("1root/2third") == FALSE);
  assert(BDT_contains("1root/2no") == FALSE);
  assert(BDT_contains("1root/2no/3nay") == FALSE);
  assert(BDT_contains("1root/2no/3nay/4never") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 1:\n%s\n", temp);
  free(temp);

  /* Children of any path must be unique, but individual directories
     in different paths needn't be */
  assert(BDT_insert("1root/2child/3grandchild") == ALREADY_IN_TREE);
  assert(BDT_contains("1root/2second/3grandchild") == FALSE);
  assert(BDT_insert("1root/2second/3grandchild") == SUCCESS);
  assert(BDT_contains("1root/2child/3grandchild") == TRUE);
  assert(BDT_contains("1root/2second/3grandchild") == TRUE);
  assert(BDT_insert("1root/2second/3grandchild") == ALREADY_IN_TREE);
  assert(BDT_insert("1root/2second/3grandchild/1root") == SUCCESS);
  assert(BDT_contains("1root/2second/3grandchild/1root") == TRUE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 2:\n%s\n", temp);
  free(temp);

  /* calling rm on a path that doesn't exist should return
     NO_SUCH_PATH, but on a path that does exist should return
     SUCCESS and remove entire subtree rooted at that path  */
  assert(BDT_contains("1root/2second/3grandchild/1root") == TRUE);
  assert(BDT_contains("1root/2second/3second") == FALSE);
  assert(BDT_rm("1root/2second/3second") == NO_SUCH_PATH);
  assert(BDT_contains("1root/2second/3second") == FALSE);
  assert(BDT_rm("1root/2second") == SUCCESS);
  assert(BDT_contains("1root") == TRUE);
  assert(BDT_contains("1root/2child") == TRUE);
  assert(BDT_contains("1root/2second") == FALSE);
  assert(BDT_contains("1root/2second/3grandchild") == FALSE);
  assert(BDT_contains("1root/2second/3grandchild/1root") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "Checkpoint 3:\n%s\n", temp);
  free(temp);

  /* removing the root doesn't uninitialize the structure */
  assert(BDT_rm("1anotherroot") == CONFLICTING_PATH);
  assert(BDT_rm("1root") == SUCCESS);
  assert(BDT_contains("1root/2child") == FALSE);
  assert(BDT_contains("1root") == FALSE);
  assert(BDT_rm("1root") == NO_SUCH_PATH);
  assert(BDT_rm("1anotherroot") == NO_SUCH_PATH);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* removing a first child should cause the second child to become
     the first child and remain so until it is itself removed. */

  /* Debugging: you may want to add this line before any failing
     assert(!strcmp(...)) line in the code below:
     fprintf(stderr, "Checkpoint Promotion:\n%s\n", temp);
  */
  assert(BDT_insert("a/y") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/y\n"));
  free(temp);
  assert(BDT_insert("a/x") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/y\na/x\n"));
  free(temp);
  assert(BDT_rm("a/y") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\n"));
  free(temp);
  assert(BDT_insert("a/y2") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\na/y2\n"));
  free(temp);
  assert(BDT_rm("a/y2") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\n"));
  free(temp);
  assert(BDT_insert("a/y3") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/x\na/y3\n"));
  free(temp);
  assert(BDT_rm("a/x") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,"a\na/y3\n"));
  free(temp);

  assert(BDT_destroy() == SUCCESS);
  assert(BDT_destroy() == INITIALIZATION_ERROR);
  assert(BDT_contains("a") == FALSE);
  assert((temp = BDT_toString()) == NULL);

  return 0;
}
