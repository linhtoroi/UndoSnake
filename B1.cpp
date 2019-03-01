#include <iostream>
using namespace std;
int factorial(int x)
{
    if (x>1)
    {
        cout<< "x = " << x << " at "<< &x << endl;
        return x*factorial(x-1);
    }
    else
    {
        cout<< "x = " << x << " at " << &x << endl;
        return 1;
    }

}
int main()
{
    int N;
    cin>>N;
    cout<<factorial(N);
}
