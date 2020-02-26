/**
For this code template, you may change the
function bodies as you see fit, except for
the main function.
**/

#include <stdio.h>
#include <stdlib.h>

#define newline printf("\n")

void UNION (int, int);
int FIND (int);
void EQUIVALENCE (void);
int TEST(int, int);
void TEST_GROUP (void);
int* INIT_ARRAY (int);

/**
QUIWA book suggested that the FATHER
array be declared as global for easier
implementation of the codes.
**/
// Declare global variables here
int *FATHER; //the global FATHER array
int SIZE; //the global FATHER array size
// Declare global variables here

int main()
{   
    EQUIVALENCE ();
    TEST_GROUP ();
    
    free (FATHER);
    return 0;
}

void EQUIVALENCE ()
{
    int i, j, n;
    
    // get size of S
    scanf("%d\n", &n);
    
    FATHER = INIT_ARRAY(n);
    SIZE = n;

    scanf("%d %d\n", &i, &j);
    while (i != 0 && j != 0)
    {
        /*
        i <- FIND(i)
        j <- FIND(j)
        if i != j then call UNION(i,j)
        */
        i = FIND(i);
        j = FIND(j);
        if(i != j) UNION(i, j);
        scanf("%d %d\n", &i, &j);
    }
    
    return;
}

int *INIT_ARRAY (int size)
{
    int* array = malloc(sizeof(int) * (size + 1));
    for(int i = 1; i <= size; ++i)
        array[i] = -1;
    return array;
}

int FIND(int i)
{
    /*
    r <- i
    while FATHER(r) > 0 do
        r <- FATHER(r)
    endwhile
    j <- i
    while j != r do
        k <- FATHER(j)
        FATHER(j) <- r
        j <- k
    endwhile
    return r
    */
    int r = i;
    while(FATHER[r] > 0)
        r = FATHER[r];
    int j = i;
    while(j != r) {
        int k = FATHER[j];
        FATHER[j] = r;
        j = k;
    }

    return r;
}

int abs (int x)
{
    if (x < 0)
        return -x;
    
    return x;
}

void UNION (int i, int j)
{
    /*
    count <- FATHER(i) + FATHER(j)
    if |FATHER(i)| > |FATHER(j)| then
        FATHER(j) <- i
        FATHER(i) <- count
    else
        FATHER(i) <- j
        FATHER(j) <- count
    */

    int count = FATHER[i] + FATHER[j];
    if(abs(i) > abs(j)) {
        FATHER[j] = i;
        FATHER[i] = count;
    } else {
        FATHER[i] = j;
        FATHER[j] = count;
    }
}

void TEST_GROUP()
{
    int k, l, result;
    
    // read elements of test set T
    scanf("%d %d\n", &k, &l);
    while (k != 0 && l != 0)
    {
        result = TEST(k, l);
        if (result)
            printf("TRUE\n");
        else
            printf("FALSE\n");
        scanf("%d %d\n", &k, &l);
    }
    
    return;
}

/*
Returns True (non-0) if k \equiv l, False (0) otherwise
*/
int TEST (int k, int l)
{
    /*
    k <- FIND(k)
    l <- FIND(l)
    if k == l then return true
              else return false
    */
    k = FIND(k);
    l = FIND(l);
    if(k == l) return 1;

    return 0;
}
