#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	char* name;
	struct node *nextSib;
	struct node *firstChild;
	char type; /* 'd' == directory; 'f' == file */
}NODE;

typedef struct tree{
	NODE* root;
	int depth;
}TREE;

TREE* tree;

TREE* create_tree(){
	
	TREE* res = (TREE*)malloc(sizeof(TREE));
	NODE* root = (NODE*)malloc(sizeof(NODE));

	if(res == NULL)        printf("error allocating tree\n");
	else if(root == NULL)  printf("error allocating root\n");
	else{
		/* root of every tree is "/" */
		root->name = "/";
		root->nextSib = NULL;
		root->firstChild = NULL;
		root->type = 'd';
		
		res->root = root;
		res->depth = 1;
	}

	return res;
}

/* given a string, return the index of the first instance of the '/' char */
int index_of_first_slash(char* str){
	char tmp = *str;
	int i = 0;

	while(tmp != '\0'){
		if(tmp == '/') return i;
		i++;
		tmp = *(str+i);

	}
	return -1; /* '/' not found in str */
}

/* given a tree and a path, return the node pointer associated with that path 
 * return null if the path does not exist
 */
NODE* get_node(TREE* tree, char* path){
	char *dir = "", *tmp = "";
	int pathLength = strlen(path), idOfSlash1 = -1, tmpLength = -1;
	NODE* curNode = NULL;

	if(tree == NULL){
		printf("tree is not initialized\n");
		return NULL;
	}else if(tree->root == NULL){
		printf("tree is empty\n");
		return NULL;
	}

	dir = (char*)malloc(sizeof(char) * pathLength);
	tmp = (char*)malloc(sizeof(char) * pathLength);
	strcpy(tmp, path);
	if(tmp[0] == '/'){ //take care of leading slash
		strcpy(dir, "/");
		strcpy(tmp, tmp+1);
	}

	idOfSlash1 = index_of_first_slash(tmp);
	tmpLength = strlen(tmp);
	curNode = tree->root;
	while(strcmp(tmp, "") != 0 && strcmp(tmp, "/") != 0){
		if(curNode == NULL){
			printf("ERROR: path: %s DNE\n", path);
			return NULL;
		}
		while(strcmp(curNode->name, dir) != 0){
			curNode = curNode->nextSib;
			if(curNode == NULL){
				printf("ERROR: path: %s DNE\n", path);
				return NULL;
			}
		}
		strncpy(dir, tmp, idOfSlash1);
		dir[idOfSlash1] = '\0';		strncpy(tmp, tmp + idOfSlash1 + 1, tmpLength - idOfSlash1 - 1);
		tmp[tmpLength-idOfSlash1-1] = '\0';
		
		curNode = curNode->firstChild;
		idOfSlash1 = index_of_first_slash(tmp);
		tmpLength = strlen(tmp);
	}
	if(strcmp(curNode->name, dir) == 0){
		return curNode;
	}
	else{
		//look through siblings to find it
		while(curNode != NULL && strcmp(curNode->name, dir) != 0){
			curNode = curNode->nextSib;
		}
		if(curNode == NULL){
			printf("ERROR: path: %s DNE\n", path);
			return NULL;
		}else{
			return curNode;
		}
	}
}

int index_of_last_slash(char* str){
	int i=-1;
	int ptr=0;
	int length = strlen(str);

	while(ptr < length){
		if(str[ptr] == '/'){
			i = ptr;
		}
		ptr++;
	}
	return i;
}

NODE* get_parent(TREE* tree, char* path){
	int length = strlen(path), lastSlashIndex;
	char *str, *parentPath;
	NODE* parent;

	str = (char*)malloc(sizeof(char)*length);
	parentPath = (char*)malloc(sizeof(char)*length);
	strcpy(str, path);

	if(str[length-1] == '/'){
		str[length-1] = '\0';
	}
	lastSlashIndex = index_of_last_slash(str);
	strncpy(parentPath, str, lastSlashIndex + 1);
	parentPath[lastSlashIndex + 1] = '\0';
	return get_node(tree, parentPath);
}

/* given a path and a tree, create a new node, 
 * and add it to the tree in correct place (rep'd by location)
 */

