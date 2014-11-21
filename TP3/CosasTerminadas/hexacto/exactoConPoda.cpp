#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
using namespace std;

bool agregar_vertice(vector<int> &conjunto_actual, float &suma_actual, int particiones, int vertice_actual, float **pesos, int vertices, float suma_solucion);
void ubicar_siguientes_vertices( vector <vector <int> > &solucion, int vertices, int aristas, int particiones, int vertice_actual, int conjuntos_disponibles, std::pair <float, vector <vector <int> > > &k_particion, float **pesos, float &suma_solucion);
void ubicar_vertice( int vertices, int aristas, int particiones, float **pesos);
void sacar_vertice(vector<int> &ubicacion, float &suma_actual,  int vertice_actual, float **pesos);

void DarTiempo(double t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile <<t << "\n";
	myfile.close();
}

void mostrar(int n, const vector <vector<int> > &solucion)
{	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < solucion.size(); j++)
		{
			for(int k = 0; k < solucion[j].size(); k++)
			{
				if(solucion[j][k] == i+1)
					cout<<j+1<<" ";
			}
		}
	}
	cout<<endl;
}

float costo_de_agregarlo(int vertice_actual, vector<int> &conjunto_actual, float **pesos){
	if(conjunto_actual.size() == 0) {return 0.0;}
	else{
		float res = 0.0;	
		for(int i = 0; i < conjunto_actual.size(); i++){
			res = res + pesos[conjunto_actual[i]-1][vertice_actual -1];
		}
		return res;
	}
}

void poda (float &suma_solucion, float **pesos, int vertices, int particiones, vector< vector<int> > &solucion){
 //la poda se encarga de meter una arista mayormente donde no forme adyacencias(hasta el momento) y sino
//donde el costo de agregarla sea minima	
	for(int j = 0; j < particiones; j++) {vector<int> conjunto; solucion.push_back(conjunto);} //inicializo
	for(int i = 0; i < vertices; i++){
		std::pair<int, float> donde;
		donde.first = -1; //inicializo  
		float cuanto;
		for(int j = 0; j < particiones; j++){
			cuanto = costo_de_agregarlo(i+1, solucion[j], pesos);				
			if (cuanto == 0.0){
				donde.first = j;
				donde.second = cuanto;
				break;
			}
			else{
				if(donde.first == -1 || cuanto < donde.second){
					donde.first = j;
					donde.second = cuanto;
				}  
			} 
		}
		solucion[donde.first].push_back(i+1);
		suma_solucion = suma_solucion + donde.second;
	}
}


int main()
{
	string l;
	std::getline(cin, l);
	while(l.at(0) != '0')
	{	
		istringstream ss(l);
		string token;
		int vertices, aristas, particiones;
		std::getline(ss, token, ' ');
		vertices = atoi(token.c_str());
		std::getline(ss, token, ' ');
		aristas = atoi(token.c_str());
		std::getline(ss, token, ' ');
		particiones = atoi(token.c_str());
				
		float **pesos = new float *[vertices];
		for(int i = 0; i < vertices; i++)  {pesos[i] = new float[vertices];}
		for (int i = 0; i < vertices; ++i)
		{
			for (int j = 0; j < vertices; ++j)
			{
				pesos[i][j]=0;
				pesos[j][i]=0;
			}
		}
		for(int i=0; i < aristas; i++)
		{
			std::getline(cin, l);
			int vertice_a;
			int vertice_b;
			float peso; 
			istringstream ssl(l);
			string token1;
			std::getline(ssl, token1, ' ');
			vertice_a = atoi(token1.c_str());
			std::getline(ssl, token1, ' ');
			vertice_b = atoi(token1.c_str());
			std::getline(ssl, token1, ' ');
			peso = atoi(token1.c_str());
			pesos[vertice_a -1][ vertice_b -1] = peso;
			pesos[vertice_b -1][ vertice_a -1] = peso;
		} 
			ubicar_vertice( vertices, aristas, particiones, pesos);
			for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
				free (pesos);
			std::getline(cin, l);
	}		
    return 0;
}

