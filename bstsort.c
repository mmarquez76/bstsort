<<<<<<< HEAD
// Matt Taylor, PID 5848958
//
// This program takes input from stdin or from a file, and then uses a BST sort
// to sort the lines of the file alphabetically and output them.
// The number next to each line of output tells how many times that line appeared.
//
=======
// This program takes input from stdin or from a file, and then uses a BST sort
// to sort the lines of the file alphabetically and output them.
// The number next to each line of output tells how many times that line appeared.
>>>>>>> updating comments in main
// I affirm that I wrote this program myself without any help from any 
// other people or sources from the internet.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char usage[] = "usage: bstsort [-c] [-o output_file_name] [input_file_name]\n";


/* STRING FUNCTIONS
 * For all functions that are normally part of stdlib or string.h that we need to
 * redefine for this assignment, the function name is identical but with an 'm' 
 * prepended to the function name (e.g. strcmp becomes mstrcmp)
 *
 * Changed functions are mstrcmp and mstrcasecmp
 */
int mstrcmp(char *str1, char *str2) {
	int i = 0;
	// do the C equivalent of a while (true);
	while (1) {
		// if the current character is not equal, check which is lower
		if (str1[i] != str2[i])
			return str1[i] < str2[i] ? -1 : 1;
		// if they are equal and the current character is null, then
		// the strings must be equal; return 0
		else if (!str1[i])
			return 0;
		// otherwise, check the next character in the string
		++i;
	}
}

int mstrcasecmp(char *str1, char *str2) {
	int i = 0;

	while (1) {
		// same logic as strcmp, but using tolower to ignore case
		if (tolower(str1[i]) != tolower(str2[i]))
			return tolower(str1[i]) < tolower(str2[i]) ? -1 : 1;
		else if (!str1[i])
			return 0;
		++i;
	}
}

/* BINARY SEARCH TREE STRUCT DECLARATIONS
 */
typedef struct Node {
	struct Node *left, *right;  // declaring pointers to left and right nodes
	char *data;		    // string in node
	int count;		    // amount of times string appears
} Node;

typedef struct BST {
	Node *root;
} BST;


/* BINARY SEARCH TREE METHODS
 */

/* Recursively add a link a child node to a parent node, using pass-by-pointer to
 * modify the parent node. If parent node is null, child becomes parent.
 */
void add_child(Node **parent, Node *child) {
	if ((*parent) == NULL) {
		*parent = child;
		return;
	}	// if child string is less than parent, add it to the left
	if (mstrcmp(child->data, (*parent)->data) < 0) {
		add_child(&(*parent)->left, child);
		return;
	}	// if child string is greater than parent, add it to the right
	if (mstrcmp(child->data, (*parent)->data) > 0) {
		add_child(&(*parent)->right, child);
		return;
	}
	else {  // if we get here, the child node has equal contents to the parent
		++(*parent)->count;
		free(child); // we don't need child here, so free that extra memory
		return;
	}
}

/* Same as add_child, but case-insensitive. For use with -c flag.
 */
void add_child_case(Node **parent, Node *child) {
	if ((*parent) == NULL) {
		*parent = child;
		return;
	}	// if child string is less than parent, add it to the left
	if (mstrcasecmp(child->data, (*parent)->data) < 0) {
		add_child_case(&(*parent)->left, child);
		return;
	}	// if child string is greater than parent, add it to the right
	if (mstrcasecmp(child->data, (*parent)->data) > 0) {
		add_child_case(&(*parent)->right, child);
		return;
	}
	else {  // if we get here, the child node has equal contents to the parent
		++(*parent)->count;
		free(child); // we don't need child here, so free that extra memory
		return;
	}
}

/* Given a tree and a string, insert the string in its proper place in the tree.
 *
 * We're using pass-by-pointer here to effectively add str to the BST tree.
 */
void insert(BST *tree, char *str) {
	Node *temp = (Node*)malloc(sizeof(Node)); // initialize temp node
	
	temp->data = (char*)malloc((strlen(str) + 1) * sizeof(char));
	strcpy(temp->data, str);	// set temp node data to str
	temp->count = 1;
	
	add_child(&(tree->root), temp);
}

/* Same as insert, but ignoring case. For use with the -c flag.
 */
void insert_case(BST *tree, char *str) {
	Node *temp = (Node*)malloc(sizeof(Node)); // initialize temp node
	
	temp->data = (char*)malloc((strlen(str) + 1) * sizeof(char));
	strcpy(temp->data, str);	// set temp node data to str
	temp->count = 1;
	
	add_child_case(&(tree->root), temp);
}

/* Recursively prints a node and its children in inorder traversal.
 */
void tostring_node(Node *parent, char *result) {
	if (parent->data == NULL)
		return;

	if (parent->left != NULL) {
		tostring_node(parent->left, result);
	}

	// if count ever gets higher than 20 digits, we have bigger problems.
	char* countstr = (char*)malloc(20 * sizeof(char));
	sprintf(countstr, "%d", parent->count);

	strcat(result, countstr);
	strcat(result, " ");
	strcat(result, parent->data);

	if (parent->right != NULL) {
		tostring_node(parent->right, result);
	}
} 