void add_node(TREE* tree, char* path){
	NODE *parent, *new_node , *child;
	int lastSlashIndex, pathLength = strlen(path);
	char str[pathLength+1];

	//create node
	new_node = (NODE*)malloc(sizeof(NODE));
	new_node->firstChild = NULL;
	new_node->nextSib = NULL;
	strcpy(str, path);
	if(str[pathLength-1] == '/'){
		new_node->type = 'd';
		str[pathLength-1] = '\0';
	}
		//get name of node
	lastSlashIndex = index_of_last_slash(str);
	strncpy(str, str + lastSlashIndex + 1, strlen(str) - lastSlashIndex - 1);
	str[strlen(str) - lastSlashIndex - 1] = '\0';
	//new_node->name = str;
	new_node->name = (char*)malloc(sizeof(char) * strlen(str));
	strcpy(new_node->name, str);

	//get parent of the node
	parent = get_parent(tree, path);
	if(parent == NULL){
		printf("could not create new node: %s because parent does not exist!\n", path);
		return;
	}

	if(parent->firstChild == NULL){
		parent->firstChild = new_node;
	}else{
		child = parent->firstChild;
		while(child->nextSib != NULL){
			if(strcmp(child->name, new_node->name) == 0){
				printf("This element: %s already exists!\n", path);
				return;
			}
			child = child->nextSib;
		}
		if(strcmp(child->name, new_node->name) == 0){
				printf("This element already exists!\n");
				return;
			}
		child->nextSib = new_node;
	}
}

void printAllChildren(TREE* tree, char* path){
	NODE* dir;
	NODE* child;

	dir = get_node(tree, path);
	if(dir == NULL){
		printf("ERROR: Path does not exist\n");
		return;
	}

	child = dir->firstChild;
	if(child == NULL){
		printf("%s is empty!\n", dir->name);
	}else{
		while(child != NULL){
			printf("%s\n", child->name);
			child = child->nextSib;
		}
	}
}

/* helper function to print tree recursively */
/*TODO: consider changing to iterative function*/
void print_tree_recursive(TREE* tree, NODE* root, char* path){
	NODE* tmp;
	char* path2;

	path2 = (char*)malloc(sizeof(char)*500);

	//strcat(path, "/");
	strcat(path, root->name);
	if(root->type == 'd' && strcmp(root->name, "/") != 0)		strcat(path, "/");
	
	printf("Path of %s = %s\n", root->name, path);

	tmp = root->firstChild;
	while(tmp != NULL){
		strcpy(path2,path);
		print_tree_recursive(tree,tmp,path2);
		tmp = tmp->nextSib;
	}

	free(path2);
}

/* given a tree: print the path for each node in the tree */
void print_tree(TREE* tree){
	char* path;

	path = (char*)malloc(sizeof(char) * 500);

	print_tree_recursive(tree, tree->root, path);
	free(path);
}

void destroy_tree_recurssive(TREE* tree, NODE* node){
	//base case: leaf & last sib
	if(node->firstChild == NULL && node->nextSib == NULL){
		free(node);
		return;
	}else if(node->nextSib == NULL){ //recurssive case 1: last Sib ONLY -> remove node and all children
		destroy_tree_recurssive(tree, node->firstChild);
		free(node);
		return;
	}else if(node->firstChild == NULL){ //recurssive case 3: leaf ONLY -> remove node and all sibs
		destroy_tree_recurssive(tree, node->nextSib);
		free(node);
		return;
	}else{ //recurssive case 3: node has sibs and cildren -> remove node & all sibs & all children
		destroy_tree_recurssive(tree, node->nextSib);
		destroy_tree_recurssive(tree, node->firstChild);
		free(node);
		return;
	}
}

void destroy_tree(TREE* tree){
	destroy_tree_recurssive(tree,tree->root);
}

void deparse_recursive(TREE* tree, NODE* root, FILE* out){
	NODE* tmp;
	char* str;
	str = (char*)malloc(sizeof(char)*1000);
	str[0] = '\0';

	strcat(str, "<");
	strcat(str, root->name);
	strcat(str, ">");

	if(root->type == 'f'){
		//handle file contents
	}
	
	if(root!=tree->root){
		fprintf(out, "%s\n", str);
		strcpy(str, "");	
	}	
	
	tmp = root->firstChild;
	while(tmp!=NULL){
		deparse_recursive(tree, tmp, out);
		tmp = tmp->nextSib;
	}
	strcat(str, "</");
	strcat(str, root->name);
	strcat(str, ">");
	if(root!=tree->root){
		fprintf(out, "%s\n", str);
		strcpy(str, "");
	}
	
	free(str);

}

void deparse(TREE* tree){
	FILE *file = fopen("parsedexample.xml", "w");
	fclose(file);
	fopen("parsedexample.xml", "a");
	deparse_recursive(tree, tree->root, file);
	fclose(file);
}

int main(int argc, char *argv[])
{
  printf("main\n");
	int length;

	tree = create_tree();
	
	static const char filename[] = "path.txt";
	FILE *file = fopen (filename, "r");
	if(file != NULL) {
		char line [128]; 
		while (fgets(line, sizeof line, file) != NULL){ 
			//printf("%s",line); 
			length = strlen(line);
			if(line[length-1] == '\n')
   				line[length-1] = '\0';
   			if(line[length-2] == '\r')
   				line[length-2] = '\0';
			//change to mkdir and then add node in mkdir 
			if(strcmp(line, ""))
				add_node(tree, line);
		}
		fclose (file);
	}else{
		perror (filename); 
	}
		print_tree(tree);
		deparse(tree);
}
