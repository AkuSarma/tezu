#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedList {
	int val;
	struct LinkedList* next;
} LinkedList;


static LinkedList* create_node(int v) {
	LinkedList* n = (LinkedList*)malloc(sizeof(LinkedList));
	if (!n) return NULL;
	n->val = v;
	n->next = NULL;
	return n;
}

static void append_node(LinkedList** head, int v) {
	if (!*head) {
		*head = create_node(v);
		return;
	}
	LinkedList* cur = *head;
	while (cur->next){ 
        cur = cur->next;
    }
	cur->next = create_node(v);
}

static int exists_in_list(LinkedList* head, int v) {
	while (head != NULL) {
        if (head->val == v){ 
            return 1;
        }
        head = head->next;
    }
	return 0;
}

static void append_unique(LinkedList** head, int v) {
	if (!exists_in_list(*head, v)){ 
        append_node(head, v);
    }
}

LinkedList* list_union(LinkedList* first, LinkedList* second) {
	LinkedList* result = NULL;
	for (LinkedList* p = first; p; p = p->next) {
        append_unique(&result, p->val);
    }
	for (LinkedList* p = second; p; p = p->next){ 
        append_unique(&result, p->val);
    }
	return result;
}

LinkedList* list_intersection(LinkedList* first, LinkedList* second) {
	LinkedList* result = NULL;
	for (LinkedList* p = first; p; p = p->next) {
		if (exists_in_list(second, p->val)){ 
            append_unique(&result, p->val);
        }
	}
	return result;
}

LinkedList* list_difference(LinkedList* first, LinkedList* second) {
	LinkedList* result = NULL;
	for (LinkedList* p = first; p; p = p->next) {
		if (!exists_in_list(second, p->val)) {
            append_unique(&result, p->val);
        }
	}
	return result;
}


int main() {
	LinkedList* first = NULL;
	LinkedList* second = NULL;

    printf("Enter the number of elements in set a: ");
    int n;
    scanf("%d", &n);
	printf("Give elements of set a : ");
    int a;
	for (int i = 0; i < n; i++) {
        scanf("%d", &a);
		append_node(&first, a);
	}

    printf("Enter the number of elements in set b: ");
    scanf("%d", &n);
    printf("Give elements of set b : ");
	for (int i = 0; i < n; i++) {
        scanf("%d", &a);
		append_node(&second, a);
	}

	LinkedList* uni = list_union(first, second);
	LinkedList* inter = list_intersection(first, second);
	LinkedList* diff = list_difference(first, second);

	printf("Union: ");
	for (LinkedList* p = uni; p; p = p->next) {
        printf("%d ", p->val);
    }
	printf("\n");

	printf("Intersection: ");
	for (LinkedList* p = inter; p; p = p->next){ 
        printf("%d ", p->val);
    }
	printf("\n");

	printf("Difference: ");
	for (LinkedList* p = diff; p; p = p->next){ 
        printf("%d ", p->val);
    }
	printf("\n");

	return 0;
}