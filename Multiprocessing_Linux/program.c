#include <stdio.h>
extern int add(int a, int b);
int main(){
	int sum = add(2,3);
	printf("%d\n", sum);
	return 0;
}
