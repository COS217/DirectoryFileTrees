/*--------------------------------------------------------------------*/
/* path.c                                                             */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dynarray.h"
#include "path.h"

/* An absolute path */
struct path {
   /* The string representation of the path,
      which uses '/' as the component delimiter */
   const char *pcPath;
   /* The string length of pcPath */
   size_t ulLength;
   /* The ordered collection of component strings in the path */
   DynArray_T oDComponents;
};

/*
  Frees pcStr. This wrapper is used to match the requirements of the
  callback function pointer passed to DynArray_map. pvExtra is unused.
*/
static void Path_freeString(char *pcStr, void *pvExtra) {
   /* pcStr may be NULL, as this is a no-op to free.
      pvExtra may be NULL, as it is unused. */
   free(pcStr);
}

/*
  Sets *poDComponents to be an ordered collection of component strings
  in pcPath, or NULL if an error occurs.
  Returns one of the following statuses:
  * SUCCESS if no error occurrs
  * BAD_PATH if pcPath is the empty string,
             or begins or ends with a '/',
             or contains consecutive '/' delimiters
  * MEMORY_ERROR if memory could not be allocated to complete request
*/
static int Path_split(const char *pcPath, DynArray_T *poDComponents) {
   const char *pcStart = pcPath;
   const char *pcEnd = pcPath;
   char *pcCopy;
   DynArray_T oDSubstrings;

   assert(pcPath != NULL);
   assert(poDComponents != NULL);

   /* path cannot be empty string */
   if(*pcPath == '\0') {
      *poDComponents = NULL;
      return BAD_PATH;
   }

   oDSubstrings = DynArray_new(0);
   if(oDSubstrings == NULL) {
      *poDComponents = NULL;
      return MEMORY_ERROR;
   }

   /* validate and split pcPath */
   while(*pcEnd != '\0') {
      pcEnd = pcStart;
      /* component can't start with delimiter */
      if(*pcEnd == '/') {
         DynArray_map(oDSubstrings,
                      (void (*)(void*, void*)) Path_freeString, NULL);
         DynArray_free(oDSubstrings);
         *poDComponents = NULL;
         return BAD_PATH;
      }

      /* advance pcEnd to end of next token */
      while(*pcEnd != '/' && *pcEnd != '\0')
         pcEnd++;

      /* final component can't end with slash */
      if(*pcEnd == '\0' && *(pcEnd-1) == '/') {
         DynArray_map(oDSubstrings,
                      (void (*)(void*, void*)) Path_freeString, NULL);
         DynArray_free(oDSubstrings);
         *poDComponents = NULL;
         return BAD_PATH;
      }

      pcCopy = calloc((size_t)(pcEnd-pcStart+1), sizeof(char));
      if(pcCopy == NULL) {
         DynArray_map(oDSubstrings,
                      (void (*)(void*, void*)) Path_freeString, NULL);
         DynArray_free(oDSubstrings);
         *poDComponents = NULL;
         return MEMORY_ERROR;
      }

      if( DynArray_add(oDSubstrings, pcCopy) == 0) {
         DynArray_map(oDSubstrings,
                      (void (*)(void*, void*)) Path_freeString, NULL);
         DynArray_free(oDSubstrings);
         *poDComponents = NULL;
         return MEMORY_ERROR;
      }

      while(pcStart != pcEnd) {
         *pcCopy = *pcStart;
         pcCopy++;
         pcStart++;
      }

      pcStart++;
   }

   *poDComponents = oDSubstrings;
   return SUCCESS;
}


int Path_new(const char *pcPath, Path_T *poPResult) {
   struct path *psNew;
   int iSplitResult;

   assert(pcPath != NULL);
   assert(poPResult != NULL);

   psNew = calloc(1, sizeof(struct path));
   if(psNew == NULL) {
      *poPResult = NULL;
      return MEMORY_ERROR;
   }

   /* instantiate and fill list of components */
   iSplitResult = Path_split(pcPath, &psNew->oDComponents);
   if(iSplitResult != SUCCESS) {
      Path_free(psNew);
      *poPResult = NULL;
      return iSplitResult;
   }

   psNew->ulLength = strlen(pcPath);
   psNew->pcPath = malloc(psNew->ulLength+1);
   if(psNew->pcPath == NULL) {
      Path_free(psNew);
      *poPResult = NULL;
      return MEMORY_ERROR;
   }
   strcpy((char *)psNew->pcPath, pcPath);

   *poPResult = psNew;
   return SUCCESS;
}

