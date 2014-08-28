#include <iostream>
#include <vector>
#include <string>
using namespace std;

void mostrar (vector<std::pair <vector<int>, int> >a);
bool agregar_elemento(std::pair<vector<int>,int>& actual, int umbral, int elemento_actual, int **peligrosidad, int cantidad_total);
bool recursion(int umbral, int cantidad_total, int elemento_actual, int camiones_actuales, 	vector <std::pair <vector<int>,int> >& ubicaciones, int **eligrosidad);
int ordenar(int umbral, int cantidad,int **peligrosidad);
void sacar_producto(std::pair<vector<int>,int>& ubicacion, int elemento_actual, int **peligrosidad);

int main()
{
	int umbral;
	int cant; 
	cin >> cant >> umbral;

	int **peligrosidad;
	peligrosidad = new int *[cant];
	for(int i = 0; i <cant; i++)
	{ 
   		peligrosidad[i] = new int[cant];
	}

	for(int fila = 0; fila < cant -1; fila++ )
	{
		int columna = fila+1;
		
		for (; columna < cant; columna++)
		{
			int valor;
			cin >> valor; 
			peligrosidad[fila][columna] = valor; 
			peligrosidad[columna][fila] = valor;
		}
	}
	ordenar(umbral, cant, peligrosidad);
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


bool recursion(int umbral, int cantidad_total, int elemento_actual, int camiones_actuales, 	vector <std::pair <vector<int>,int> >& ubicaciones ,int **peligrosidad)
{
	bool res = false;
	if (elemento_actual > cantidad_total) //termine de ubicarlos!
	{
		return true;
	}	
	else
	{
		int i =0;
		while(i <= camiones_actuales)
		{
			if (agregar_elemento(ubicaciones[i], umbral, elemento_actual, peligrosidad, cantidad_total))
			{
				int siguiente_elemento = elemento_actual + 1;
				res = recursion(umbral, cantidad_total, siguiente_elemento, camiones_actuales, ubicaciones, peligrosidad);	
				if (res) //si termine res esta en true
				{
					return res; // aviso que termine
				}
				else //debo quitar el elemento del lugar actual
				{
					sacar_producto(ubicaciones[i], elemento_actual, peligrosidad);
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
		int siguiente_elemento = elemento_actual + 1;
		termine = recursion(umbral, cantidad, siguiente_elemento, camiones_actuales, res, peligrosidad );
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

