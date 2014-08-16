#include <iostream>

using namespace std;

int main()
{
	int arr[] = { 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1 };
	int c = 3;
	int length = 13;

	if(c > length)
		cout<<"un salto"<<endl;
	else
	{
		int i = c + 1;
		while(i - 1 < length)
		{
			if(arr[i - 1] == 0)
			{
				cout<<"saltar a "<<i<<" - ";
				i = i + c + 1;
			}
			else
			{
				int j = i - 1;
				for(; j >= i - 1 - c; j--)
				{
					if(arr[j] == 0)
					{
						if(j == i - 1 - c && arr[j + c + 1] == 1)
						{
							cout<<"sin solucion"<<endl;
							return 1;
						}

						i = j + 1;
						break;
					}
				}
			}
		}

		cout<<"saltar a "<<length<< " - ";
	}

	cout<<endl;
}