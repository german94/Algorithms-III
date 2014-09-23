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
  bool operator ==(const enlace& otra) const {
        return this->e1 == otra.e1 && this->e2 == otra.e2 && this->costo == otra.costo;
    }
};

int raiz(int x, int padre[]) {
    if(x == padre[x]) {return x;}          //Si estoy en la raiz
    else {return raiz(padre[x], padre);} //De otro modo busco el padre del vértice actual, hasta llegar a la raiz.
}

bool por_costo(const enlace& x,const enlace y){return x.costo < y.costo;}

bool por_nodo(const enlace& x,const enlace y){
    if(x.e1 == y.e1) {return x.e2 < y.e2;}
    else {return x.e1 < y.e1;}    
}

bool pertenece(enlace a_buscar, vector<enlace> generador_minimo, int desde, int hasta){   
   if(desde == hasta) {return a_buscar == generador_minimo[desde];}
   else
   { 
        int medio = (int)(desde+hasta)/2;
        if(generador_minimo[medio] == a_buscar) {return true;}
        else
        {
            if(generador_minimo[medio].e1 < a_buscar.e1) {return pertenece(a_buscar, generador_minimo, medio+1, hasta);} 
            else
            {    
                if (generador_minimo[medio].e1 > a_buscar.e1) {return pertenece(a_buscar, generador_minimo, desde, medio);}    
                else
                {
                    if (generador_minimo[medio].e2 <= a_buscar.e2) {return pertenece(a_buscar, generador_minimo, medio+1, hasta);} 
                    else {return pertenece(a_buscar, generador_minimo, desde, medio); }
                }    
            } 
        } 
    }
}

vector<enlace> buscar_hasta(vector<vector<std::pair <int, enlace> > > adyacencias, int desde, std::pair<int, enlace> adyacente ,int llegada ){

    vector<enlace> result;
    if(adyacente.first == llegada) {result.push_back(adyacente.second); return result;}
    else
    {
        vector<std::pair <int, enlace> >  siguiente_adyacente = adyacencias[adyacente.first -1];     
        for(int i=0; i < siguiente_adyacente.size(); i++)
        {     
            if(siguiente_adyacente[i].first != desde)
            {    
                result = buscar_hasta(adyacencias, adyacente.first ,siguiente_adyacente[i] ,llegada);
                if (result.size() !=0) {result.push_back(adyacente.second); return result;} // ya tengo el camino
            }
        }
    }    
    return result;
}

vector<enlace> buscar_ciclo(vector<enlace> generador_minimo, int cantidad_equipos, int desde, int hasta){
    vector<vector<std::pair <int, enlace> > > adyacencias; //segun la posicion tiene un vector que representa el elemento al que es adyacente y ese enlace
    for(int i = 0; i < cantidad_equipos ; i++)
    {
        vector<std::pair <int, enlace> > posicion; 
        adyacencias.push_back(posicion); //inicializo adyacencias
    }
    for(int i = 0; i < generador_minimo.size() ; i++)
    {
        std::pair<int, enlace> para_e1; 
        std::pair<int, enlace> para_e2;
        para_e1.first = generador_minimo[i].e2; //completo las adyacencias;
        para_e2.first = generador_minimo[i].e1;
        para_e1.second = generador_minimo[i]; //completo con su tupla;
        para_e2.second = generador_minimo[i];
        adyacencias[generador_minimo[i].e1 -1].push_back(para_e1); //guardo las adyacencias si bien van a ver repetidos creo que no superaria las 2.n aristas
        adyacencias[generador_minimo[i].e2 -1].push_back(para_e2); //
    } 
    vector<enlace> ciclo;
    vector<std::pair<int, enlace> > adyacente = adyacencias[desde -1]; //estaria agarrando la posicion de adyacencias "empieza" el ciclo, el -1 porque se indexa desde cero
    for(int i =0; i < adyacente.size(); i++)
    {     
        ciclo = buscar_hasta(adyacencias, desde, adyacente[i] ,hasta);
        if (ciclo.size() !=0) {break;} // ya tengo el camino
    }     
    return ciclo;
}

void mostrar( std::pair<vector<enlace>, vector<enlace> > result) {
    int costo = 0;
    for (int i = 0; i < result.first.size(); i++)
    {
        costo = costo + result.first[i].costo;
    } 
    for (int i = 0; i < result.second.size(); i++)
    {
        costo = costo + result.second[i].costo;
    } 
    cout << costo << " " << result.first.size() << " " << result.second.size() << endl;  
    cout << endl;
    for (int i = 0; i < result.first.size(); i++)
    {
        cout << result.first[i].e1 << " " << result.first[i].e2 << endl;
    } 
    cout << endl;
    for (int i = 0; i < result.second.size(); i++)
    {
        cout << result.second[i].e1 << " " << result.second[i].e2 << endl;
    } 
}

 std::pair<vector<enlace>, vector<enlace> > anillo(int cantidad_equipos, int cantidad_enlaces, vector<enlace> grafo){

    std::sort (grafo.begin(), grafo.end(), por_costo);//ordeno por menor costo
    
    int padre[cantidad_equipos]; // para cada nodo voy a poner quien es su raiz
    for(int i = 0; i< cantidad_equipos; i++) {padre[i]= i;} //inicialmente cada nodo es su padre
    
    vector<enlace> generador_minimo;
    for(int i = 0, aristas = 0;  aristas < cantidad_equipos -1; i++)   //algoritmo de  Kruskal
    {
        int vertice_a = grafo[i].e1;
        int vertice_b = grafo[i].e2;
        int raiz_de_a = raiz(vertice_a -1, padre);
        int raiz_de_b = raiz(vertice_b -1, padre);

        if(raiz_de_a != raiz_de_b) 
        {
            padre[raiz_de_a] = raiz_de_b; 
            generador_minimo.push_back(grafo[i]);
            aristas++;
        }
    } //ya tengo el generador minimo;//SUPUESTAMENTE ESTO NO ES N.N.N

    enlace menor_peso;  
    menor_peso.costo = -1; //busco la arista de menor peso para formar el ciclo
    std::sort (generador_minimo.begin(), generador_minimo.end(), por_nodo); //para hacer busqueda binaria
    for(int i = 0; i < grafo.size(); i++)
    {
        if(!pertenece(grafo[i], generador_minimo, 0, generador_minimo.size()-1) && (menor_peso.costo==-1 || menor_peso.costo > grafo[i].costo))
        {
            menor_peso.e1 = grafo[i].e1;
            menor_peso.e2 = grafo[i].e2;
            menor_peso.costo = grafo[i].costo;
        }    
    } 
   
    vector<enlace> ciclo = buscar_ciclo(generador_minimo, cantidad_equipos, menor_peso.e1, menor_peso.e2);
 
    vector<enlace> sin_el_ciclo;
    std::sort (ciclo.begin(), ciclo.end(), por_nodo); //para hacer busqueda binaria
    for(int i = 0; i < generador_minimo.size(); i++)
    {
        if(!pertenece(generador_minimo[i], ciclo, 0, ciclo.size()-1)) { sin_el_ciclo.push_back(generador_minimo[i]);}    
    } 
    ciclo.push_back(menor_peso); // agrego el que forma el ciclo
    std::pair<vector<enlace>, vector<enlace> > resultado;
    resultado.first = ciclo;
    resultado.second = sin_el_ciclo;    
    return resultado;
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
    std::pair<vector<enlace>, vector<enlace> > result = anillo(cantidad_equipos, cantidad_enlaces, grafo);  
    mostrar (result);
    return  0;
}