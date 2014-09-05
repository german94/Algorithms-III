#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

void mostrar_resultado (vector<std::pair <vector<int>, int> >a);
bool agregar_producto_a_un_camion(std::pair<vector<int>,int>& actual, int umbral, int producto_actual, int **peligrosidades, int cantidad_total);
bool ubicar_siguientes_productos(int umbral, int cantidad_total, int producto_actual, int camiones_actuales, 	vector <std::pair <vector<int>,int> >& camiones, int **eligrosidad);
int ubicar_producto(int umbral, int cantidad_total, int **peligrosidades);
void sacar_producto_de_un_camion(std::pair<vector<int>,int>& ubicacion, int producto_actual, int **peligrosidades);

void DarTiempo(double t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile <<t << "\n";
	myfile.close();
}

int main()
{
vector<pair<int**, pair<int, int> > > entradas;
	int umbral;
	int cant;
	string l;
	std::getline(cin, l);
	while(l.at(0) != '0')
	{
		istringstream sse(l);
		string tokenl;
		std::getline(sse, tokenl, ' ');
		cant = atoi(tokenl.c_str());
		std::getline(sse, tokenl, ' ');
		umbral = atoi(tokenl.c_str());
		int **peligrosidades = new int *[cant];// creo una matriz con el coeficiente de peligrosidades entre pares de elementos donde el numero de 
		// fila y columna determinan que elementos analizo, la matriz va a ser de tamaña n.n, pero la mitad de sus elementos estan 
		// repetidos ya que la peligrosidades de 1 con 2, es la misma de 2 con 1. Asi quese podria achicar a un tamaño n.n/2
		// pero es mucho mas comodo tener la matriz completa.
		for(int i = 0; i <cant; i++)
	   	{	
	   		peligrosidades[i] = new int[cant];
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
				peligrosidades[j][k] = atoi(token.c_str());
				peligrosidades[k][j] = atoi(token.c_str());
			    k++;
			}
			j++;
		}
		pair<int**, pair<int, int> > entrada;
		entrada.first = peligrosidades;
		entrada.second.first = cant;
		entrada.second.second = umbral;
		entradas.push_back(entrada);
		cout<<"_________________________"<<endl;
		std::getline(cin, l);
	}
cout<<"Soluciones:"<<endl;
	for(int i=0; i<entradas.size(); i++)
	{
		ubicar_producto(entradas[i].second.second, entradas[i].second.first, entradas[i].first);
		cout<<"_________________________"<<endl;
	}
	
    return 0;
}
	
void mostrar_resultado (vector<std::pair <vector<int>, int> > camion)
{// P camionR camion mostr camionr_result camiondo EL RESULT camionDO
	int i =0;
	while (i <  camion.size())
	{
 		cout << "camión n°" << i+1 << " [";
 		int j = 0;
 		while(j <  camion[i].first.size() -1)
 		{
			cout << ( camion[i].first)[j] << ",";
		j++;
		}
		cout <<  camion[i].first[j]<< "]" << endl;
	i++;
	}
} 

bool agregar_producto_a_un_camion(std::pair<vector<int>,int>& camion_actual, int umbral, int producto_actual, int **peligrosidades, int cantidad_total)
{ // SE FIJA SI SE PUEDE INGRESAR EL ELEMENTO EN EL CAMION camion_actual
	//SUMANDO LOS COEFICIENTES, SI SE PUEDE SE INSERTA EL PRODUCTO
	// Y SE camion_actualIZA EL COEFICIENTE, SI NO NADA.
	int suma = camion_actual.second; 
	for (int i =0; i < camion_actual.first.size(); i++)
	{
		suma = suma + peligrosidades[camion_actual.first[i]-1][producto_actual -1];
		if (suma > umbral)
			return false;
	}
	camion_actual.second = suma; // es la suma con el nuevo elemento
	camion_actual.first.push_back(producto_actual);
	return true;
}

 void sacar_producto_de_un_camion(std::pair<vector<int>,int>& camion_actual, int producto_actual, int **peligrosidades)
 { // SE RETIRA EL PRODUCTO DEL CAMION Y ADEMAS SE ACTUALIZA LA SUMA DE LOS COEFICIENTES
 	camion_actual.first.pop_back(); // retiro el elemento
 	for (int i = 0; i < camion_actual.first.size(); i++) 
 		{
 			camion_actual.second = camion_actual.second - peligrosidades[camion_actual.first[i] -1][producto_actual-1];
 		}
 }

bool ubicar_siguientes_productos(int umbral,  int cantidad_total, int producto_actual, int camiones_actuales, vector <std::pair <vector<int>,int> >& camiones ,int **peligrosidades)
{
	bool termine = false;
	if (producto_actual > cantidad_total) //termine de ubicarlos!
	{
		return true;
	}	
	else
	{
		int i = 0;
		while(i <= camiones_actuales)
		{
			if (agregar_producto_a_un_camion(camiones[i], umbral, producto_actual, peligrosidades, cantidad_total))
			{
				termine = ubicar_siguientes_productos(umbral, cantidad_total, producto_actual+1, camiones_actuales, camiones, peligrosidades);	
				if (termine) //si termine termine esta en true
				{
					return termine; // aviso que termine
				}
				else //debo quitar el elemento del lugar actual
				{
					sacar_producto_de_un_camion(camiones[i], producto_actual, peligrosidades);
				}	
			} // si no puedo agregar al elemento me fijo en el siguiente camion	
			i++;
		}	
		return termine; //supuestamente si salio del while se me terminaron los camiones // y no logre acomdarlos 
	}	
}                     

int ubicar_producto(int umbral, int cantidad_total, int **peligrosidades)
{ //ES LA FUNCION PRINCIPAL LA CUAL LLAMA A ubicar_siguientes_productos , ESTA ES LA FUNCION QUE VA A PERMITIR 
	// CREAR UN NUEVO CAMION Y AVISAR A LOS DEMAS PRODUCTOS QUE SE CUENTA CON UN NUEVO CAMION 
		clock_t begin = clock();

	vector <std::pair <vector <int>,int> > camiones;                     
	bool termine = false;
	int producto_actual = 1;
	int camiones_disponibles = 0; //en realidad es uno solo que se indexa  cero
	while(!termine) 
	{
		std::pair <vector <int>,int> camion;
		camion.first.push_back(1);
		camion.second = 0; // solo hay un producto no hay peligrosidades
		camiones.push_back(camion);
		termine = ubicar_siguientes_productos(umbral, cantidad_total, producto_actual + 1, camiones_disponibles, camiones, peligrosidades );
		if (termine)
		{
			mostrar_resultado(camiones);
			clock_t end = clock();
    		double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC); //en seg
  		    DarTiempo(elapsed_msecs);
			return camiones_disponibles++;
		}
		else
		{
			camiones[camiones_disponibles].first.pop_back();
			camiones_disponibles++; // voy a agregar otro camion pero antes quito el	//producto 1 del camion anterior;
		}	
	}
}

