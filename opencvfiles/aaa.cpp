
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
void expand(int value)
{
    const char * const ones[20] = {"zero", "one", "two", "three","four","five","six","seven",
    "eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
    "eighteen","nineteen"};
    const char * const tens[10] = {"", "ten", "twenty", "thirty","forty","fifty","sixty","seventy",
    "eighty","ninety"};

    if(value<0)
    {
        cout<<"minus ";
        expand(-value);
    }
    else if(value>=1000)
    {
        expand(value/1000);
        cout<<" thousand";
        if(value % 1000)
        {
            if(value % 1000 < 100)
            {
                cout << " and";
            }
            cout << " " ;
            expand(value % 1000);
        }
    }
    else if(value >= 100)
    {
        expand(value / 100);
        cout<<" hundred";
        if(value % 100)
        {
            cout << " and ";
            expand (value % 100);
        }
    }
    else if(value >= 20)
    {
        cout << tens[value / 10];
        if(value % 10)
        {
            cout << " ";
            expand(value % 10);
        }
    }
    else
    {
        cout<<ones[value];
    }
	cout<<" ";
    return;
}

void conversion( float d)
		{ 	int a=d,l=2;
			expand(a);
				d-=a;
				if(d!=0)
				cout<<" point ";
				while(l--)
				{
					d*=10;
					int c=d;
				        expand(c);
					d-=c;
                                }
                               cout << " centimeters" ;
		}

   int main()
{
   float d;
	  for(int i=0;i<5;i++)
	{
			    scanf("%f",&d);

			    freopen ("myfile.txt","w",stdout);
			    cout << "The depth is ";
			    conversion(d);
			    fclose (stdout);

			    system("say -f myfile.txt");
	}
   
  return 0;
}
