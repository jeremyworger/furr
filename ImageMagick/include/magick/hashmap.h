/*
  ImageMagick hash methods.
*/
#ifndef _MAGICK_HASHMAP_H
#define _MAGICK_HASHMAP_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define SmallHashmapSize  17
#define MediumHashmapSize  701
#define LargeHashmapSize  8191

typedef struct _HashmapInfo
  HashmapInfo;

typedef struct _LinkedListInfo
  LinkedListInfo;

extern MagickExport HashmapInfo
  *NewHashmap(const unsigned long,size_t (*)(const void *),void *(*)(void *),
    void *(*)(void *));

extern MagickExport LinkedListInfo
  *NewLinkedList(const unsigned long);

extern MagickExport MagickBooleanType
  AppendElementToLinkedList(LinkedListInfo *,const void *),
  InsertElementInLinkedList(LinkedListInfo *,const unsigned long,const void *),
  InsertElementInSortedLinkedList(LinkedListInfo *,
    int (*)(const void *,const void *),void **,const void *),
  IsHashmapEmpty(const HashmapInfo *),
  IsLinkedListEmpty(const LinkedListInfo *),
  LinkedListToArray(LinkedListInfo *,void **),
  PutEntryInHashmap(HashmapInfo *,const void *,const void *);

extern MagickExport size_t
  HashPointerType(const void *),
  HashStringType(const void *),
  HashStringInfoType(const void *);

extern MagickExport unsigned long
  GetNumberOfElementsInLinkedList(const LinkedListInfo *),
  GetNumberOfEntriesInHashmap(const HashmapInfo *);

extern MagickExport void
  ClearLinkedList(LinkedListInfo *,void *(*)(void *)),
  DestroyHashmap(HashmapInfo *),
  DestroyLinkedList(LinkedListInfo *,void *(*)(void *)),
  *GetElementFromLinkedList(const LinkedListInfo *,const unsigned long),
  *GetEntryFromHashmap(const HashmapInfo *,const void *),
  *GetLastElementInLinkedList(const LinkedListInfo *),
  *GetNextElementInLinkedList(LinkedListInfo *),
  *GetNextEntryInHashmap(HashmapInfo *),
  *RemoveElementByValueFromLinkedList(LinkedListInfo *,const void *),
  *RemoveElementFromLinkedList(LinkedListInfo *,const unsigned long),
  *RemoveEntryFromHashmap(HashmapInfo *,const void *),
  *RemoveLastElementFromLinkedList(LinkedListInfo *),
  ResetHashmapIterator(HashmapInfo *),
  ResetLinkedListIterator(LinkedListInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
