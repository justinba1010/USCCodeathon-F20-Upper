#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int weight;
	int neighbor;
} node;

int main(){
	int size;
	node n;
scanf("%d", &size);
//scanf("%d:%d", &n.neighbor, &n.weight);





size_t bufsize = 1000;
size_t characters;

char* b = (char*)malloc(bufsize*sizeof(char));

printf("type here: ");
characters = getline(&b, &bufsize, stdin);
printf("%zu characters were read.\n",characters);
printf("You typed: '%s'\n",b);

//printf("%d,%d,%d\n", size, n.neighbor, n.weight);
return 0;
}
