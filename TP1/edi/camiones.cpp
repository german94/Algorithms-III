#include <iostream>
#include <vector>
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

	int **peligrosidad;// creo una matriz con el coeficiente de peligrosidad entre pares de elementos donde el numero de 
	// fila y columna determinan que elementos analizo, la matriz va a ser de tamaña n.n, pero la mitad de sus elementos estan 
	// repetidos ya que la peligrosidad de 1 con 2, es la misma de 2 con 1. Asi quese podria achicar a un tamaño n.n/2
	// pero es mucho mas comodo tener la matriz completa.
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
			peligrosidad[fila][columna] = valor; //completo el valor tanto para la posicion ij como ji
			peligrosidad[columna][fila] = valor;
		}
	}
	ordenar(umbral, cant, peligrosidad);
    return 0;
}

void mostrar (vector<std::pair <vector<int>, int> >a)
{//tan solo muestra EL RESULTADO
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
	//SUMANDO LOS COEFICIENTES, SI SE PUEDE SE INSERTA EL PRODUCTO al "final" del camion
	// Y SE ACTUALIZA EL COEFICIENTE, SI NO, no se hace nada.
	//voy a tener un vector de tuplas <vector, suma_total_de_la_pelicgrosidad>; el vector mas externo representa
	// la cantidad de camiones, y los de cada tuplas los productos en cada iesimo camion
	int suma = actual.second; // a la suma que ya haya voy a agregarle la peligrosidad que pasa a tener si se 
	//agregara el nuevo elemento cada vez que se lo sumo a un producto de los que ya habia en el 
	//camion veo si es menor que el umbral
	for (int i =0; i < actual.first.size(); i++)
	{
		suma = suma + peligrosidad[actual.first[i]-1][elemento_actual -1];
		if (suma > umbral)
			return false;
	}
	actual.second = suma; // es la suma con el nuevo elemento
	actual.first.push_back(elemento_actual); //si la suma total dio menor que el umbral agrego el elemento
	return true;
}

 void sacar_producto(std::pair<vector<int>,int>& ubicacion, int elemento_actual, int **peligrosidad)
 { // SE RETIRA EL PRODUCTO DEL CAMION Y ADEMAS SE ACTUALIZA LA SUMA DE LOS COEFICIENTES (la segunda componente de la tupla)
 	ubicacion.first.pop_back(); // retiro el elemento
 	for (int i = 0; i < ubicacion.first.size(); i++) 
 		{
 			ubicacion.second = ubicacion.second - peligrosidad[ubicacion.first[i] -1][elemento_actual-1];
 		} // resto 1 porque se indexa desde el cero pero los productos comienzan a partir del 1
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
		int i =0; //cada vez que se llama a esta funcion se prueban agregando los productos desde el comienzo
		// pero camiones_actuales van a ir cambiando por la funcion ordenar que es la que crea si es 
		//necesario un nuevo producto
		while(i <= camiones_actuales)
		{
			if (agregar_elemento(ubicaciones[i], umbral, elemento_actual, peligrosidad, cantidad_total))
			{//si se pudo agregar el producto al camion actual se prueba con el siguiente; si no se pudo
			//se ve en el siguiente camion, (sigue este while)
				int siguiente_elemento = elemento_actual + 1;
				res = recursion(umbral, cantidad_total, siguiente_elemento, camiones_actuales, ubicaciones, peligrosidad);	
				if (res) //si termine res esta en true
				{
					return res; // aviso que termine
				}
				else //si no termine cambio de posicion a este producto, pero tengo que sacarlo del
				//camion actual
				{
					sacar_producto(ubicaciones[i], elemento_actual, peligrosidad);
				}	
			} // si no puedo agregar al elemento me fijo en el siguiente camion	
			i++;
		}	
		return res; //supuestamente si salio del while se me terminaron los camiones // y no logre acomodarlos 
	}	
}                     

int ordenar(int umbral, int cantidad, int **peligrosidad)
{ //ES LA FUNCION PRINCIPAL LA CUAL LLAMA A RECURSION , ESTA ES LA FUNCION QUE VA A PERMITIR 
	// CREAR UN NUEVO CAMION Y AVISAR A LOS DEMAS PRODUCTOS QUE SE CUENTA CON UN NUEVO CAMION 
	vector <std::pair <vector <int>,int> > res;    // creo el vector que va a almacenar los productos y las sumas parciales                 
	bool termine = false;
	int elemento_actual = 1; // primer elemento
	int camiones_actuales = 0; //en realidad es uno solo que se indexa desde cero
	while(!termine) 
	{
		std::pair <vector <int>,int> agregar;
		agregar.first.push_back(1); //creo la tupla que se agrega al vector de tupla y ubico al primer elemento
		agregar.second = 0; // solo hay un producto no hay peligrosidad
		res.push_back(agregar);
		int siguiente_elemento = elemento_actual + 1; //una vez ubicado el primero llamo a otra funcion
		//con el sgte elemento
		termine = recursion(umbral, cantidad, siguiente_elemento, camiones_actuales, res, peligrosidad );
		if (termine)
		{
			mostrar(res); //luego de que terminen las funciones recursivas si termine listo
			//sino termine las recursiones pero no me sirven la cantidad actuales de camion
			//por lo que en el else saco al producto 1 del camion actual para meterlo en otro siguente a 
			//crear.
			return camiones_actuales++;
		}
		else
		{//aca retiro al producto 1 , se ejecuta otra vez el while creando un nuevo camion.
			res[camiones_actuales].first.pop_back();
			camiones_actuales++; // voy a agregar otro camion pero antes quito el	//producto 1 del camion anterior;
		}	
	}
}

