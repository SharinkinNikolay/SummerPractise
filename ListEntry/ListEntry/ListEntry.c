#include "ListEntry.h"

ListEntry* createListentry(String* string) {
    ListEntry* entry = (ListEntry*)malloc(sizeof(ListEntry));
    entry->next = NULL;
    entry->prev = NULL;
    entry->string = string;
    return entry;
}

LinkedList* createLinkedList() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void appendToList(LinkedList* list, ListEntry* entry) {
    if (list->head == NULL) {
        list->head = entry;
        list->tail = entry;
    }
    else {
        entry->prev = list->tail;
        list->tail->next = entry;
        list->tail = entry;
    }
}

void swapEntries(ListEntry* entry1, ListEntry* entry2) {
    String* temp = entry1->string;
    entry1->string = entry2->string;
    entry2->string = temp;
}

void bubbleSort(LinkedList* list) {
    int swapped;
    ListEntry* current;
    ListEntry* last = NULL;

    if (list->head == NULL) {
        return 1;
    }
    do {
        swapped = 0;
        current = list->head;

        while (current->next != last) {
            if (strcmp(current->string->data, current->next->string->data) > 0) {
                swapEntries(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}

void freeLinkedList(LinkedList* list) {
    ListEntry* current = list->head;
    while (current != NULL) {
        ListEntry* next = current->next;
        free(current->string->data);
        free(current->string);
        free(current);
        current = next;
    }
    free(list);
}