/* Traverses a tree in inorder and print contents of the tree.
 */
char* tostring_tree(BST *tree) {
	char* ret = (char*)malloc(sizeof(char)); // start with empty string
	tostring_node(tree->root, ret);
	return ret;
}

/* Recursively traverses tree in postorder and frees nodes starting from bottom.
 */
void free_node(Node *parent) {
	if (parent->data == NULL)
		return;

	if (parent->left != NULL) {
		free_node(parent->left);
	}

	if (parent->right != NULL) {
		free_node(parent->right);
	}
	
	free(parent);
} 

/* Calls free_node to recursively free tree memory in postorder traversal.
 */
void free_tree(BST *tree) {
	free_node(tree->root);	//freenode -- IRC, anyone?
}


/* FILE I/O METHODS
 */

/**
 * read_from_file: read lines from a file and sort them using a BST
 * @filename: where to read file from
 * @c: whether to ignore case (0 means case-sensitive, 1 means ignore case)
 */
BST *read_from_file(char* filename, int c) {
	FILE *file;
	char line[101];
	char* temp;
	BST *tree = (BST*)malloc(sizeof(BST));

	// Open at filename for read-only access, after checking for existence
	file = fopen(filename, "r");
	if (file == NULL) {
		char *fileerror = "error: file %s does not exist\n";
		fprintf(stderr, fileerror, filename);
		fprintf(stderr, usage);
		exit(1);
	}	

	// As per assignment specs, each line is no more than 100 characters.
	// Buffer for fgets is 101 to leave space for /0
	// Comparing with \n to ensure we stop when we encounter a blank line
	while((fgets(line, 101, file)) != NULL && line[0] != '\n' && line[0] != '\0') {
		temp = (char*)malloc((strlen(line) + 1) * sizeof(char));
		strcpy(temp, line);
		if (c)
			insert(tree, temp);
		else
			insert_case(tree, temp);
	}

	return tree;
}

/**
 * read_from_stdin: read lines from stdin and sort them using a BST
 * @c: whether to ignore case (1 means case-sensitive, 0 means ignore case)
 */
BST *read_from_stdin(int c) {
	char line[101];
	char* temp;
	BST *tree = (BST*)malloc(sizeof(BST));

	printf("\nEnter lines. To finish, enter a blank line or hit Ctrl+D.\n");

	// As per assignment specs, each line is no more than 100 characters.
	// Buffer for fgets is 101 to leave space for /0
	// We compare to \n here, because char value 10 is a newline (empty line)
	while((fgets(line, 101, stdin)) != NULL && line[0] != '\n' && line[0] != '\0') {
		temp = (char*)malloc((strlen(line) + 1) * sizeof(char));
		strcpy(temp, line);
		if (c)
			insert(tree, temp);
		else
			insert_case(tree, temp);
	}
	
	return tree;
}

void write_to_file(BST *tree, char *filename) {
	FILE *file;

	// Open at filename for write access, overwriting previous file
	file = fopen(filename, "w");

	// Write string representation of BST to file
	fprintf(file, tostring_tree(tree));

	printf("Output written to %s\n", filename);
}

/* MAIN METHOD
 */
int main(int argc, char** argv) {
	extern char *optarg; // Points to argument for optional args
	extern int optind; // Used to find the num after optional args
	int arg;
	int cflag = 0, oflag = 0, err = 0;
	char *ostring = NULL, *filename = NULL; 

	// This while loop assigns arg to the next argument found by getopt
	// then checks if it's -1, because a -1 return indicates no more args
	while((arg = getopt(argc, argv, "o:c")) != -1) {
		// See which argument we got, and assign vars accordingly
		switch(arg) {
			case 'o':
				oflag = 1;
				ostring = optarg;
				break;		  
			case 'c':
				cflag = 1;
				break;
			case '?':		// If ?, invalid arg was used
				err = 1;
		}
	}

	// If invalid arg was detected, print usage and exit
	if (err == 1) {
		fprintf(stderr, usage);
		exit(1);
	}

	// Check for arguments after command-line args
	if (optind < argc) {  	// If this is true, the value argument is here 
		filename = argv[optind];
	}

	BST *tree;

	if (filename != NULL) {
		tree = read_from_file(filename, cflag);
		// no need to allocate memory for the rest of root's members; pointers
		// will be overwritten with allocated ones when insert is called.
	}
	else {
		tree = read_from_stdin(cflag);
	}

	// Validate -o argument. If out of range, throw error.
	if (oflag == 1 && ostring == NULL) {
		char *oerror = "%s: -o requires a valid filename argument\n";
		fprintf(stderr, oerror, argv[0]);
		fprintf(stderr, usage);
		exit(1);
	}

	if (oflag) {
		write_to_file(tree, ostring);
	}
	else {
		printf("%s", tostring_tree(tree)); // Bon appetit.
	}

	// We're done; free all of our resources.
	free_tree(tree);
	// All other strings are freed automatically by garbage collection: we're done.
}
