#include "sorted-list.h"

/*Creates and Initializes a Sorted List object*/
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	if(cf!= NULL && df!=NULL){
		SortedListPtr slptr = (SortedListPtr)malloc(sizeof(struct SortedList));
		slptr -> head = NULL;
		slptr ->compare = cf;
		slptr ->destroy = df;
		return slptr;
	}
	else{
		return NULL;
	}
}

/*Creates an Iterator object and sets curr to head of list*/
SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr SortedListIterT;
	if(list == NULL){
		return NULL;
	}
	SortedListIterT = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	SortedListIterT -> curr = list->head;
	SortedListIterT -> destroy = list->destroy;
	if(SortedListIterT -> curr!= NULL){
	list->head->ref_count++; 
	}
	return SortedListIterT;
}

/*Creates and Initializes a node object*/
NodePtr CreateNode(void* newObj){
 	NodePtr addition = (NodePtr)malloc(sizeof(Node));
 	addition -> data = newObj;
 	addition -> ref_count = 1;
	addition -> to_delete = 0;
 	addition -> next = NULL;
 	return addition;
}

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to an object
 * already in the list (according to the comparator function), then the operation should fail.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj){
 	int currcomp;
 	NodePtr curr, previous, addition;
 	curr = list->head;
 	previous = NULL;

 	if (list==NULL || newObj==NULL){
 		return 0;
 	}

 	/*empty list addition case*/
 	if(curr == NULL){
 		addition = CreateNode(newObj);
 		list->head = addition;
 		addition->next = NULL;
 		return 1;
 	}

 	/*head cases*/
	currcomp = (*list->compare)(newObj,curr->data);
	if(currcomp==0){
		return 0;
	}
	if(currcomp > 0){
		addition = CreateNode(newObj);
		addition->next = list->head;
		list->head = addition;
		return 1;
	}

	/*standard case*/
	previous = curr;
	curr = curr->next; 
 	while(curr!=NULL){
 		currcomp =  (*list->compare)(newObj,curr->data);
 		if(currcomp==0){
 			return 0;
 		}
 		if(currcomp>0){
 			addition = CreateNode(newObj);
			addition->next = curr;
			previous->next = addition;
			return 1;
 		}
		previous=curr;
		curr=curr->next;
 	}

	addition = CreateNode(newObj);
	previous->next = addition;
 	return 1;	
 }

 /*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
  */

/*Our implementation removes the node from the list and frees if possible; otherwise marks it as to_delete*/
int SLRemove(SortedListPtr list, void *newObj){
	NodePtr curr, prev;
	if(list == NULL || newObj==NULL) return 0;
	if(list->head == NULL) return 0;

	curr = list->head;
	/*head is value to remove; increment the next's ref_count if not fully deleted*/
	if((*list->compare)(newObj, curr->data) == 0){
		list->head = curr->next;

		curr->ref_count--;
		if(curr->ref_count == 0){
			(*list->destroy)(curr->data);
			free(curr);
			return 1;
		}
		if(curr->next) curr->next->ref_count++;
		curr->to_delete = 1;
		return 1; 
	}

	prev = list->head;
	curr = list->head->next;
	/*examine other nodes for value to remove; If found, increment the next's ref_count if not fully deleted*/
	while(curr != NULL){
		if((*list->compare)(newObj, curr->data) == 0){
			prev->next = curr->next;

			curr->ref_count--;
			if(curr->ref_count == 0){
				(*list->destroy)(curr->data);
				free(curr);
				return 1;
			}
			if(curr->next) curr->next->ref_count++;
			curr->to_delete = 1;
			return 1;
		}
		prev = curr;
		curr = curr->next;
	}
	/* node not found */
	return 0;
}

/*Goes through the list and destroys all nodes without extra pointers to them and sets all remaining nodes to delete*/
void SLDestroy(SortedListPtr list){
	NodePtr curr, prev;
	curr = list->head;
	while(curr){
		prev = curr;
		curr->ref_count--;
		curr->to_delete = 1;
		/* destroys nodes that are safe to destroy*/
		if(curr->ref_count == 0){
			(*list->destroy)(curr->data);
			curr = curr->next;
			free(prev);
		}
		/*otherwise sets next to null to avoid garbage pointers*/
		else{
			curr = curr->next;
			prev->next = NULL;
		}
	}
	list->head= NULL;
	free(list);
	return;
}
/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
	NodePtr destroy, curr;
	if(iter->curr == NULL){
		free(iter);
		return;
	}
	/*decrement the ref count and delete node if necessary*/
	iter->curr->ref_count--;
	if(iter->curr->ref_count == 0){
		destroy = iter->curr;
		curr = iter->curr->next;
		(*iter->destroy)(destroy->data);
		free(destroy);
		/* if node was deleted decrement ref count on the next node and delete again if needed*/
		while(curr){
		    curr->ref_count--;
		    if(curr->ref_count != 0) break;
			destroy = curr;
			curr = curr->next;
			(*iter->destroy)(destroy->data);
			free(destroy);
		}
	}
	free(iter);
	return;
}
 
/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 *
 * Note the function will return 0 for deleted nodes as well(should note in big documentation)
 *
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
	if(iter->curr == NULL){
		return 0;
	}
	return (iter->curr->data);
}

/*
 * SLNextItem returns the pointer to the data associated with the
 * next object in the list associated with the given iterator.
 * It should return a NULL when the end of the list has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list associated with an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter){
	NodePtr destroy;
	if(iter==NULL || iter->curr==NULL){
		return NULL;
	}
	iter->curr->ref_count--;
	/* delete current node if ref count is zero after iteration*/
	if(iter->curr->ref_count == 0){
		destroy = iter->curr;
		iter->curr = iter->curr->next;
		(*iter->destroy)(destroy->data);
		free(destroy);

		/* if node was deleted decrement ref count on the next node and delete again if needed*/
		while(iter->curr){
			iter->curr->ref_count--;
			if(iter->curr->ref_count != 0) break;
			destroy = iter->curr;
			iter->curr = iter->curr->next;
			(*iter->destroy)(destroy->data);
			free(destroy);
		}
	}
	else{
		iter->curr = iter->curr->next; 
	}
	/* Advance past all nodes stated for deletion*/
	while(iter->curr){
		if(iter->curr->to_delete){
			iter->curr = iter->curr->next;
		}else{
			break;
		}		
	}

	if(iter->curr == NULL){ 
		return NULL;
	}
	iter->curr->ref_count++;
	return iter->curr->data;
}

