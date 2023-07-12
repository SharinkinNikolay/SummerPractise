#include "ListEntry.h"

int main() {
    LinkedList* list = createLinkedList();

    String* string1 = (String*)malloc(sizeof(String));
    string1->data = strdup("Hello");
    ListEntry* entry1 = createListentry(string1);
    appendToList(list, entry1);

    String* string2 = (String*)malloc(sizeof(String));
    string2->data = strdup("World");
    ListEntry* entry2 = createListentry(string2);
    appendToList(list, entry2);

    String* string3 = (String*)malloc(sizeof(String));
    string3->data = strdup("GPTie");
    ListEntry* entry3 = createListentry(string3);
    appendToList(list, entry3);

    bubbleSort(list);

    ListEntry* current = list->head;
    while (current != NULL) {
        printf("%s\n", current->string->data);
        current = current->next;
    }

    freeLinkedList(list);

    return 0;
}