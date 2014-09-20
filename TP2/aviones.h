#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <algorithm> 
using namespace std;

struct vuelo
{
	int lugar_de_salida;
	int lugar_de_llegada;
	int hora_salida;
	int hora_llegada;
};

vector<string> mapear(vector<string> a_mapear) //"mapeados"
{
	vector<string> res;
	for (int i = 0; i < a_mapear.size(); i++)
	{
		bool esta = false;
		for(int j = 0; j < i; j++)
		{
			if(a_mapear[j]==a_mapear[i]) {esta = true;} //si la ciudad no esta la agrego
		}
		if (!esta) {res.push_back(a_mapear[i]);}
	}
	return res;
}

vector<vuelo> construir_vuelos(vector<string> mapeados, vector<string> a_mapear, vector<int> horas)
{
	vector<vuelo> vuelos;
	int i;
	int j;
	for( i =0, j= 0; i < a_mapear.size(); i=i+2, j=j+2 ) //a_mapear[i] representa la ciudad de origen y a_mapear[i+1] la destino para un vuelo y
	// horas[j] es la hora de salida. horas[j+1] la hora de llegada para ese vuelo		
	{
		int salida;//las ciudades ahora como int
		int llegada;
		for(int buscar = 0; buscar < mapeados.size(); buscar++)
		{
			if(mapeados[buscar] == a_mapear[i]) {salida = buscar;}
			if(mapeados[buscar] == a_mapear[i+1]) {llegada = buscar;}
		}
		vuelo viaje;
		viaje.lugar_de_salida= salida;
		viaje.lugar_de_llegada= llegada;
		viaje.hora_salida= horas[j];
		viaje.hora_llegada= horas[j+1];
		vuelos.push_back(viaje);
	}
	return vuelos;
}

vector<vector<int> > de_donde_salen(vector<vuelo> vuelos, int cantidad_paises) //"donde"
{
	vector<vector<int> > res;
	for(int i = 0; i < cantidad_paises ; i++)
	{
		vector<int> posiciones;
		res.push_back(posiciones);//cada posición de res reprenta a una ciudad mapeada y el vector los vuelos donde esa ciudad, es una ciudad de salida en "vuelos"
	}
	for(int i = 0; i < vuelos.size(); i++)
	{
		res[vuelos[i].lugar_de_salida].push_back(i);//recorro el vector "vuelos" y agrego cada vuelo segun corresponda
	}
	return res;
}

bool No_Hay_Camino(vector<std::pair<int, vector<int> > > soluciones)
{
	for (int i = 0; i < soluciones.size(); i++)
	{
		if (soluciones[i].first != -1) {return false;}//luego de ver todos los vuelos me fijo si hay alguno que llego a destino
	} // si todos son -1 entonces no hay camino.
	return true;
}

std::pair<int, vector<int> > minimo_camino(vector <std::pair<int, vector<int> > > soluciones)
{
	int posicion = 0; //se que hay por lo menos un vuelo que llego a "destino"
	for(int i = 0; i< soluciones.size(); i++)
	{
		if(soluciones[i].first <= soluciones[posicion].first && soluciones[i].first !=-1)
		{
			posicion = i;//me guardo la posición donde la primer componente de la tupla es la menor y distinto a -1		
		} // esto significa que esa tupla llego a destino y me quedo con la hora de llegada menor
	}
	return soluciones[posicion]; //devuelo la tupla minima
}

void mostrar(std::pair<int, vector<int> > v)
{
	if (v.first == -1) cout << "no" << endl;// no hay solución si tiene -1	
	else
	{	
		cout << v.first << " " << v.second.size() << endl;  //la hora de llegada y la cantidad de vuelos usados
		for(int i = v.second.size() -1; i>=0; i--)
		{
			cout << v.second[i] +1 << " " << endl; //el +1 es porque en v estan las posiciones de los vuelos segun el vector "vuelos"
			//y como se identa desde cero y los vuelos desde 1 sumo 1		
			v.second.pop_back();
		}
	}	
}