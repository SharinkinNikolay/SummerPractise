#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* data;
}String;

typedef struct ListEntry {
    struct ListEntry* next;
    struct ListEntry* prev;
    String* string;
}ListEntry;

typedef struct {
    ListEntry* head;
    ListEntry* tail;
}LinkedList;

ListEntry* createListentry(String* string);
LinkedList* createLinkedList();

void appendToList(LinkedList* list, ListEntry* entry);
void swapEntries(ListEntry* entry1, ListEntry* entry2);
void bubbleSort(LinkedList* list);
void freeLinkedList(LinkedList* list);
