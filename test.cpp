#include <iostream>
#include <cstdlib>
using namespace std;

class Kok {
private:
    int razm;
    int* arr;
public:
    Kok() {
        arr = new int[razm];
        razm = 2;
        for(int i = 0; i < razm; i++)
            arr[i] = i+1;
    }

    Kok(int razm) {
        arr = new int[razm];
        this->razm = razm;
        for(int i = 0; i < razm; i++)
            arr[i] = i+1;
    }

    void Print() {
        cout << endl;
        for(int i = 0; i < razm; i++)
            cout << arr[i] << " ";
    }

    ~Kok() {
        delete[] arr;    
    }
};

int main()
{
    srand(time(NULL));
    Kok std;
    std.Print();
    cout << endl;
    Kok hand(rand()%10+1);
    hand.Print();
    return 0;
}
