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


vector<vuelo> construir_vuelos(vector<string> mapeados, vector<string> a_mapear, vector<int> horas)
{
	vector<vuelo> vuelos;
	int i;
	int j;
	for( i =0, j= 0; i < a_mapear.size(); i=i+2, j=j+2 )
	{
		int salida;
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

vector<string> mapear(vector<string> a_mapear)
{
	vector<string> res;
	for (int i = 0; i < a_mapear.size(); i++)
	{
		bool esta = false;
		for(int j = 0; j < i; j++)
		{
			if(a_mapear[j]==a_mapear[i]) {esta = true;}
		}
		if (!esta) {res.push_back(a_mapear[i]);}
	}
	return res;
}

bool por_salida(const vuelo& v1,const vuelo& v2)
{
	return v1.hora_salida < v2.hora_salida;
}

vector<vector<int> > de_donde_salen(vector<vuelo> vuelos, int cantidad_paises)
{
	vector<vector<int> > res;
	for(int i = 0; i < cantidad_paises ; i++)
	{
		vector<int> posiciones;
		res.push_back(posiciones);
	}
	for(int i = 0; i < vuelos.size(); i++)
	{
		res[vuelos[i].lugar_de_salida].push_back(i);
	}
return res;
}

vector<vuelo> salida(vector<vuelo> vuelos, int inicio, int llegada, vector<vector<int> > donde);
std::pair<int, vector<vuelo> > Buscar_Llegada(vector<vuelo> vuelos, std::pair<int, vector<vuelo> > verifico[], vuelo salida, int posicion_de_salida, int llegada, vector<vector<int> > donde);

bool No_Hay_Camino(vector<std::pair<int, vector<vuelo> > > camino)
{
	for (int i = 0; i < camino.size(); i++)
	{
		if (camino[i].first != -1) {return false;}
	}
	return true;
}

std::pair<int, vector<vuelo> > minimo_camino(vector <std::pair<int, vector<vuelo> > > camino)
{
	int posicion = 0;
	for(int i = 0; i< camino.size(); i++)
	{
		if(camino[i].first <= camino[posicion].first && camino[i].first !=-1)
		{
			posicion = i;
		}
	}
	return camino[posicion];
}

void mostrar(vector<vuelo> v, vector<string> mapeados )
{
	for(int i = v.size() -1; i>=0; i--){
		vuelo a = v[i];
		cout << mapeados[a.lugar_de_salida] << " " << mapeados[a.lugar_de_llegada] << " " << a.hora_salida << " " << a.hora_llegada << endl;
		v.pop_back();
	}
}

int main()
{
vector<string> a_mapear;
vector<int> horas;
int cantidad;
string desde;
string hasta;
cin >> cantidad >> desde >> hasta;

for(int i = 0; i < cantidad; i++)
{
	string lu_salida;
	cin >> lu_salida;
	a_mapear.push_back(lu_salida);
	string lu_llegada;
	cin >> lu_llegada;
	a_mapear.push_back(lu_llegada);
	int ho_salida;
	cin >> ho_salida;
	horas.push_back(ho_salida);
	int ho_llegada;
	cin >> ho_llegada;
	horas.push_back(ho_llegada);
}

vector<string> mapeados =mapear(a_mapear);

vector<vuelo> vuelos = construir_vuelos(mapeados, a_mapear, horas);

std::sort(vuelos.begin(), vuelos.end(), por_salida);  

int cantidad_paises = mapeados.size(); 

vector<vector<int> > donde = de_donde_salen(vuelos, cantidad_paises);

int desde_mapeado;
int hasta_mapeado;
for(int i = 0; i < mapeados.size(); i++)
{
	if(mapeados[i] == desde) {desde_mapeado = i;}
	if(mapeados[i] == hasta) {hasta_mapeado = i;}
}

vector<vuelo> resul = salida(vuelos, desde_mapeado, hasta_mapeado, donde);
mostrar (resul, mapeados);
   return 0;
}

vector<vuelo> salida(vector<vuelo> vuelos, int inicio, int llegada, vector<vector<int> > donde)
{
	int n = vuelos.size();
	std::pair<int, vector<vuelo> > verifico[n]; //debo dejar los caminos prearmados tmb
	for (int i = 0; i <n ; i++ )
	{
		std::pair<int, vector<vuelo> > resul_parciales;
		//vector<vuelo> itinerario;
		resul_parciales.first = -2;
		//resul_parciales.second = itinerario;
		verifico[i] = resul_parciales ; //siginifica que no hice nada con ese vuelo
	}
	vector<int> posiciones = donde[inicio];
	if (posiciones.size() == 0)
	{
		 cout << "No hay solución"<< endl;
	}
	else
	{	
		vector <std::pair<int, vector<vuelo> > > soluciones; // la segunda componente 
		//contiene una lista de vuelos, la primera la hora de llegada
		for (int i = 0; i< posiciones.size(); i++)
		{
			std::pair<int, vector<vuelo> > s = Buscar_Llegada(vuelos, verifico, vuelos[posiciones[i]], posiciones[i], llegada, donde);
			soluciones.push_back(s);			
		}
		if (No_Hay_Camino(soluciones))
			{
				cout << "No hay solución" << endl;
			}
			else
			{
				vector<vuelo> minimo = minimo_camino(soluciones).second;
				return minimo;
			}
	}
}			

std::pair<int, vector<vuelo> > Buscar_Llegada(vector<vuelo> vuelos, std::pair<int, vector<vuelo> > verifico[], vuelo salida, int posicion_de_salida, int llegada, vector< vector<int> > donde)
{
	if (salida.lugar_de_llegada == llegada)
	{
		vector<vuelo> camino; //acá voy a tener la lista con todos los vuelos que me ayudan 
		//a llegar a B
		camino.push_back(salida); //salida es el ultimo vuelo que me hace llegar a B
		std::pair <int , vector<vuelo> > itinerario; // voy a poner la hora de llegada a B
		// y a empezar a construir el camino
		itinerario.first = salida.hora_llegada; //la hora que llego a B
		itinerario.second = camino; // los vuelos para llegar
		verifico[posicion_de_salida] = itinerario;
		return itinerario; //devuelvo la tupla
	}
	else 
	{
		if (verifico[posicion_de_salida].first != -2)
		{
			return verifico[posicion_de_salida]; 
		}
		else
		{
			vector<int> posiciones = donde[salida.lugar_de_llegada];
			if (posiciones.size() == 0)
			{ 
				vector<vuelo> camino; //acá va a estar vacio por que termine los vuelos
				//por este lado y no llegue a B
				std::pair <int , vector<vuelo> > itinerario; // no tengo ninguna hora de llegada es -1
				itinerario.first = -1; //la hora que llego a B
				itinerario.second = camino; // los vuelos para llegar
				verifico[posicion_de_salida] = itinerario; // no hay ningun camino no pongo nada en second
				return itinerario; //devuelvo la tupla
			}
			else
			{
				vector <std::pair<int, vector<vuelo> > > soluciones;
				for(int i =0; i< posiciones.size(); i++)
				{
					if(vuelos[posiciones[i]].hora_salida - salida.hora_llegada >= 2)
					{
						std::pair<int, vector<vuelo> >  s = Buscar_Llegada(vuelos, verifico, vuelos[posiciones[i]], posiciones[i], llegada, donde);
						soluciones.push_back(s);
					}	
				}
				if (No_Hay_Camino(soluciones))
				{
					std::pair <int, vector<vuelo> > itinerario; // no tengo ninguna hora de llegada es -1
					itinerario.first = -1; //la hora que llego a B
					verifico[posicion_de_salida]= itinerario;
					return itinerario; //d
				}
				else
				{
					std::pair<int, vector<vuelo> > minimo =  minimo_camino(soluciones);
					minimo.second.push_back(salida);
					verifico[posicion_de_salida] = minimo;
					return minimo;
				}	
			}	
		}	
	}	
}

