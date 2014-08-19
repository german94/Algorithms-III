#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>

using namespace std;

void resolverInstancia(vector<int> p, int c)
{
	int length = p.size();

	if(c > length)
		cout<<length + 1;
	else
	{
		vector<int> s;
		int i = c + 1;
		while(i - 1 < length)
		{
			if(p[i - 1] == 0)
			{
				s.push_back(i);
				i = i + c + 1;
			}
			else
			{
				int j = i - 1;
				for(; j >= i - 1 - c; j--)
				{
					if(j == i - 1 - c && (p[j] == 1 || (j + c + 1 < length && p[j + c + 1] == 1)))
					{
						cout<<"no";
						return;
					}

					if(p[j] == 0)
					{
						i = j + 1;
						break;
					}
				}
			}
		}

	   s.push_back(length + 1);
	   vector<int>::const_iterator ci;
	   for(ci=s.begin(); ci!=s.end(); ci++)
	   		cout << *ci << " ";
	}
}

int main()
{
	vector<vector<int> > v;
	string l;
	getline(cin, l);
	while(l.at(0) != '0')
	{
		vector<int> vec;
		istringstream ss(l);
		string token;
		while(std::getline(ss, token, ' ')) 
		    vec.push_back(atoi(token.c_str()));

		v.push_back(vec);
		getline(cin, l);
	}

	cout<<"___________________________\n"<<"Soluciones: "<<endl;
	for(int i=0; i<v.size(); i++)
	{
		int c = v[i][1];
		v[i].erase(v[i].begin(), v[i].begin()+2);
		resolverInstancia(v[i], c);
		cout<<endl;
	}
}