#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm> 
using namespace std;

struct enlace
{
  int e1;
  int e2;
  int costo;  
};

int raiz(int x, int padre[]) {
    if(x == padre[x]){          //Si estoy en la raiz
        return x;                   //Retorno la raiz
    }
    else return raiz(padre[x], padre); //De otro modo busco el padre del vértice actual, hasta llegar a la raiz.
}

bool por_costo(const enlace& x,const enlace y){ return x.costo < y.costo;}

bool pertenece(enlace a_buscar, vector<enlace> generador_minimo)
{   
    for(int i = 0; i < generador_minimo.size(); i++)
    {
        if(generador_minimo[i].e1 == a_buscar.e1 && generador_minimo[i].e2 == a_buscar.e2 && generador_minimo[i].costo == a_buscar.costo)
        {
            return true;
        } 
    }
    return false;    
}

vector<int> buscar_ciclo(vector<vector<int> > grafo, int nodo_anterior, int nodo_actual, int llegada)
{
    if(nodo_actual == llegada)
    {
        vector<int> camino; //llegue! 
        camino.push_back(llegada);
    }
    else
    {
        ////////////////////////////////
    }     
}

void mostrar(vector<enlace> result, int cantidad_equipos)
{
    int costo = 0;
    for (int i = 0; i < result.size(); i++)
    {
        costo = costo + result[i].costo;
    }    
    //////////////////////////////////
    vector<vector<int> > adyacencias;
    for(int i = 0; i < cantidad_equipos.size() ; i++)
    {
        vector<int> posicion; 
        adyacencias.push_back(posicion);
    }

    for(int i = 0; i < result.size() -1; i++)
    {
        adyacencias[grafo[i].e1 -1].push_back(e2); //guardo las adyacencias si bien van a ver repetidos creo que no superaria las 2.n aristas
        adyacencias[grafo[i].e2 -1].push_back(e1); //
    } 
    vector<int> ciclo;
    int inicio =  result[result.size() -1].e1;
    int llegada = result[result.size() -1].e2;
    vector<int> adyacente = adyacencias(inicio-1) //estaria agarrando la posicion de adyacencias "empieza" el ciclo, el -1 porque se indexa desde cero
    for(int =0; i < adyacente.size(); i++)
    {     
        ciclo =buscar_ciclo(adyacencias, inicio, adyacente[i] ,llegada);
        if (ciclo.size !=0) {ciclo.push_back(inicio); break for;} // ya tengo el camino
    }
}

vector<enlace> anillo(int cantidad_equipos, int cantidad_enlaces, vector<enlace> grafo){

    std::sort (grafo.begin(), grafo.end(), por_costo);//ordeno por menor costo

    int padre[cantidad_equipos]; // para cada nodo voy a poner quien es su raiz
    for(int i = 0; i< cantidad_equipos; i++){
        padre[i]= i; //inicialmente cada nodo es su padre
    }

    vector<enlace> generador_minimo;
    for(int i = 0; i < grafo.size(); i++)   //algoritmo de  Kruskal
    {
        int vertice_a = grafo[i].e1;
        int vertice_b = grafo[i].e2;
        int raiz_de_a = raiz(vertice_a -1, padre);
        int raiz_de_b = raiz(vertice_b -1, padre);

        if(raiz_de_a != raiz_de_b) 
        {
            padre[raiz_de_a] = raiz_de_b; 
            generador_minimo.push_back(grafo[i]);
        }
    } //ya tengo el generador minimo;

    enlace menor_peso;  
    menor_peso.costo = -1; //busco la arista de menor peso para formar el ciclo
    for(int i = 0; i < grafo.size(); i++)
    {
        if(!pertenece(grafo[i], generador_minimo) && (menor_peso.costo==-1 || menor_peso.costo > grafo[i].costo))
        {
            menor_peso.e1 = grafo[i].e1;
            menor_peso.e2 = grafo[i].e2;
            menor_peso.costo = grafo[i].costo;
        }    
    } 

    if(menor_peso.costo != -1) {generador_minimo.push_back(menor_peso);} // no se si puede pasar de no tener que agregar una arista

    return generador_minimo;
}    
     
int main(){
    int cantidad_equipos;
    int cantidad_enlaces;
    cin >> cantidad_equipos >> cantidad_enlaces;

    vector<enlace> grafo;
    for (int i= 0; i < cantidad_enlaces; i++){
        enlace a;
        int e_1, e_2, costo_i;
        cin >> e_1 >> e_2 >> costo_i;
        a.e1 = e_1;
        a.e2 = e_2;
        a.costo = costo_i;
        grafo.push_back(a);
    }
    vector<enlace> result = anillo(cantidad_equipos, cantidad_enlaces, grafo);  
    mostrar (result, cantidad_equipos);
    return  0;
}
