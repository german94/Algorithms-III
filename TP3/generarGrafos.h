
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <time.h>       /* time */
#include <sstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <vector>
#include <math.h>
#include <functional>
#include <iterator>


#include <cstdlib>
#include <sstream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void grafo_azar(int nodos, int maxArista,int particiones, string archivo);
void grafo_ConNAristas(int nodos, int maxArista,int aristas, int particiones,const char *archivo);	
void grafo_Completo(int nodos, int maxArista,int particiones,const char *archivo);
void circuito_Simple(int nodos, int maxArista,int particiones,const char *archivo);
void grafo_bipartito_completo(int N, int M, int maxArista, int particiones,const char *archivo);
void grafo_estrella(int n, int maxArista, int particiones,const char *archivo);
void grafo_corona(int N, int maxArista, int particiones,const char *archivo); //n>=2, es como un bipartito completo n,n 
	// pero la arista i, i+N no está. Para i de 1 a n.
