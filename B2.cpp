#include <iostream>
using namespace std;
int main()
{
    int a[10];
    char b[10];
    for (int i=0; i<10; i++)
    {
        cout<<&a[i]<<" ";
    }
    cout<<endl;
    for (int i=0; i<10; i++)
    {
        cout<<&b[i]<<" ";
    }
}
