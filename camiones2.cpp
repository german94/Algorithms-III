#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

void mostrar (vector<std::pair <vector<int>, int> >a);
bool agregar_elemento(std::pair<vector<int>,int>& actual, int umbral, int elemento_actual, int **peligrosidad, int cantidad_total);
bool recursion(int umbral, int desde,  int cantidad_total, int elemento_actual, int camiones_actuales, 	vector <std::pair <vector<int>,int> >& ubicaciones, int **eligrosidad);
int ordenar(int umbral, int cantidad,int **peligrosidad);
void sacar_producto(std::pair<vector<int>,int>& ubicacion, int elemento_actual, int **peligrosidad);

int main()
{
vector<pair<int**, pair<int, int> > > entradas;
	int umbral;
	int cant;
	string l;
	std::getline(cin, l);
	while(l.at(0) != '0')
	{
		cant = atoi(l.c_str());
		std::getline(cin, l);
		umbral = atoi(l.c_str());
		int **peligrosidad = new int *[cant];// creo una matriz con el coeficiente de peligrosidad entre pares de elementos donde el numero de 
		// fila y columna determinan que elementos analizo, la matriz va a ser de tamaña n.n, pero la mitad de sus elementos estan 
		// repetidos ya que la peligrosidad de 1 con 2, es la misma de 2 con 1. Asi quese podria achicar a un tamaño n.n/2
		// pero es mucho mas comodo tener la matriz completa.
		for(int i = 0; i <cant; i++)
	   	{	
	   		peligrosidad[i] = new int[cant];
		}
		int j = 0;
		while(j < cant -1)
		{
			std::getline(cin, l);
			istringstream ss(l);
			string token;
			int k = j + 1;
			while(std::getline(ss, token, ' ')) 
			{
				peligrosidad[j][k] = atoi(token.c_str());
				peligrosidad[k][j] = atoi(token.c_str());
			    k++;
			}
			j++;
		}
		pair<int**, pair<int, int> > entrada;
		entrada.first = peligrosidad;
		entrada.second.first = cant;
		entrada.second.second = umbral;
		entradas.push_back(entrada);
		cout<<"_________________________"<<endl;
		std::getline(cin, l);
	}
cout<<"Soluciones:"<<endl;
	for(int i=0; i<entradas.size(); i++)
	{
		ordenar(entradas[i].second.second, entradas[i].second.first, entradas[i].first);
		cout<<"_________________________"<<endl;
	}
	
    return 0;
}
	
void mostrar (vector<std::pair <vector<int>, int> >a)
{// PARA MOSTRAR EL RESULTADO
	int i =0;
	while (i < a.size())
	{
 		cout << "camión n°" << i+1 << " [";
 		int j = 0;
 		while(j < a[i].first.size() -1)
 		{
			cout << (a[i].first)[j] << ",";
		j++;
		}
		cout << a[i].first[j]<< "]" << endl;
	i++;
	}
} 

bool agregar_elemento(std::pair<vector<int>,int>& actual, int umbral, int elemento_actual, int **peligrosidad, int cantidad_total)
{ // SE FIJA SI SE PUEDE INGRESAR EL ELEMENTO EN EL CAMION ACTUAL
	//SUMANDO LOS COEFICIENTES, SI SE PUEDE SE INSERTA EL PRODUCTO
	// Y SE ACTUALIZA EL COEFICIENTE, SI NO NADA.
	int suma = actual.second; 
	for (int i =0; i < actual.first.size(); i++)
	{
		suma = suma + peligrosidad[actual.first[i]-1][elemento_actual -1];
		if (suma > umbral)
			return false;
	}
	actual.second = suma; // es la suma con el nuevo elemento
	actual.first.push_back(elemento_actual);
	return true;
}

 void sacar_producto(std::pair<vector<int>,int>& ubicacion, int elemento_actual, int **peligrosidad)
 { // SE RETIRA EL PRODUCTO DEL CAMION Y ADEMAS SE ACTUALIZA LA SUMA DE LOS COEFICIENTES
 	ubicacion.first.pop_back(); // retiro el elemento
 	for (int i = 0; i < ubicacion.first.size(); i++) 
 		{
 			ubicacion.second = ubicacion.second - peligrosidad[ubicacion.first[i] -1][elemento_actual-1];
 		}
 }

bool recursion(int umbral, int desde,  int cantidad_total, int elemento_actual, int camiones_actuales, 	vector <std::pair <vector<int>,int> >& ubicaciones ,int **peligrosidad)
{
	bool res = false;
	if (elemento_actual > cantidad_total) //termine de ubicarlos!
	{
		return true;
	}	
	else
	{
		int i = 0;
		while(i <= camiones_actuales)
		{
			if (agregar_elemento(ubicaciones[i], umbral, elemento_actual, peligrosidad, cantidad_total))
			{
				res = recursion(umbral, desde, cantidad_total, elemento_actual+1, camiones_actuales, ubicaciones, peligrosidad);	
				if (res) //si termine res esta en true
				{
					return res; // aviso que termine
				}
				else //debo quitar el elemento del lugar actual
				{
					sacar_producto(ubicaciones[i], elemento_actual, peligrosidad);
					desde = i;
				}	
			} // si no puedo agregar al elemento me fijo en el siguiente camion	
			i++;
		}	
		return res; //supuestamente si salio del while se me terminaron los camiones // y no logre acomdarlos 
	}	
}                     

int ordenar(int umbral, int cantidad, int **peligrosidad)
{ //ES LA FUNCION PRINCIPAL LA CUAL LLAMA A RECURSION , ESTA ES LA FUNCION QUE VA A PERMITIR 
	// CREAR UN NUEVO CAMION Y AVISAR A LOS DEMAS PRODUCTOS QUE SE CUENTA CON UN NUEVO CAMION 
	vector <std::pair <vector <int>,int> > res;                     
	bool termine = false;
	int elemento_actual = 1;
	int camiones_actuales = 0; //en realidad es uno solo que se indexa desde cero
	while(!termine) 
	{
		std::pair <vector <int>,int> agregar;
		agregar.first.push_back(1);
		agregar.second = 0; // solo hay un producto no hay peligrosidad
		res.push_back(agregar);
		termine = recursion(umbral, 0, cantidad, elemento_actual + 1, camiones_actuales, res, peligrosidad );
		if (termine)
		{
			mostrar(res);
			return camiones_actuales++;
		}
		else
		{
			res[camiones_actuales].first.pop_back();
			camiones_actuales++; // voy a agregar otro camion pero antes quito el	//producto 1 del camion anterior;
		}	
	}
}

