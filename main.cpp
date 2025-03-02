#include "Hash_Set.h"

int main() {
    HashSet<double> ht(10);
    ht.Add(10.00001);
    ht.Add(15.00001);
    ht.Add(20.00001);

    if (ht.Find(10.00001)) cout << "Yes" << endl;
    else cout << "No" << endl;

    if (ht.Find(101)) cout << "Yes" << endl;
    else cout << "No" << endl;

    ht.Remove(15.00001);

    if (ht.Find(15.00001)) cout << "Yes" << endl;
    else cout << "No" << endl;

}