bool agregar_vertice(vector<int> &conjunto_actual, float &suma_actual, int particiones, int vertice_actual, float **pesos, int vertices, float suma_solucion){ 
	float suma = suma_actual; //VA A RECORRER EL CONJUNTO ACTUAL Y VER SI EL AGREGAR UN VERTICE SUPERA a suma_solucion"
	// SINO SUPERA SE AGREGA EL VERTICE AL ELEMENTO ACTUAL
	//Y SE ACTUALIZA SUMA_SOLUCION. SINO NO SE HACE NADA
	for (int i =0; i < conjunto_actual.size(); i++){
		suma = suma + pesos[conjunto_actual[i]-1][vertice_actual -1];
		if (suma > suma_solucion) {return false;}
	}
	suma_actual = suma; // es la suma con el nuevo elemento
	conjunto_actual.push_back(vertice_actual);
	return true;
}

 void sacar_vertice(vector<int> &conjunto_actual, float &suma_actual, int vertice_actual, float **pesos){ // SE RETIRA EL PRODUCTO DEL conjunto Y ADEMAS SE ACTUALIZA LA SUMA DE LOS COEFICIENTES
 	conjunto_actual.pop_back(); // retiro el elemento, Y ACTUALIZO LA SUMA ACTUAL
 	for (int i = 0; i < conjunto_actual.size(); i++) {
 		suma_actual = suma_actual - pesos[conjunto_actual[i] -1][vertice_actual-1];
 	}
 }

void ubicar_siguientes_vertices(vector <vector <int> > &solucion, int vertices, int aristas, int particiones, int vertice_actual, int conjuntos_disponibles, std::pair<float, vector <vector <int> > > &k_particion, float **pesos, float &suma_solucion){
	//ES LA RECURSIVA QUE SE ENCARGA DE AGREGAR LOS SGTES VERTICES, RETROCEDER, ETC
	for (int i = 0; i <= conjuntos_disponibles; i++){
		if (suma_solucion == 0.0) {break;}
		if (agregar_vertice(k_particion.second[i], k_particion.first, particiones, vertice_actual, pesos, vertices, suma_solucion)){
			if(vertice_actual == vertices){ //RECIEN CUANDO ES EL ULTIMO VERTICE ACTUALIZO
				//DE SER NECESARIO SUMA_SOLUCION Y EL VECTOR SOLUCION
				suma_solucion = k_particion.first;
				solucion = k_particion.second;
			}
			else{
				ubicar_siguientes_vertices(solucion, vertices, aristas, particiones, vertice_actual + 1, conjuntos_disponibles, k_particion, pesos, suma_solucion);
			}//SINO ES EL ULTIMO SIGO ITERANDO
			sacar_vertice(k_particion.second[i], k_particion.first, vertice_actual, pesos);
		}	
	} 
}	

void ubicar_vertice(int vertices, int aristas, int particiones, float **pesos)
{ 
		clock_t begin = clock();

	std::pair <float, vector <vector <int> > > k_particion;
	//K_PARTICION ES A LA QUE SE LE VAN A IR AGREGANDO CONJUNTOS A MEDIDA QUE SE ITERA
	//Y EN LA PRIMER COMPONENTE VA A TENER LA SUMA TOTAL DE LA COMBINACION DE LA SEGUNDA
	//COMPONENTE
	int vertice_actual = 1;
	float suma_solucion = 0.0; // VA A TENER LA SUMA MINIMA HASTA EL MOMENTO
	vector <vector <int> > solucion; //ACÁ SE VAN A SOBREESCRIBIR LAS SOLUCIONES 

	poda(suma_solucion, pesos, vertices, particiones, solucion);
		vector <int> conjunto;
		k_particion.second.push_back(conjunto);
		for (int conjuntos_disponibles = 1; conjuntos_disponibles < particiones; conjuntos_disponibles++ ){	
			//ACA SE VAN A IR AGREGANDO MAS CONJUNTOS A MEDIDAD QUE HAGA FALTA
			if (suma_solucion == 0.0) {break;}// cero es lo minimo no tiene sentido continuar analizando
			vector <int> conjunto;
			conjunto.push_back(1);
			k_particion.second.push_back(conjunto);
			k_particion.first = 0.0; //la suma total es cero volvi a empezar
			ubicar_siguientes_vertices(solucion, vertices, aristas, particiones, vertice_actual + 1, conjuntos_disponibles, k_particion, pesos, suma_solucion );
			k_particion.second[conjuntos_disponibles].pop_back();// retiro el primer elemento 
		}	
	clock_t end = clock();
   	double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) *1000; //en seg
    DarTiempo(elapsed_msecs);
	mostrar(vertices, solucion);
}