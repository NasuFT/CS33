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
    int BF;
    BinaryTreeNode* LSON;
    BinaryTreeDataType KEY;
    BinaryTreeDataType DATA;
    BinaryTreeNode* RSON;
    BinaryTreeNode* FATHER;
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

void AVL_INSERT(BinaryTreeNode** H, BinaryTreeDataType K, BinaryTreeDataType d);
void ROTATE_LEFT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** beta);
void ROTATE_RIGHT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** beta);
void ROTATE_LEFT_RIGHT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** p);
void ROTATE_RIGHT_LEFT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** p);

int main()
{
    BinaryTreeNode* H = NULL;

    int x;
    scanf("%d", &x);
    while(x != 0) {
        AVL_INSERT(&H, x, x);
        PREORDER_T(H);
        newline;
        scanf("%d", &x);
    }
    
    return 0;
}

// int read_tree_input (Edge **edges, int **inorder_list)
// {
// 	int i, n;
//     int lson, rson, name;
	
// 	scanf("%d\n", &n);
//     *edges = (Edge *) malloc (n*sizeof(Edge));
//     for (i = 0; i < n; i++)
//     {
//         (*edges)[i].name = i+1;
        
//         scanf("%d %d\n", &lson, &rson);
//         (*edges)[i].lson = lson;
//         (*edges)[i].rson = rson;        
//     }
//     *inorder_list = (int *) malloc (n*sizeof(int));
//     for (i = 0; i < n; i++)
//     {
//         scanf("-%d-", &name);
//         (*inorder_list)[i] = name;
//     }
	
// 	return n;
// }

void AVL_INSERT(BinaryTreeNode **H, BinaryTreeDataType K, BinaryTreeDataType d) {
    if(*H == NULL) {
        BinaryTreeNode* v = malloc(sizeof(BinaryTreeNode));
        v->KEY = K;
        v->DATA = d;
        v->LSON = NULL;
        v->RSON = NULL;
        v->BF = 0;
        v->FATHER = NULL;
        *H = v;
        return;
    }

    BinaryTreeNode* v = NULL;
    BinaryTreeNode* alpha = *H;
    BinaryTreeNode* gamma = *H;
    BinaryTreeNode* omega = NULL;
    BinaryTreeNode* tau = NULL;
    while(1) {
        if(K == gamma->KEY) {
            return;
        } else if(K < gamma->KEY) {
            tau = gamma->LSON;
            if(tau == NULL) {
                v = malloc(sizeof(BinaryTreeNode));
                gamma->LSON = v;
                v->FATHER = gamma;
                break;
            }
        } else if(K > gamma->KEY) {
            tau = gamma->RSON;
            if(tau == NULL) {
                v = malloc(sizeof(BinaryTreeNode));
                gamma->RSON = v;
                v->FATHER = gamma;
                break;
            }
        }

        if(tau->BF != 0) {
            alpha = tau;
            alpha->FATHER = gamma;
        }
        gamma = tau;
    }

    v->KEY = K;
    v->DATA = d;
    v->LSON = NULL;
    v->RSON = NULL;
    v->BF = 0;

    BinaryTreeNode* beta = NULL;
    if(K < alpha->KEY) {
        beta = alpha->LSON;
        gamma = alpha->LSON;
    } else {
        beta = alpha->RSON;
        gamma = alpha->RSON;
    }

    while(gamma != v) {
        if(K < gamma->KEY) {
            gamma->BF = -1;
            gamma = gamma->LSON;
        } else {
            gamma->BF = 1;
            gamma = gamma->RSON;
        }
    }

    int w;
    if(K < alpha->KEY)
        w = -1;
    else 
        w = 1;
    
    BinaryTreeNode* p = NULL;
    if(alpha->BF == 0)
        alpha->BF = w;
    else if(alpha->BF == -w) {
        alpha->BF = 0;
    } else {
        if(K < alpha->KEY && beta->BF == -w)
            ROTATE_LEFT_RIGHT(H, alpha, &p);
        else if(K < alpha->KEY && beta->BF == w)
            ROTATE_RIGHT(H, alpha, &p);
        else if(K > alpha->KEY && beta->BF == w)
            ROTATE_LEFT(H, alpha, &p);
        else
            ROTATE_RIGHT_LEFT(H, alpha, &p);

        if(alpha == *H)
            *H = p;
        else if(alpha == alpha->FATHER->LSON)
            alpha->FATHER->LSON = p;
        else
            alpha->FATHER->RSON = p;
    }

    return;
}

