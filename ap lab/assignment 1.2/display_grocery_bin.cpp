#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class GroceryItem {
public:
    int id;
    string name;
    string category;
    float price;
    string expiry;

    void read(ifstream &ifs) {
        size_t len;
        ifs.read((char*)&id, sizeof(id));
        ifs.read((char*)&len, sizeof(len));
        name.resize(len);
        ifs.read(&name[0], len);
        ifs.read((char*)&len, sizeof(len));
        category.resize(len);
        ifs.read(&category[0], len);
        ifs.read((char*)&price, sizeof(price));
        ifs.read((char*)&len, sizeof(len));
        expiry.resize(len);
        ifs.read(&expiry[0], len);
    }
};

int main() {
    ifstream ifs("grocery.bin", ios::binary);
    if (!ifs) {
        cout << "Could not open grocery.bin for reading." << endl;
        return 1;
    }
    int n;
    ifs.read((char*)&n, sizeof(n));
    vector<GroceryItem> items;
    for (int i = 0; i < n; ++i) {
        GroceryItem item;
        item.read(ifs);
        items.push_back(item);
    }
    ifs.close();

    cout << "Grocery Items in grocery.bin:\n";
    for (const auto& item : items) {
        cout << "ID: " << item.id
             << ", Name: " << item.name
             << ", Category: " << item.category
             << ", Price: " << item.price
             << ", Expiry: " << item.expiry << endl;
    }
    return 0;
}
