#define SIZE 12
#include <stdio.h>
struct Node { //next is closer to the head of queue
	struct Node* prev;
	struct Node* next;
	int priority;
	char* name;
};
struct Queue {
	struct Node* head;
	struct Node* tail;
};
void push(struct Queue* q, struct Node* n, char* nm) {
	n->priority = 0;
	n->name = nm;
	n->prev = NULL;
	if (q->head == NULL) {
		n->next = NULL;
		q->head = q->tail = n;
	}
	else {
		n->next = q->tail;
		q->tail->prev = n;
		q->tail = n;
	}
	return;
}
void pop(struct Queue* q) {
	if (q->head == q->tail) {
		free(q->head);
		q->head = q->tail = NULL;
	}
	else {
		struct Node* tmp = q->head->prev;
		tmp->next = NULL;
		free(q->head);
		q->head = tmp;
	}
	return;
}
void print_queue(struct Queue* q) { //head of queue is on the right site
	struct Node* tmp = q->tail;
	while (tmp != NULL) {
		printf("%s ", tmp->name);
		tmp = tmp->next;
	} printf("\n");
	return;
}
void insert(struct Queue* q, struct Node* n, char* nm, int p) {
	n->name = nm;
	n->priority = p;
	if (q->head == NULL) { //if queue is empty
		n->prev = n->next = NULL;
		q->head = q->tail = n;
	}
	else { //queue not empty
		struct Node* tmp = q->head;
		if (tmp->priority < n->priority) { //priority of new node is higher than head's priority
			n->prev = tmp;
			n->next = NULL;
			tmp->next = n;
			q->head = n;
			return;
		}
		while (tmp->priority >= n->priority) { //traverse untill the right place in queue is found
			tmp = tmp->prev;
			if (tmp == NULL) { //new node will be on the end of queue
				n->prev = NULL;
				n->next = q->tail;
				q->tail->prev = n;
				q->tail = n;
				return;
			}
		}
		n->prev = tmp;
		n->next = tmp->next;
		n->next->prev = n;
		tmp->next = n;
	}
	return;
}
void delete_Node(struct Queue* q, struct Node* n) {
	if (n == q->head) {
		pop(q);
	}
	else if (n == q->tail) {
		struct Node* tmp = q->tail->next;
		tmp->prev = NULL;
		q->tail = tmp;
		free(n);
	}
	else {
		struct Node* pred, * succ; //predecessor and successor of the Node, successor is closer to head
		pred = n->prev;
		succ = n->next;
		pred->next = succ;
		succ->prev = pred;
		free(n);
	}
	return;
}
void delete_by_priority(struct Queue* q, int p) {
	struct Node* tmp = q->head;
	while (tmp != NULL) {
		struct Node* behind = tmp->prev;
		if (tmp->priority == p)
			delete_Node(q, tmp);
		tmp = behind;
	}
	return;
}
struct Queue merge_two(struct Queue* a, struct Queue* b) {
	struct Node* tmp = b->head;
	while (tmp != NULL) {
		struct Node* behind = tmp->prev;
		insert(a, tmp, tmp->name, tmp->priority);
		tmp = behind;
	}
	return *a;
}
void delete_list(struct Queue* q) {
	while (q->head != NULL) {
		delete_Node(q, q->head);
	}
}
int main() {
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->head = NULL;
	queue->tail = NULL;

	struct Node* n[SIZE];
	for (int i = 0; i < SIZE; i++) {
		n[i] = (struct Node*)malloc(sizeof(struct Node));
	}

	push(queue, n[0], "XD");
	push(queue, n[1], "Xd");
	push(queue, n[2], "xD");
	push(queue, n[3], "xd");
	print_queue(queue);
	pop(queue);
	print_queue(queue);

	insert(queue, n[4], "haha", 2);
	insert(queue, n[5], "hehe", 3);
	insert(queue, n[6], "hihi", 0);
	insert(queue, n[7], "hoho", 1);
	print_queue(queue);
	delete_by_priority(queue, 2);
	print_queue(queue);
	delete_by_priority(queue, 3);
	//delete_by_priority(queue, 0);
	print_queue(queue);

	struct Queue* queue2 = (struct Queue*)malloc(sizeof(struct Queue));
	queue2->head = NULL;
	queue2->tail = NULL;
	insert(queue2, n[8], "Frodo", 2);
	push(queue2, n[9], "Sam");
	insert(queue2, n[10], "Gandalf", 3);
	push(queue2, n[11], "Gollum");
	print_queue(queue2);

	struct Queue merged_queue = merge_two(queue, queue2);
	print_queue(&merged_queue);

	delete_list(&merged_queue);
	free(queue);
	free(queue2);
	return 0;
}