#include "input.h"

#define DEBUG 0

int recursao(Graph *G, int root, int parent, int m, int *bResult, int *wResult);
void calculaBprime(int m, int *b, int *w, int *bPrime);
void calculaWprime(int m, int *b, int *w, int *wPrime);
void calculaBprimePrime(int m, int *lastB, int *bPrime);
void calculaWprimePrime(int m, int *lastW, int *wPrime);
int soma(int a, int b);
int menor(int a, int b);     
void bLeaf(int m, int *bResult);
void wLeaf(int m, int *wResult);
void printVector(int *vect, int m);


 
/* M = M+1 */
int main (int argc, char *argv[])
{
    int m, *bResult, *wResult;
    int root = 0;
    Graph *G;

    
    m = atoi(argv[2]);
    G = read(argv[1]);
    bResult = malloc((m+1)*sizeof(int));
    wResult = malloc((m+1)*sizeof(int));


    recursao(G, root, 0, m+1, bResult, wResult);

    if(DEBUG)printVector(bResult, m+1);
    if(DEBUG)printVector(wResult, m+1);
    printf("%d\n", menor(bResult[m], wResult[m]));
    freeGraph(G);
    free(bResult);
    free(wResult);
    return 0;
}


int recursao(Graph *G, int root, int parent, int m, int *bResult, int *wResult)
{
    /* retorna -- */
    int k = 0; /* contador para filhos */
    int i;
    int *bPrime, *wPrime, *bPrimePrime, *wPrimePrime; 
    Vertex *v;
    if(DEBUG) printf("DESCEU %d\n", root);
    v = G->adj[root]->next;
 

    /* quando é folha, retorna valor das folhas */
    if(v == NULL || (v->vertex ==parent && v->next == NULL)) /* LEAFS */
    {
        if(DEBUG) printf(" FOLHA\n");
        bLeaf(m, bResult);
        wLeaf(m, wResult);
        return 0;
    }
    
    
    /* percorre todos os filhos */
    for (; v!=NULL; v = v->next)
    {
        if(parent != v->vertex)
        {
            /* reutilizando bwResult */
            recursao(G, v->vertex, root, m, bResult, wResult);
            bPrime = malloc(m*sizeof(int));
            wPrime = malloc(m*sizeof(int));
            

            if(DEBUG){
                printf("b[%d]:  ", v->vertex);
                printVector(bResult, m);
                printf("w[%d]:  ", v->vertex);
                printVector(wResult, m);
            }
            calculaBprime(m, bResult, wResult, bPrime);
            calculaWprime(m, bResult, wResult, wPrime);

            if(DEBUG){
                printf("b'[%d]:  ", v->vertex);
                printVector(bPrime, m);
                printf("w'[%d]:  ", v->vertex);
                printVector(wPrime, m);
            }

            /* primeiro filho */
            if(k == 0) 
            {
                bPrimePrime = malloc(m*sizeof(int));
                wPrimePrime = malloc(m*sizeof(int));   
            }
            for (i = 0; i < m && k == 0; i++)
            {
                bPrimePrime[i] = bPrime[i];
                wPrimePrime[i] = wPrime[i];
            }

            /* demais filhos */
            if( k>0)
            {
                calculaBprimePrime(m, bPrimePrime, bPrime);
                calculaWprimePrime(m, wPrimePrime, wPrime);
            }

            free(bPrime);
            free(wPrime);

            k++;
            if(DEBUG) printf("---------\n");

        }
    }
    for(i = 0; i<m; i++)
    {
        bResult[i] = bPrimePrime[i];
        wResult[i] = wPrimePrime[i];
    }
    free(bPrimePrime);
    free(wPrimePrime);
    return 0;
}

void calculaBprime(int m, int *b, int *w, int *bPrime)
{
    int i;
    bPrime[0] = -1;
    for(i = 1; i<m; i++)
    {   
        if(w[i-1] == -1)
            bPrime[i] = b[i-1];
        else if(w[i-1] + 1 <= b[i-1] || b[i-1]<0)
            bPrime[i] = w[i-1] + 1;
        else
            bPrime[i] = b[i-1];
    }
}

void calculaWprime(int m, int *b, int *w, int *wPrime)
{
    int i;
    
    for(i = 0; i<m; i++)
    {   
        if(b[i] == -1)
            wPrime[i] = w[i];
        else if(b[i] + 1 <= w[i] || w[i]<0)
            wPrime[i] = b[i] + 1;
        else
            wPrime[i] = w[i];
    }
}


void calculaBprimePrime(int m, int *lastB, int *bPrime)
{      
    int *newB = malloc(m*sizeof(int));
    int i, l;
    /* todos os m's */ 
    for(i = 0; i<m; i++)
    {
        newB[i] = -1;
        for(l = 1; l<=i; l++)
        {
            newB[i] = menor(newB[i], soma(lastB[l], bPrime[i-l+1]));
        }
    }
    for(i = 0; i<m; i++)
        lastB[i] = newB[i];
    free(newB);
}

void calculaWprimePrime(int m, int *lastW, int *wPrime)
{      
    int *newW = malloc(m*sizeof(int));
    int i, l;
    
    /* todos os m's */ 
    for(i = 0; i<m; i++)
    {
        newW[i] = soma(lastW[0], wPrime[i]);
        for(l = 1; l<=i; l++)
        {
            newW[i] = menor(newW[i], soma(lastW[l], wPrime[i-l]));
        }
    }
    
    for(i = 0; i<m; i++)
        lastW[i] = newW[i];
    free(newW);
}


int soma(int a, int b)
{
    if(a == -1 || b == -1)
    {
        return -1;
    }
    return a+b;
}

int menor(int a, int b)     
{
    if(a == -1) return b;
    if(b == -1) return a;
    if(a<=b) return a;
    return b;
}

void bLeaf(int m, int *bResult)
{
    int i;
    for (i = 0; i < m; i++)
        bResult[i] = -1;
    bResult[1] = 0;
}


void wLeaf(int m, int *wResult)
{
    int i;
    for(i = 1; i<m; i++)
        wResult[i] = -1;
    wResult[0] = 0;
}



void printVector(int *vect, int m)
{
    int i;
    for(i = 0; i<m; i++)
    {
        printf("[%d]%d, ", i, vect[i]);
    }
    printf("\n");
}