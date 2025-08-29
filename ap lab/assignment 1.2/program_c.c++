#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define MAX_STACK 100

class GroceryItem {
public:
    int id;
    string name;
    string category;
    float price;
    string expiry;

    // For binary read/write
    void write(ofstream &ofs) const {
        ofs.write((char*)&id, sizeof(id));
        size_t len = name.size();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(name.c_str(), len);
        len = category.size();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(category.c_str(), len);
        ofs.write((char*)&price, sizeof(price));
        len = expiry.size();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(expiry.c_str(), len);
    }

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

class Stack {
public:
    vector<GroceryItem> items;

    void push(const GroceryItem& x) {
        if (items.size() < MAX_STACK)
            items.push_back(x);
    }

    GroceryItem pop() {
        if (!items.empty()) {
            GroceryItem x = items.back();
            items.pop_back();
            return x;
        }
        return GroceryItem();
    }

    bool isEmpty() const {
        return items.empty();
    }
};

void readFromFile(const string& filename, vector<GroceryItem>& arr) {
    ifstream ifs(filename, ios::binary);
    if (!ifs) return;
    int n;
    ifs.read((char*)&n, sizeof(n));
    for (int i = 0; i < n; ++i) {
        GroceryItem item;
        item.read(ifs);
        arr.push_back(item);
    }
}

int main() {
    vector<GroceryItem> arr;
    readFromFile("grocery.bin", arr);

    // Create stacks for each category
    map<string, Stack> stacks;
    for (const auto& item : arr) {
        stacks[item.category].push(item);
    }

    // Example: print and pop all items from each stack
    for (auto& pair : stacks) {
        cout << "Category: " << pair.first << endl;
        while (!pair.second.isEmpty()) {
            GroceryItem gi = pair.second.pop();
            cout << "ID: " << gi.id << ", Name: " << gi.name << ", Price: " << gi.price << ", Expiry: " << gi.expiry << endl;
        }
    }
    return 0;
}