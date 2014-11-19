#include "generarGrafos.h"


void grafo_azar(int nodos, int maxArista,int particiones, const char *archivo) { 
	srand(time(NULL));
	float **pesos = new float *[nodos+1];
	for(int i = 0; i < nodos+1; i++)  {pesos[i] = new float[nodos+1];}
	for (int i = 0; i < nodos+1; ++i)
	{
		for (int j = 0; j < nodos+1; ++j)
		{
			pesos[i][j]=0;
		}
	}

	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = 1; j < nodos+1; ++j)
		{	
			// srand (time(NULL) +i +j);
			int aristaCon0 = rand()%2;
			if (pesos[i][j]==0 && aristaCon0 != 0)
			{
				// srand (time(NULL)+j+i);
				float random = ((float) rand()) / (float) RAND_MAX;
				random = random*(float)maxArista;
				pesos[i][j]= random;			
				pesos[j][i]= random;			

			}
		}
	}
	int aristas=0;
	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				aristas++;
		}
	}

	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<nodos<<" ";
  	myfile<<aristas<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				{
					myfile<<i<<" "<<j<<" "<<pesos[i][j];
					myfile<<"\n";
				}
		}
	}
  	myfile.close(); 
	for(int i = 0; i < nodos; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);
}

void grafo_ConNAristas(int nodos, int maxArista,int aristas, int particiones,const char *archivo) { 
	srand (time(NULL) );
	float **pesos = new float *[nodos+1];
	for(int i = 0; i < nodos+1; i++)  {pesos[i] = new float[nodos+1];}
	for (int i = 0; i < nodos+1; ++i)
	{
		for (int j = 0; j < nodos+1; ++j)
		{
			pesos[i][j]=0;
			pesos[j][i]=0;
		}
	}
	int cantAristas=0;
	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = 1; j < nodos+1; ++j)
		{	
			// srand (time(NULL) +i +j);
			if (pesos[i][j]==0  && cantAristas <= aristas+1) // no se porque tengo que poner el +1 para que la cantidad de aristas y nodos sean los mismos en el grafo generado
 			{	
				cantAristas ++;
				// srand (time(NULL)+j+i);
				float random = ((float) rand()) / (float) RAND_MAX;
				random = random*(float)maxArista;
				pesos[i][j]= random;			
				pesos[j][i]= random;			

			}
		}
	}
	int numeroDearistas=0;
	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				numeroDearistas++;
		}
	}

	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<nodos<<" ";
  	myfile<<numeroDearistas<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				{
					myfile<<i<<" "<<j<<" "<<pesos[i][j];
					myfile<<"\n";
				}
		}
	}
  	myfile.close(); 
	for(int i = 0; i < nodos; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);
}

void grafo_Completo(int nodos, int maxArista,int particiones,const char *archivo) { // tipo 1
	srand (time(NULL));
	float **pesos = new float *[nodos+1];
	for(int i = 0; i < nodos+1; i++)  {pesos[i] = new float[nodos+1];}
	for (int i = 0; i < nodos+1; ++i)
	{
		for (int j = 0; j < nodos+1; ++j)
		{
			pesos[i][j]=0;
		}
	}

	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = 1; j < nodos+1; ++j)
		{	
			if (pesos[i][j]==0)
			{
				float random = ((float) rand()) / (float) RAND_MAX;
				random = random*(float)maxArista;
				pesos[i][j]= random;			
				pesos[j][i]= random;			

			}
		}
	}
	int aristas=0;
	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				aristas++;
		}
	}

	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<nodos<<" ";
  	myfile<<aristas<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	for (int i = 1; i < nodos+1; ++i)
	{
		for (int j = i+1; j < nodos+1; ++j)
		{
			if (pesos[i][j] != 0)
				{
					myfile<<i<<" "<<j<<" "<<pesos[i][j];
					myfile<<"\n";
				}
		}
	}
  	myfile.close(); 
	for(int i = 0; i < nodos; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);
}
void circuito_Simple(int nodos, int maxArista,int particiones,const char *archivo) { 
	srand (time(NULL));
	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<nodos<<" ";
  	myfile<<nodos<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	int i = 1;
  	for (i=1; i < nodos; ++i)
  	{	
		float random = ((float) rand()) / (float) RAND_MAX;
		random = random*(float)maxArista;
		myfile<<i<<" "<<i+1<<" "<<random;
		myfile<<"\n";
	}
	float random = ((float) rand()) / (float) RAND_MAX;
	random = random*(float)maxArista;
	myfile<<nodos<<" "<<1<<" "<<random;
	myfile<<"\n";
  	myfile.close(); 
}

void grafo_bipartito_completo(int N, int M, int maxArista, int particiones,const char *archivo) { // tipo 3
	srand (time(NULL));
	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<N+M<<" ";
  	myfile<<N*M<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	for (int n = 1; n < N+1; ++n)
	{
		for (int m = 1; m < M+1; ++m)
		{
			float random = ((float) rand()) / (float) RAND_MAX;
			random = random*(float)maxArista;
			myfile<<n<<" "<<N+m<<" "<<random;
			myfile<<"\n";
		}
	}
  	myfile.close(); 
}