int Path_prefix(Path_T oPPath, size_t ulDepth, Path_T *poPResult) {
   struct path *psNew;
   size_t ulIndex, ulLength, ulSum;
   const char *pcComponent;
   char *pcCopy;
   char *pcBuild;
   char *pcInsert;

   assert(oPPath != NULL);
   assert(poPResult != NULL);

   /* cannot build empty path */
   if(ulDepth == 0) {
      *poPResult = NULL;
      return NO_SUCH_PATH;
   }

   /* cannot have a prefix longer than oPPath */
   if(Path_getDepth(oPPath) < ulDepth) {
      *poPResult = NULL;
      return NO_SUCH_PATH;
   }

   psNew = calloc(1, sizeof(struct path));
   if(psNew == NULL) {
      *poPResult = NULL;
      return MEMORY_ERROR;
   }

   psNew->oDComponents = DynArray_new(ulDepth);
   if(psNew->oDComponents == NULL) {
      Path_free(psNew);
      *poPResult = NULL;
      return MEMORY_ERROR;
   }

   pcBuild = calloc(Path_getStrLength(oPPath)+1, sizeof(char));
   if(pcBuild == NULL) {
      Path_free(psNew);
      *poPResult = NULL;
      return MEMORY_ERROR;
   }

   pcInsert = pcBuild;
   ulSum = 0;

   for(ulIndex = 0; ulIndex < ulDepth; ulIndex++) {
      /* deep copy each component to new DynArray */
      pcComponent = Path_getComponent(oPPath, ulIndex);
      ulLength = strlen(pcComponent);
      pcCopy = calloc(ulLength + 1, sizeof(char));
      if(pcCopy == NULL) {
         free(pcBuild);
         Path_free(psNew);
         *poPResult = NULL;
         return MEMORY_ERROR;
      }
      strcpy(pcCopy, pcComponent);
      (void) DynArray_set(psNew->oDComponents, ulIndex, pcCopy);
      /* construct prefix's pathname string */
      strcpy(pcInsert, pcComponent);
      pcInsert[ulLength] = '/';
      ulSum += ulLength + 1;
      pcInsert += ulLength + 1;
   }
   pcBuild[ulSum-1] = '\0';

   /* shrink allocation to fit prefix's pathname string if needed */
   pcInsert = realloc(pcBuild, ulSum);
   if(pcInsert == NULL) {
      free(pcBuild);
      Path_free(psNew);
      *poPResult = NULL;
      return MEMORY_ERROR;
   }
   psNew->ulLength = ulSum-1;
   psNew->pcPath = pcInsert;

   *poPResult = psNew;
   return SUCCESS;
}

int Path_dup(Path_T oPPath, Path_T *poPResult) {
   assert(oPPath != NULL);
   assert(poPResult != NULL);

   return Path_prefix(oPPath, Path_getDepth(oPPath), poPResult);
}

void Path_free(Path_T oPPath) {
   if(oPPath != NULL) {
      free((char *)oPPath->pcPath);

      if(oPPath->oDComponents != NULL) {
         DynArray_map(oPPath->oDComponents,
                      (void (*)(void*, void*)) Path_freeString, NULL);
         DynArray_free(oPPath->oDComponents);
      }
   }
   free((struct path*) oPPath);
}

const char *Path_getPathname(Path_T oPPath) {
   assert(oPPath != NULL);

   return oPPath->pcPath;
}

size_t Path_getStrLength(Path_T oPPath) {
   assert(oPPath != NULL);

   return oPPath->ulLength;
}

int Path_comparePath(Path_T oPPath1, Path_T oPPath2) {
   assert(oPPath1 != NULL);
   assert(oPPath2 != NULL);

   return strcmp(oPPath1->pcPath, oPPath2->pcPath);
}

int Path_compareString(Path_T oPPath, const char *pcStr) {
   assert(oPPath != NULL);
   assert(pcStr != NULL);

   return strcmp(oPPath->pcPath, pcStr);
}

size_t Path_getDepth(Path_T oPPath) {
   assert(oPPath != NULL);

   return DynArray_getLength(oPPath->oDComponents);
}

size_t Path_getSharedPrefixDepth(Path_T oPPath1, Path_T oPPath2) {
   size_t ulDepth1, ulDepth2, ulMin, i;

   assert(oPPath1 != NULL);
   assert(oPPath2 != NULL);

   ulDepth1 = Path_getDepth(oPPath1);
   ulDepth2 = Path_getDepth(oPPath2);
   if(ulDepth1 < ulDepth2)
      ulMin = ulDepth1;
   else
      ulMin = ulDepth2;
   for(i = 0; i < ulMin; i++) {
      if(strcmp(Path_getComponent(oPPath1, i),
                Path_getComponent(oPPath2, i)))
         return i;
   }
   return ulMin;
}

const char *Path_getComponent(Path_T oPPath, size_t ulLevel) {
   assert(oPPath != NULL);

   if(ulLevel >= Path_getDepth(oPPath))
      return NULL;

   return DynArray_get(oPPath->oDComponents, ulLevel);
}
