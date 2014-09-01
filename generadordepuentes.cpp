#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
	srand(time(NULL));
	ofstream myfile;
	myfile.open ("tests2");
	for(int i = 1000000; i <= 30000000; i+=1000000)
	{
		int c = rand()%1000000;
		myfile <<i <<" "<< c<<" ";		//c random
		for(int k=0; k<i; k++)
		{
			int t = 0;
			if(c > 10000)
				t = 1;
			myfile << t << " ";
		}

		myfile<<"\n";
	}
	myfile<<"0";

	myfile.close();
}