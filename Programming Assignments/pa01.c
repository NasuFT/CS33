#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newline printf("\n")

/* structure used for reading tree input */
struct edge
{
	int name;
    int lson;
    int rson;
};
typedef struct edge Edge;

/* binary tree-related structures */
typedef int BinaryTreeDataType;
typedef struct binarytreenode BinaryTreeNode;
struct binarytreenode
{
    int LTAG;
    BinaryTreeNode *LSON;
    BinaryTreeDataType DATA;
    BinaryTreeNode *RSON;
    int RTAG;
};

/* binary tree-related operations */
void CREATE(BinaryTreeNode **);
int read_tree_input(Edge **, int **);
void POSTORDER_T (BinaryTreeNode *);
void PREORDER_T (BinaryTreeNode *);
void INORDER_T (BinaryTreeNode *);
BinaryTreeNode *INSUC (BinaryTreeNode *);
BinaryTreeNode *PRESUC (BinaryTreeNode *);
BinaryTreeNode *POSTSUC (BinaryTreeNode *);
void VISIT (BinaryTreeNode *);
void FREE_TREE (BinaryTreeNode *);

/* other functions */
int LOOK (int *, int, int);

int main()
{
    BinaryTreeNode *H;
    
    CREATE(&H);
    INORDER_T(H);
    newline;
    PREORDER_T(H);
    newline;
    POSTORDER_T (H);
    newline;
    
    FREE_TREE(H->LSON);
    free(H);
    return 0;
}

int read_tree_input (Edge **edges, int **inorder_list)
{
	int i, n;
    int lson, rson, name;
	
	scanf("%d\n", &n);
    *edges = (Edge *) malloc (n*sizeof(Edge));
    for (i = 0; i < n; i++)
    {
        (*edges)[i].name = i+1;
        
        scanf("%d %d\n", &lson, &rson);
        (*edges)[i].lson = lson;
        (*edges)[i].rson = rson;        
    }
    *inorder_list = (int *) malloc (n*sizeof(int));
    for (i = 0; i < n; i++)
    {
        scanf("-%d-", &name);
        (*inorder_list)[i] = name;
    }
	
	return n;
}

void CREATE (BinaryTreeNode **H)
{
	Edge *edges;
    int *inorder_list;
    BinaryTreeNode **nodes;
    BinaryTreeNode *head;
	int n;
	int i = 0, index;
	
	n = read_tree_input(&edges, &inorder_list);
    
    /**
    Head node of the threaded binary tree.
    **/
    head = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
    head->DATA = -1;
    head->RSON = head;
    head->RTAG = 1;
	
    nodes = (BinaryTreeNode **) malloc (n*sizeof(BinaryTreeNode*));
	for (i = 0; i < n; i++)
    {
        nodes[i] = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
        nodes[i]->DATA = edges[i].name;
        nodes[i]->LSON = NULL;
        nodes[i]->LTAG = 0;
        nodes[i]->RSON = NULL;
        nodes[i]->RTAG = 0;
    }
    
    head->LSON = nodes[0];
    head->LTAG = 1;
    
    /*
    Creates the regular and thread links
    */
    for (i = 0; i < n; i++)
    {
        
        if (edges[i].lson > 0) 
        {
            nodes[i]->LSON = nodes[edges[i].lson-1];
            nodes[i]->LTAG = 1;
        }
        else
        {
            index = LOOK (inorder_list, n, edges[i].name);
            if (index == 0) nodes[i]->LSON = head;
            else nodes[i]->LSON = nodes[inorder_list[index-1]-1];
        }
        
        if (edges[i].rson > 0) 
        {
            nodes[i]->RSON = nodes[edges[i].rson-1];
            nodes[i]->RTAG = 1;
        }
        else
        {
            index = LOOK (inorder_list, n, edges[i].name);
            if (index == n-1) nodes[i]->RSON = head;
            else nodes[i]->RSON = nodes[inorder_list[index+1]-1];
        }
        
    }
    
    *H = head;
    	
	free(edges);
    free(inorder_list);
}

void VISIT(BinaryTreeNode *T)
{
    printf("-%d-", T->DATA);
}

/**
Deallocating a Binary Tree should be
done in POSTORDER. Suggested is iterative
to avoid stack overflow.
**/
void FREE_TREE (BinaryTreeNode *T)
{
    if (T->LTAG == 1) FREE_TREE(T->LSON);
    if (T->RTAG == 1) FREE_TREE(T->RSON);
    free(T);
    
    return;
}

/*
Standard linear search
*/
int LOOK (int *list, int size, int key)
{
    int i;
    for (i = 0; i < size; i++)
        if (list[i] == key) return i;
    
    return -1;
}

// Traversals Onward

BinaryTreeNode *INSUC(BinaryTreeNode *alpha)
{
	if(alpha->RTAG == 1) {
        BinaryTreeNode* a = alpha->RSON;
        while(a->LTAG == 1) a = a->LSON;
        return a;
    } else {
        return alpha->RSON;
    }
}

void INORDER_T (BinaryTreeNode *H)
{
	BinaryTreeNode* a = H->LSON;
    while(a->LTAG == 1) a = a->LSON;
    while(a != H) {
        VISIT(a);
        a = INSUC(a);
    }
}

BinaryTreeNode *PRESUC (BinaryTreeNode *alpha)
{
	if(alpha->LTAG == 1) return alpha->LSON;
    else if(alpha->RTAG == 1) return alpha->RSON;
    else {
        BinaryTreeNode* a = alpha->RSON;
        while(a->RTAG == 0) a = a->RSON;
        return a->RSON;
    }
}

void PREORDER_T (BinaryTreeNode *H)
{
	BinaryTreeNode* a = H->LSON;
    while(a != H) {
        VISIT(a);
        a = PRESUC(a);
    }
}

BinaryTreeNode *FATHER (BinaryTreeNode *alpha) {
    BinaryTreeNode* a = alpha;
    while(a->LTAG == 1) a = a->LSON;
    a = a->LSON;
    if(a->RSON == alpha) return a;
    else {
        BinaryTreeNode* b = a;
        b = b->RSON;
        while(b != alpha) {
            a = b;
            b = b->LSON;
        }

        return a;
    }
}

BinaryTreeNode *POSTSUC (BinaryTreeNode *alpha)
{
    BinaryTreeNode* a = FATHER(alpha);
    if(alpha == a->LSON) {
        if(a->RTAG == 1 || alpha != a->LSON) {
            a = a->RSON;
            while(1) {
                if(a == a->RSON) break;
                else if(a->LTAG == 1) a = a->LSON;
                else if(a->RTAG == 1) a = a->RSON;
                else break;
            }
            return a;
        } else return a;
    } else return a;
}

void POSTORDER_T (BinaryTreeNode *H)
{
	BinaryTreeNode* a = H->LSON;
    while(1) {
        if(a->LTAG == 1) a = a->LSON;
        else if(a->RTAG == 1) a = a->RSON;
        else break;
    }
    while(a != H) {
        VISIT(a);
        a = POSTSUC(a);
    }
}