void grafo_corona(int N, int maxArista, int particiones,const char *archivo) { //n>=2, es como un bipartito completo n,n 
	// pero la arista i, i+N no está. Para i de 1 a n.
	srand (time(NULL));
	ofstream myfile;
  	myfile.open (archivo, ios::app);
  	myfile<<N+N<<" ";
  	myfile<<N*(N-1)<<" ";
  	myfile<<particiones;
  	myfile<<"\n";
  	int m = 1;
  	int n = 1;

	for (n=1; n < N+1; ++n) 
	{
		for (m = 1; m < N+1; ++m) 
		{
			if(n==m) 
			{
				continue;
			}
			float random = ((float) rand()) / (float) RAND_MAX;
			random = random*(float)maxArista;
			myfile<<n<<" "<<N+m<<" "<<random;
			myfile<<"\n";
		}
	}
  	myfile.close(); 
}

void grafo_estrella(int n, int maxArista, int particiones,const char *archivo) { // tipo 6
	grafo_bipartito_completo(1, n, maxArista, particiones,archivo);
}

int main(int argc, char const *argv[])
{	
	srand (time(NULL));
/*	grafoConNAristas:
	for (int i = 1; i <= 10; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_ConNAristas(i,50,i,k, "grafoA-100.txt");
	}
		ofstream myfile;
		myfile.open ("grafoA-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();

//grafo_azar:
	for (int i = 10; i <=20 ; ++i)
	{	int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		ofstream myfile;
		myfile.open ("grafoA-100.txt", ios::app);
  		myfile<<i<<" "<<k<< "\n";
  		myfile.close();
		// grafo_azar(i,50,k, "grafoA-100.txt");
		
	}*/


// GENERADOR:

	// grafo_azar:
	for (int i = 1; i <= 100; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_azar(i,50,k, "grafoAzarK10MaxAris50Rang1-100.txt");
	}
		ofstream myfile;
		myfile.open ("grafoAzarK10MaxAris50Rang1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();


	// grafoConNAristas:
	for (int i = 1; i <= 100; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_ConNAristas(i,50,i,k, "grafoNaristask10Maxaris50RAng1-100.txt");
	}
		myfile;
		myfile.open ("grafoNaristask10Maxaris50RAng1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();

	// grafoCompleto
	for (int i = 1; i <= 100; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_Completo(i,50,k, "grafoCompletok10Maxaris50RAng1-100.txt");
	}
		myfile;
		myfile.open ("grafoCompletok10Maxaris50RAng1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();
  	

	//circuito simple
	for (int i = 1; i <= 100; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		circuito_Simple(i,50,k, "circuitoSimplek10Maxaris50RAng1-100.txt");
	}
		myfile;
		myfile.open ("circuitoSimplek10Maxaris50RAng1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();


	// bipartito completo
	for (int i = 1; i <= 50; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_bipartito_completo(i,i,50,k, "bipartitoNNCompletok10Maxaris50RAng1-100.txt");
	}
		myfile;
		myfile.open ("bipartitoNNCompletok10Maxaris50RAng1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();


	// estrella
	for (int i = 1; i <= 100; ++i)
	{
		int k=1; // porque sino sale error en float.
		if(i!=1)
			k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_estrella(i,50,k, "estrellak10Maxaris50RAng1-100.txt");
	}
		myfile;
		myfile.open ("estrellak10Maxaris50RAng1-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();

	//corona
	for (int i = 2; i <= 50; ++i)
	{
			int k=1; // porque sino sale error en float.
			if(i!=1)
				k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
		grafo_corona(i,50,k, "coronak10Maxaris50RAng2-100.txt");
	}
		myfile;
		myfile.open ("coronak10Maxaris50RAng2-100.txt", ios::app);
  		myfile<<"0"<< "\n";
  		myfile.close();



	return 0;
}


	//cambiando el k
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	grafo_azar(i,50,20, "grafoAzarK20MaxAris50Rang1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("grafoAzarK20MaxAris50Rang1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();

	// // grafoConNAristas:
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	grafo_ConNAristas(i,50,i,20, "grafoNaristask20Maxaris50RAng1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("grafoNaristask20Maxaris50RAng1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();
	// // grafoCompleto
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	grafo_Completo(i,50,20, "grafoCompletok20Maxaris50RAng1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("grafoCompletok20Maxaris50RAng1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();
	// //circuito simple
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	circuito_Simple(i,50,20, "circuitoSimplek20Maxaris50RAng1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("circuitoSimplek20Maxaris50RAng1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();
	// // bipartito completo
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	grafo_bipartito_completo(i,i,50,20, "bipartitoNNCompletok20Maxaris50RAng1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("bipartitoNNCompletok20Maxaris50RAng1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();
	// // estrella
	// for (int i = 1; i <= 100; ++i)
	// {
	// 	int k=1; // porque sino sale error en float.
	// 	if(i!=1)
	// 		k = (rand() % (int)(i-1)) +2; // random entre 2 y n.
	// 	grafo_estrella(i,50,20, "estrellak20Maxaris50RAng1-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("estrellak20Maxaris50RAng1-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();
	// //corona
	// for (int i = 2; i <= 100; ++i)
	// {
	// 	grafo_corona(i,50,20, "coronak20Maxaris50RAng2-100.txt");
	// }
	// 	myfile;
	// 	myfile.open ("coronak20Maxaris50RAng2-100.txt", ios::app);
 //  		myfile<<"0"<< "\n";
 //  		myfile.close();