void ROTATE_RIGHT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** beta) {
    *beta = alpha->LSON;
    alpha->LSON = (*beta)->RSON;
    (*beta)->RSON = alpha;
    alpha->BF = 0;
    (*beta)->BF = 0;
}

void ROTATE_LEFT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** beta) {
    *beta = alpha->RSON;
    alpha->RSON = (*beta)->LSON;
    (*beta)->LSON = alpha;
    alpha->BF = 0;
    (*beta)->BF = 0;
}

void ROTATE_LEFT_RIGHT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** p) {
    BinaryTreeNode* beta = alpha->LSON;
    *p = beta->RSON;
    beta->RSON = (*p)->LSON;
    (*p)->LSON = beta;
    alpha->LSON = (*p)->RSON;
    (*p)->RSON = alpha;
    if((*p)->BF == 0) {
        alpha->BF = 0;
        beta->BF = 0;
    } else if((*p)->BF == 1) {
        alpha->BF = 0;
        beta->BF = -1;
    } else {
        alpha->BF = 1;
        beta->BF = 0;
    }
    (*p)->BF = 0;
}

void ROTATE_RIGHT_LEFT(BinaryTreeNode** B, BinaryTreeNode* alpha, BinaryTreeNode** p) {
    BinaryTreeNode* beta = alpha->RSON;
    *p = beta->LSON;
    beta->LSON = (*p)->RSON;
    (*p)->RSON = beta;
    alpha->RSON = (*p)->LSON;
    (*p)->LSON = alpha;
    if((*p)->BF == 0){
        alpha->BF = 0;
        beta->BF = 0;
    } else if((*p)->BF == 1) {
        alpha->BF = -1;
        beta->BF = 0;
    } else {
        alpha->BF = 0;
        beta->BF = 1;
    }
    (*p)->BF = 0;
}

// void CREATE (BinaryTreeNode **H)
// {
// 	Edge *edges;
//     int *inorder_list;
//     BinaryTreeNode **nodes;
//     BinaryTreeNode *head;
// 	int n;
// 	int i = 0, index;
	
// 	n = read_tree_input(&edges, &inorder_list);
    
//     /**
//     Head node of the threaded binary tree.
//     **/
//     head = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
//     head->DATA = -1;
//     head->RSON = head;
//     head->RTAG = 1;
	
//     nodes = (BinaryTreeNode **) malloc (n*sizeof(BinaryTreeNode*));
// 	for (i = 0; i < n; i++)
//     {
//         nodes[i] = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
//         nodes[i]->DATA = edges[i].name;
//         nodes[i]->LSON = NULL;
//         nodes[i]->LTAG = 0;
//         nodes[i]->RSON = NULL;
//         nodes[i]->RTAG = 0;
//     }
    
//     head->LSON = nodes[0];
//     head->LTAG = 1;
    
//     /*
//     Creates the regular and thread links
//     */
//     for (i = 0; i < n; i++)
//     {
        
//         if (edges[i].lson > 0) 
//         {
//             nodes[i]->LSON = nodes[edges[i].lson-1];
//             nodes[i]->LTAG = 1;
//         }
//         else
//         {
//             index = LOOK (inorder_list, n, edges[i].name);
//             if (index == 0) nodes[i]->LSON = head;
//             else nodes[i]->LSON = nodes[inorder_list[index-1]-1];
//         }
        
