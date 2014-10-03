#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm> 
#include <list>
#include <ctime>
#include <sstream>
#include <fstream>
using namespace std;

struct vuelo
{
	int lugar_de_salida;
	int lugar_de_llegada;
	int hora_salida;
	int hora_llegada;
};

list<string> mapear(vector<string>& a_mapear){ //"mapeados"
	
	list<string> res_list;//lista porque apriori no conozco la cantidad total de ciudades distintas, por lo que con vector no seria lineal
	for (int i = 0; i < a_mapear.size(); i++)
	{
		bool esta = false;
		for(int j = 0; j < i; j++)
		{
			if(a_mapear[j]==a_mapear[i]) {esta = true;} //si la ciudad no esta la agrego
		}
		if (!esta) {res_list.push_back(a_mapear[i]);} //primero como lista 
	}
	return res_list;
}

vector<vuelo> construir_vuelos(list<string>& mapeados, vector<string>& a_mapear, vector<int>& horas){
	
	vector<vuelo> vuelos;

	vuelos.resize(horas.size()/2); // sobre dos porque la hora de salida y llegada de un mismo vuelo ocupan dos lugares
	int i, j, pos;
	for( i =0, j= 0, pos = 0; i < a_mapear.size(); i=i+2, j=j+2, pos++) //a_mapear[i] representa la ciudad de origen y a_mapear[i+1] la destino para un vuelo y
	// horas[j] es la hora de salida. horas[j+1] la hora de llegada para ese vuelo		
	{
		int salida, llegada;//las ciudades ahora como int
		int buscar = 0;
		for(std::list<string>::iterator it = mapeados.begin(); it != mapeados.end(); ++it, buscar++)
		{
			if(*it == a_mapear[i]) {salida = buscar;}
			if(*it == a_mapear[i+1]) {llegada = buscar;}
		}
		vuelo viaje;
		viaje.lugar_de_salida= salida;
		viaje.lugar_de_llegada= llegada;
		viaje.hora_salida= horas[j];
		viaje.hora_llegada= horas[j+1];
		vuelos[pos] = viaje; //guardo el viaje;
	}
	return vuelos;
}

bool por_llegada(const vuelo x,const vuelo y){return x.hora_llegada < y.hora_llegada;}

vector<list<int> > de_donde_salen(vector<vuelo>& vuelos, int cantidad_paises) //"donde"
{
	vector<list<int> > res;
	res.resize(cantidad_paises);
	for(int i = 0; i < vuelos.size(); i++)
	{
		res[vuelos[i].lugar_de_salida].push_back(i);//recorro el vector "vuelos" y agrego cada vuelo segun corresponda
	}
	return res;
}

bool No_Hay_Camino(list<std::pair<int, list<int> > >& soluciones)
{
	for(std::list<std::pair<int, list<int> > >::iterator it = soluciones.begin(); it != soluciones.end(); ++it)
	{
		if ((*it).first != -1) {return false;}//luego de ver todos los vuelos me fijo si hay alguno que llego a destino
	} // si todos son -1 entonces no hay camino.
	return true;
}

std::pair<int, list<int> > minimo_camino(list <std::pair<int, list<int> > >& soluciones)
{
	std::list<std::pair<int, list<int> > >::iterator it_comparador = soluciones.begin(); //se que hay por lo menos un vuelo que llego a "destino"
	for(std::list<std::pair<int, list<int> > >::iterator it = soluciones.begin(); it != soluciones.end(); ++it)
	{
		if((*it_comparador).first==-1) it_comparador = it;
		if((*it).first <= (*it_comparador).first && (*it).first !=-1)
		{
			it_comparador = it;//me guardo la posición donde la primer componente de la tupla es la menor y distinto a -1		
		} // esto significa que esa tupla llego a destino y me quedo con la hora de llegada menor
	}
	return *it_comparador; //devuelo la tupla minima
}

void mostrar(std::pair<int, list<int> > v){
	if (v.first == -1) cout << "no";// no hay solución si tiene -1	
	else
	{	
		cout << v.first << " " << v.second.size() << " ";
		for(std::list<int>::reverse_iterator it = v.second.rbegin();  it != v.second.rend(); ++it) {cout << *it +1 << " ";}
	}
	cout << endl;	
}
