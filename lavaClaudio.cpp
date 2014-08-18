#include <iostream>

using namespace std;

int main()
{
    int arr[] = { 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1 };
	int c = 3;
	int n = 13;

	int i=-1;
	int j=0;
	while(i< n){
		for(j=i+c;true;--j)
        {
        	if (arr[j]== 0) {i=j; cout<<i<<endl;break;}
        	if (j ==i+1 && arr[j]==1) {i=n;cout<<"sin solucion"<<endl;break;}
        	if (i + c > n) {i= n;break;}
        }
	}

    return 0;
}

// el primer if es si soy un tablón válido salto a ese y ahora estoy parado en ese j 
// el otro es si estoy en un escalon que saltando c escalones salgo afuera salgo del while y del break
// el segundo es si mi escalon uno más adelante de donde estoy no es válido y los otros que podría dar más adelanta tampoco lo son digo sin solución y salgo del for y el while