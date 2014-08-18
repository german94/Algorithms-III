#include <iostream>

using namespace std;

int seguidos(int tabla[], int cant) {
int maxi = 0;
int contador = 0;
for (int i = 1 ; i <= cant; i++)
{
    if (tabla[i] == 0)
    {
        if (contador >= maxi ) {maxi = contador;} else{}
    contador =0;
    }
    else {contador++;}
    }
return maxi;
}


int saltos(int cantidad, int desplazamiento, int tablones[]){

int cant = 0;

if (seguidos(tablones, cantidad) > desplazamiento) {cout << "error" << endl;}
else {
desplazamiento++;
int i = -1;
for (; i + desplazamiento < cantidad; )
{
    if (tablones[i + desplazamiento] != 1 )
    {
        i = i + desplazamiento;
        cout << "salto a: " << i << endl;
        cant++;
    }
    else
    {
        int j = i + desplazamiento;
        while( tablones[j] !=0)
        {
            j--;
        }
    i = j;
    cout << "salto a:" << i << endl;
    cant ++;
    }
}
cout << "ultimo salto a:" << i + desplazamiento<< endl;
cant++;
}

cout << cant << endl;
return cant;

return 0;
}



int main()
{
  /* int tam = 12;
   int tabla[12] = {0,1,1,1,0,0,1,1,1,0,1,1};
   seguidos(tabla, tam);
    saltos(12, 3, tabla);
    return 0;
    * */
int arr[] = { 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1 };
int c = 3;
int length = 13;
saltos(13,3,arr);
    return 0 ;
}
