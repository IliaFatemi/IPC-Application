#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

List listWithHeads[LIST_MAX_NUM_HEADS]; // a list with the max num of heads
Node listWithNodes[LIST_MAX_NUM_NODES]; // a list with the max num of nodes

List* List_create(){
    static int listIndex = 0;

    if (listIndex < LIST_MAX_NUM_HEADS){
        List *new_list = &listWithHeads[listIndex++];
        new_list->head = NULL;
        new_list->tail = NULL;
        new_list->current = NULL;
        new_list->boundary_position = LIST_INBOUND;
        new_list->size = 0;
        return new_list;
    }
    printf("Max number of heads reached.\n"); 
    return NULL;
}

int List_count(List* pList){
    if(pList == NULL){return LIST_FAIL;}
    return pList->size;
}

void* List_first(List* pList){
    // If the head is not empty, set the current to the head.
    // Otherwise set current to NULL
    if(pList->head != NULL){
        pList->current = pList->head;
        return pList->current->data;
    }else{
        pList->current = NULL;
        pList->boundary_position = LIST_INBOUND;
        return NULL;
    }
}


void* List_last(List* pList){
    // If the tail is not empty, set the current to the head.
    // Otherwise set current to NULL
    if(pList->tail != NULL){
        pList->current = pList->tail;
        return pList->current->data;
    }else{
        pList->current = NULL;
        pList->boundary_position = LIST_INBOUND;
        return NULL;
    }
}

void* List_next(List* pList){
    if (pList->size == 0){return 0;}
    if (pList->boundary_position == LIST_OOB_START){
        pList->current = pList->head;
        pList->boundary_position = LIST_INBOUND; // in boundary
        return pList->current->data;
    }else{
        // boundary check
        if(pList->current == pList->tail){
            pList->current = NULL;
            // printf("fdsfsdf\n");
            pList->boundary_position = LIST_OOB_END; // beyond the tail
            return NULL;
        }else{
            pList->current = pList->current->next; //advance to next item
            return pList->current->data;
        }

    }
    
    
}

void* List_prev(List* pList){
    if (pList->size == 0){return 0;}
    //boundary check
    if(pList->boundary_position == LIST_OOB_END){
        pList->current = pList->tail;
        pList->boundary_position = LIST_INBOUND;
        return pList->current->data;
    }else{
        if(pList->current != pList->head){
            // printf("=====\n");
            pList->current = pList->current->previous;
            pList->boundary_position = LIST_OOB_START;
            return pList->current->data;
        }else{
            pList->current = NULL;
            pList->boundary_position = LIST_OOB_START;
            return NULL;
        }
    }
    // pList->current = pList->current->previous;
    // pList->boundary_position = LIST_INBOUND; // in boundary
    // return pList->current->data;
}

int List_append(List* pList, void* pItem){
    Node * new_node = &listWithNodes[SUM_NODES];
    new_node->data = pItem;
    new_node->next = NULL;
    new_node->previous = NULL;
    //If empty add the item to the first index
    if (pList->head == NULL){
        pList->head = new_node;
        pList->current = new_node;
        pList->tail = new_node;
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }else{
        new_node->previous = pList->tail;
        new_node->previous->next = new_node;
        pList->current = new_node;
        pList->current->next = new_node;
        pList->tail = new_node;
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }
    return -1;
}

int List_prepend(List* pList, void* pItem){
    Node * new_node = &listWithNodes[SUM_NODES];
    new_node->data = pItem;
    new_node->next = NULL;
    new_node->previous = NULL;
    //If empty add the item to the first index
    if(pList->head == NULL){
        pList->head = new_node;
        pList->current = new_node;
        pList->tail = new_node;
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }else{
        new_node->next = pList->head;
        new_node->next->previous = new_node;
        pList->head = new_node;
        pList->current = new_node;
        // pList->current->previous = new_node;
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }
    return -1;
}


void* List_curr(List* pList){
    if (pList->current == NULL){
        return NULL;
    }
    return pList->current->data;
}


int List_insert_after(List* pList, void* pItem){
    Node * new_node = &listWithNodes[SUM_NODES];
    new_node->data = pItem;
    new_node->next = NULL;
    new_node->previous = NULL;
    if(pList->head == NULL){
        // printf("check1\n");
        pList->head = new_node;
        pList->tail = new_node;
        pList->current = new_node;
        
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }else{
        if(pList->boundary_position == LIST_OOB_START){
            // printf("check2\n");
            new_node->next = pList-> head;
            new_node->next->previous = new_node;
            pList->head = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }else if(pList->boundary_position = LIST_OOB_END){
            // printf("check3\n");
            new_node->previous = pList->tail;
            new_node->previous->next = new_node;
            pList->tail = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }else{
            // printf("check4\n");
            new_node->next = pList->current->next;
            pList->current->next = new_node;
            new_node->previous = pList->current;
            new_node->next->previous = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }
    }
    return -1;
}


