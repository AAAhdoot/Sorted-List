#include "sorted-list.h"
#include "sorted-list.c"


/*Compare Function for testing. Compares two ints*/
int compare(void* a, void* b){
	int x;
	int y;
	x = *(int*)a;
	y = *(int*)b;
	if(x==y) return 0;
	if(x<y) return -1;
	return 1;
}

/*Destroy Function for testing. Free's malloced ints*/
void destroy(void* data){
	free(data);
	return;
}

/*Creates a malloced int and returns the void* pointer to the data*/
void* itv(int a){
	int* b = (int*)malloc(sizeof(int));
	*b = a;
	return(void*)(b);
}

/*Print function that uses SortedListIterators*/
void print_em(SortedListPtr list){
	SortedListIteratorPtr iter;
	int *x;
	int y;
	printf("List contains: ");
	if(list == NULL){
		printf("NULL list object \n");
		return;
	}
	iter = SLCreateIterator(list);
	if(SLGetItem(iter) == 0){
		printf("\n");
		free(iter);
		return;
	}
	y = *(int*)SLGetItem(iter);
	printf("%d ",y);
	while((x = (int*)SLNextItem(iter))!=NULL){
			y = *x;
			printf("%d ",y);

	}
	printf("\n");
	free(iter);
	return;
}