//         if (edges[i].rson > 0) 
//         {
//             nodes[i]->RSON = nodes[edges[i].rson-1];
//             nodes[i]->RTAG = 1;
//         }
//         else
//         {
//             index = LOOK (inorder_list, n, edges[i].name);
//             if (index == n-1) nodes[i]->RSON = head;
//             else nodes[i]->RSON = nodes[inorder_list[index+1]-1];
//         }
        
//     }
    
//     *H = head;
    	
// 	free(edges);
//     free(inorder_list);
// }

void VISIT(BinaryTreeNode *T)
{
    printf("-%d-", T->DATA);
}

void PREORDER_T(BinaryTreeNode *T) {
    if(T != NULL) {
        VISIT(T);
        PREORDER_T(T->LSON);
        PREORDER_T(T->RSON);
    }
    return;
}

/**
Deallocating a Binary Tree should be
done in POSTORDER. Suggested is iterative
to avoid stack overflow.
**/
// void FREE_TREE (BinaryTreeNode *T)
// {
//     if (T->LTAG == 1) FREE_TREE(T->LSON);
//     if (T->RTAG == 1) FREE_TREE(T->RSON);
//     free(T);
    
//     return;
// }

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

// BinaryTreeNode *INSUC(BinaryTreeNode *alpha)
// {
// 	if(alpha->RTAG == 1) {
//         BinaryTreeNode* a = alpha->RSON;
//         while(a->LTAG == 1) a = a->LSON;
//         return a;
//     } else {
//         return alpha->RSON;
//     }
// }

// void INORDER_T (BinaryTreeNode *H)
// {
// 	BinaryTreeNode* a = H->LSON;
//     while(a->LTAG == 1) a = a->LSON;
//     while(a != H) {
//         VISIT(a);
//         a = INSUC(a);
//     }
// }

// BinaryTreeNode *PRESUC (BinaryTreeNode *alpha)
// {
// 	if(alpha->LTAG == 1) return alpha->LSON;
//     else if(alpha->RTAG == 1) return alpha->RSON;
//     else {
//         BinaryTreeNode* a = alpha->RSON;
//         while(a->RTAG == 0) a = a->RSON;
//         return a->RSON;
//     }
// }

// void PREORDER_T (BinaryTreeNode *H)
// {
// 	BinaryTreeNode* a = H->LSON;
//     while(a != H) {
//         VISIT(a);
//         a = PRESUC(a);
//     }
// }

// BinaryTreeNode *FATHER (BinaryTreeNode *alpha) {
//     BinaryTreeNode* a = alpha;
//     while(a->LTAG == 1) a = a->LSON;
//     a = a->LSON;
//     if(a->RSON == alpha) return a;
//     else {
//         BinaryTreeNode* b = a;
//         b = b->RSON;
//         while(b != alpha) {
//             a = b;
//             b = b->LSON;
//         }

//         return a;
//     }
// }

// BinaryTreeNode *POSTSUC (BinaryTreeNode *alpha)
// {
//     BinaryTreeNode* a = FATHER(alpha);
//     if(alpha == a->LSON) {
//         if(a->RTAG == 1 || alpha != a->LSON) {
//             a = a->RSON;
//             while(1) {
//                 if(a == a->RSON) break;
//                 else if(a->LTAG == 1) a = a->LSON;
//                 else if(a->RTAG == 1) a = a->RSON;
//                 else break;
//             }
//             return a;
//         } else return a;
//     } else return a;
// }

// void POSTORDER_T (BinaryTreeNode *H)
// {
// 	BinaryTreeNode* a = H->LSON;
//     while(1) {
//         if(a->LTAG == 1) a = a->LSON;
//         else if(a->RTAG == 1) a = a->RSON;
//         else break;
//     }
//     while(a != H) {
//         VISIT(a);
//         a = POSTSUC(a);
//     }
// }