int List_insert_before(List* pList, void* pItem){
    Node * new_node = &listWithNodes[SUM_NODES];
    new_node->data = pItem;
    new_node->next = NULL;
    new_node->previous = NULL;
    if(pList->head == NULL){
        pList->head = new_node;
        pList->tail = new_node;
        pList->current = new_node;
        
        pList->boundary_position = LIST_INBOUND;
        pList->size++;
        SUM_NODES++;
        return 0;
    }else{
        if(pList->boundary_position == LIST_OOB_START || pList->current == pList->head){
            new_node->next = pList-> head;
            new_node->next->previous = new_node;
            pList->head = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }else if(pList->boundary_position = LIST_OOB_END){
            new_node->previous = pList->tail;
            new_node->previous->next = new_node;
            pList->tail = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }else{
            new_node->next = pList->current;
            new_node->previous = pList->current->previous;
            pList->current->previous->next = new_node;
            pList->current->previous = new_node;
            pList->current = new_node;
            pList->boundary_position = LIST_INBOUND;
            pList->size++;
            SUM_NODES++;
            return 0;
        }

    }
    return -1;
    
}


void* List_remove(List* pList){
    if(pList->size == 0){
        return NULL;
    }

    if(pList->boundary_position == LIST_OOB_END || pList->boundary_position == LIST_OOB_START){
        return NULL;
    }

    Node* new_node = pList->current;
    void* current_node = new_node->data;

    if(pList->current->next == NULL || pList->current->previous == NULL){
        // If there is only one item
        if(pList->current == pList->tail && pList->current == pList->head){
            // printf("1\n");
            pList->head = NULL;
            pList->tail = NULL;
            pList->current = NULL;
            // pList->boundary_position = LIST_OOB_END;
            // pList -> current -> next -> previous = NULL;
            // pList -> current -> data = NULL;
            pList->size--;
            return current_node;
        }else if (pList->current == pList->tail){
            // printf("2\n");
            // pList->boundary_position = LIST_OOB_END;
            pList->tail = pList->tail->previous;
            pList->tail->next = NULL;
            pList->current = pList->tail;
            // pList -> current -> previous -> next = NULL;
            // pList -> current -> data = NULL;
            pList->size--;
            return current_node;
        }else if (pList->current == pList->head){
            // printf("3\n");
            // pList->boundary_position = LIST_OOB_START;
            pList->head = pList->head->next;
            pList->head->previous = NULL;
            pList->current = pList->head;
            pList->size--;
            return current_node;
        }
    }else{
        // printf("4\n");
        // pList->boundary_position = LIST_INBOUND;
        pList->current->previous->next = pList->current->next;
        pList->current->next->previous = pList->current->previous;
        pList->size--;
        return current_node;
    }
}


void* List_trim(List* pList){
    if(pList->size == 0 || pList->head == NULL){
        return NULL;
    }
    pList->current = pList->tail;
    Node* last_node = pList->current->data;
    pList->current = pList->tail->previous;
    if(pList->size != 1){
        pList->current->next = NULL;
    }
    pList->size--;
    pList->tail = pList->current;
    return last_node;
}


void List_concat(List* pList1, List* pList2){
    if(pList1 == NULL || pList2 == NULL){
        return;
    }

    // check if the size does not exceed the limit
    if(pList1->size + pList2->size <= LIST_MAX_NUM_NODES){
        if(pList1->head != NULL){
            pList1->tail->next = pList2->head;
            pList1->tail->next->previous = pList2->tail;
            pList1->tail = pList2->tail;
            pList1->size += pList2->size;
        }else{
            pList1->head = pList2->head;
            pList1->tail = pList2->tail;
            pList1->size += pList2->size;
        }

        //destroy pList2
        pList2->boundary_position = LIST_INBOUND;
        pList2->size = 0;
        pList2->current = NULL;
        pList2->head = NULL;
        pList2->tail = NULL;
    }
}


typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    void *temp = NULL;

    if (pList->head != NULL){
        //empty from the last item in the list
        while (pList->head != pList->tail){
            pList->current = pList->tail;
            temp = List_trim(pList);
            pItemFreeFn(temp);
        }

        if(pList->head == pList->tail){
            pList->current = pList->head;
            temp = List_trim(pList);
            pItemFreeFn(temp);
        }

        pList = NULL;

    }
}


typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    Node* node = pList->current;

    // if out of bounds at the head
    if(pList->boundary_position == LIST_OOB_START){
        node = pList->head;
    }

    while(node != NULL){
        if((*pComparator)(node->data, pComparisonArg)){
            pList->current = node;
            return node->data;
        }
        node = node->next;
    }

    //If match not found, set out of bounds towards the end
    pList->current = NULL;
    pList->boundary_position = LIST_OOB_END;
    return NULL;
}