/* Runs our test cases as expalined in the testcases file*/
int main(){
	SortedListPtr list, list2;
	SortedListIteratorPtr iter, iter2;
	
	printf("\n");
	/*Test case 1: Basic insetrions and removals*/
	printf("Test case 1: Basic insertions and removals\n");
	list = SLCreate(compare, destroy);
	SLInsert(list,itv(7));
	SLInsert(list,itv(15));
	SLInsert(list,itv(2));
	SLInsert(list,itv(5));
	printf("After insertion of : 7 15 2 5\n");
	print_em(list);
	void* a = itv(15);
	void* b = itv(5);
	void* c = itv(7);
 	void* d = itv(2);
	SLRemove(list, a);
	SLRemove(list, b);
	printf("After removal of : 15 5\n");
	print_em(list);
	SLRemove(list, c);
	SLRemove(list, d);
	free(a);
	free(b);
	free(c);
	free(d);
	printf("After removal of : 15 5 7 2\n");
	print_em(list);
	SLDestroy(list);
	printf("\n");

	/*Test case 2: Double add and deletion of a nonexistent object*/
	printf("Test case 2: Double add and deletion of a nonexistent opject\n");
	list = SLCreate(compare, destroy);
	SLInsert(list,itv(114));
	SLInsert(list,itv(51));
	void* m =itv(114);
	void* e = itv(87);
	SLInsert(list, m);
	SLRemove(list, e);
	free(e);
	free(m);
	SLInsert(list,itv(12));
	printf("After insertion of : 114 51 114\n");
	printf("After removal of : 87\n");
	printf("After insertion of : 12\n");
	print_em(list);
	SLDestroy(list);
	printf("\n");

	printf("Test case 3: Zero and negative check\n");
	list = SLCreate(compare, destroy);
	SLInsert(list,itv(12));
	SLInsert(list,itv(0));
	SLInsert(list,itv(-2));
	printf("After insertion of : 12 0 -2\n");
	print_em(list);
	printf("After removal of : 0\n");
	void* f = itv(0);
	SLRemove(list, f);
	free(f);
	print_em(list);
	SLDestroy(list);

	printf("\n");

	/*Test case 4: Delete nodes with iterators and re-enter the list using get next*/
	printf("Test case 4: Delete nodes with iterators and re-enter the list using get next\n");
	list = SLCreate(compare, destroy);
	SLInsert(list,itv(128));
	SLInsert(list,itv(35));
	SLInsert(list,itv(-2));
	printf("After insertion of : 128 35 -2\n");
	print_em(list);
	iter = SLCreateIterator(list);
	iter2= SLCreateIterator(list);
	SLNextItem(iter2);
	void* g = itv(128);	
	void* h = itv(35);	
	SLRemove(list,  g );
	SLRemove(list,  h );
	free(g);
	free(h);
	printf("After removal of : 128 35\n");
	print_em(list);
	printf("First deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter));
	printf("Second deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter2));
	printf("NextItem for both iterator returns: ");
	printf("%d ",*(int*)SLNextItem(iter2));
	printf("%d\n",*(int*)SLNextItem(iter));
	SLDestroyIterator(iter);
	SLDestroyIterator(iter2);
	SLDestroy(list);
	printf("\n");

	/*Test case 5: Delete nodes with iterators and exit list using get next*/
	printf("Test case 5: Delete nodes with iterators and exit list using get next\n");
	list = SLCreate(compare, destroy);
	SLInsert(list, itv(25));
	SLInsert(list, itv(64));
	SLInsert(list, itv(56));
	printf("After insertion of : 25 64 56\n");
	print_em(list);
	printf("Create 2 iterators and move iterators to the last 56 25 \n");
	iter = SLCreateIterator(list);
	iter2 = SLCreateIterator(list);
	SLNextItem(iter);
	SLNextItem(iter2);
	SLNextItem(iter2);
	void* i = itv(56);
	void* j = itv(25);
	SLRemove(list, i);
	SLRemove(list, j);
	free(i);
	free(j);
	printf("After removal of : 56 25\n");
	print_em(list);
	printf("First deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter));
	printf("Second deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter2));
	printf("NextItem for both iterator returns: ");
	if(SLNextItem(iter2) == NULL) printf("NULL ");
	if(SLNextItem(iter) == NULL) printf("NULL\n");
	SLDestroy(list);
	SLDestroyIterator(iter);
	SLDestroyIterator(iter2);
	printf("\n");

	/*Test case 6: Destroy list with 2 active iterators*/
	printf("Test case 6: Destroy list with 2 active iterators\n");
	list2 = SLCreate(compare,destroy);
	SLInsert(list2, itv(8));
	SLInsert(list2, itv(1));
	SLInsert(list2, itv(9));
	SLInsert(list2, itv(456));
	printf("After insertion of : 8 1 9 456\n");
	print_em(list2);
	printf("Create 2 iterators and move to 9 and 8\n");
	iter = SLCreateIterator(list2);
	iter2 = SLCreateIterator(list2);
	SLNextItem(iter);
	SLNextItem(iter2);
	SLNextItem(iter2);
	printf("Destroy List\n");
	SLDestroy(list2);
	printf("First deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter));
	printf("Second deleted node value: ");
	printf("%d\n",*(int*)SLGetItem(iter2));
	printf("NextItem for both iterator returns: ");
	if(SLNextItem(iter) == NULL) printf("NULL ");
	if(SLNextItem(iter2) == NULL) printf("NULL\n");
	SLDestroyIterator(iter);
	SLDestroyIterator(iter2);
	printf("\n");

	/*Test case 7: Destroy iterator with on deleted nodes for Valgrind memcheck*/
	printf("Test case 7: Destroy iterator on deleted nodes\n");
	list2 = SLCreate(compare,destroy);
	SLInsert(list2, itv(15));
	SLInsert(list2, itv(32));
	SLInsert(list2, itv(84));
	printf("After insertion of : 15 32 84\n");
	print_em(list2);
	printf("Create an iterators at 84\n");
	iter = SLCreateIterator(list2);
	printf("Remove 32 84\n");
	void* k = itv(84);
	void* l = itv(32);
	SLRemove(list2, k);
	SLRemove(list2, l);
	free(k);
	free(l);
	printf("After removal of : 84 32\n");
	print_em(list2);
	printf("Destroy iterator\n");
	SLDestroyIterator(iter);
	print_em(list2);
	SLDestroy(list2);
	printf("\n");
	return 0;
}
