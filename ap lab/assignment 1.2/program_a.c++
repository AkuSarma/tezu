#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define MAX_LIST 100

class GroceryItem {
public:
    int id;
    string name;
    string category;
    float price;
    string expiry;

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

class GroceryList {
public:
    vector<GroceryItem> items;

    void insert(const GroceryItem& x, int p) {
        if (items.size() >= MAX_LIST) return;
        items.insert(items.begin() + p, x);
    }

    void remove(int p) {
        if (items.empty() || p >= items.size()) return;
        items.erase(items.begin() + p);
    }

    int search(int id) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].id == id) return i;
        }
        return -1;
    }

    void print() {
        for (const auto& item : items) {
            cout << "ID: " << item.id << ", Name: " << item.name
                 << ", Category: " << item.category << ", Price: " << item.price
                 << ", Expiry: " << item.expiry << endl;
        }
    }

    GroceryItem retrieve(int p) {
        if (p < 0 || p >= items.size()) return GroceryItem();
        return items[p];
    }

    bool isEmpty() {
        return items.empty();
    }

    void makeNull() {
        items.clear();
    }

    void writeToFile(const string& filename) {
        ofstream ofs(filename, ios::binary);
        if (!ofs) return;
        int n = items.size();
        ofs.write((char*)&n, sizeof(n));
        for (const auto& item : items) item.write(ofs);
        ofs.close();
    }
};

int main() {
    GroceryList L;
    L.makeNull();
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Insert item\n";
        cout << "2. Delete item\n";
        cout << "3. Search item by ID\n";
        cout << "4. Print all items\n";
        cout << "5. Retrieve item by position\n";
        cout << "6. Check if list is empty\n";
        cout << "7. Make list null (clear)\n";
        cout << "8. Save and Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            GroceryItem x;
            int pos;
            cout << "Enter ID: "; cin >> x.id; cin.ignore();
            cout << "Enter Name: "; getline(cin, x.name);
            cout << "Enter Category: "; getline(cin, x.category);
            cout << "Enter Price: "; cin >> x.price; cin.ignore();
            cout << "Enter Expiry (YYYY-MM-DD): "; getline(cin, x.expiry);
            cout << "Enter position to insert (0 to " << L.items.size() << "): ";
            cin >> pos; cin.ignore();
            if (pos < 0 || pos > L.items.size()) {
                cout << "Invalid position.\n";
            } else {
                L.insert(x, pos);
                cout << "Item inserted.\n";
            }
        } else if (choice == 2) {
            int pos;
            cout << "Enter position to delete (0 to " << L.items.size()-1 << "): ";
            cin >> pos; cin.ignore();
            if (pos < 0 || pos >= L.items.size()) {
                cout << "Invalid position.\n";
            } else {
                L.remove(pos);
                cout << "Item deleted.\n";
            }
        } else if (choice == 3) {
            int id;
            cout << "Enter ID to search: ";
            cin >> id; cin.ignore();
            int idx = L.search(id);
            if (idx == -1) cout << "Item not found.\n";
            else {
                cout << "Item found at position " << idx << ":\n";
                GroceryItem x = L.retrieve(idx);
                cout << "ID: " << x.id << ", Name: " << x.name << ", Category: " << x.category
                     << ", Price: " << x.price << ", Expiry: " << x.expiry << endl;
            }
        } else if (choice == 4) {
            L.print();
        } else if (choice == 5) {
            int pos;
            cout << "Enter position to retrieve (0 to " << L.items.size()-1 << "): ";
            cin >> pos; cin.ignore();
            if (pos < 0 || pos >= L.items.size()) {
                cout << "Invalid position.\n";
            } else {
                GroceryItem x = L.retrieve(pos);
                cout << "ID: " << x.id << ", Name: " << x.name << ", Category: " << x.category
                     << ", Price: " << x.price << ", Expiry: " << x.expiry << endl;
            }
        } else if (choice == 6) {
            if (L.isEmpty()) cout << "List is empty.\n";
            else cout << "List is not empty.\n";
        } else if (choice == 7) {
            L.makeNull();
            cout << "List cleared.\n";
        } else if (choice == 8) {
            L.writeToFile("grocery.bin");
            cout << "List saved to grocery.bin. Exiting.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 8);
    return 0;
}