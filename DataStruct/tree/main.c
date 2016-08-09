#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[])
{
    Tree* tree = Tree_Create();
    
    Tree_Destroy(tree);
    
	return 0;
}
