#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define MAX_QUEUE 100

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

class Queue {
public:
    vector<GroceryItem> items;

    void enqueue(const GroceryItem& x) {
        if (items.size() < MAX_QUEUE)
            items.push_back(x);
    }

    GroceryItem dequeue() {
        if (!items.empty()) {
            GroceryItem x = items.front();
            items.erase(items.begin());
            return x;
        }
        return GroceryItem();
    }

    GroceryItem first() {
        if (!items.empty()) return items.front();
        return GroceryItem();
    }

    bool isEmpty() {
        return items.empty();
    }

    void makeNull() {
        items.clear();
    }

    int count(const string& date) {
        int cnt = 0;
        for (const auto& item : items) {
            if (item.expiry == date) cnt++;
        }
        return cnt;
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

    map<string, Queue> categoryQueues;
    for (const auto& item : arr) {
        categoryQueues[item.category].enqueue(item);
    }

    int choice;
    string cat, date;
    do {
        cout << "\nMenu:\n";
        cout << "1. Enqueue item\n";
        cout << "2. Dequeue item\n";
        cout << "3. Show first item\n";
        cout << "4. Check if queue is empty\n";
        cout << "5. Make queue null (clear)\n";
        cout << "6. Count items with expiry date\n";
        cout << "7. Print all items in a category\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            GroceryItem x;
            cout << "Enter category: ";
            getline(cin, cat);
            cout << "Enter ID: "; cin >> x.id; cin.ignore();
            cout << "Enter Name: "; getline(cin, x.name);
            x.category = cat;
            cout << "Enter Price: "; cin >> x.price; cin.ignore();
            cout << "Enter Expiry (YYYY-MM-DD): "; getline(cin, x.expiry);
            categoryQueues[cat].enqueue(x);
            cout << "Item enqueued.\n";
        } else if (choice == 2) {
            cout << "Enter category: ";
            getline(cin, cat);
            if (categoryQueues.count(cat) && !categoryQueues[cat].isEmpty()) {
                GroceryItem x = categoryQueues[cat].dequeue();
                cout << "Dequeued: ID: " << x.id << ", Name: " << x.name << ", Price: " << x.price << ", Expiry: " << x.expiry << endl;
            } else {
                cout << "Queue is empty or category not found.\n";
            }
        } else if (choice == 3) {
            cout << "Enter category: ";
            getline(cin, cat);
            if (categoryQueues.count(cat) && !categoryQueues[cat].isEmpty()) {
                GroceryItem x = categoryQueues[cat].first();
                cout << "First: ID: " << x.id << ", Name: " << x.name << ", Price: " << x.price << ", Expiry: " << x.expiry << endl;
            } else {
                cout << "Queue is empty or category not found.\n";
            }
        } else if (choice == 4) {
            cout << "Enter category: ";
            getline(cin, cat);
            if (categoryQueues.count(cat) && categoryQueues[cat].isEmpty())
                cout << "Queue is empty.\n";
            else
                cout << "Queue is not empty or category not found.\n";
        } else if (choice == 5) {
            cout << "Enter category: ";
            getline(cin, cat);
            if (categoryQueues.count(cat)) {
                categoryQueues[cat].makeNull();
                cout << "Queue cleared.\n";
            } else {
                cout << "Category not found.\n";
            }
        } else if (choice == 6) {
            cout << "Enter category: ";
            getline(cin, cat);
            cout << "Enter expiry date (YYYY-MM-DD): ";
            getline(cin, date);
            if (categoryQueues.count(cat)) {
                int cnt = categoryQueues[cat].count(date);
                cout << "Count: " << cnt << endl;
            } else {
                cout << "Category not found.\n";
            }
        } else if (choice == 7) {
            cout << "Enter category: ";
            getline(cin, cat);
            if (categoryQueues.count(cat)) {
                cout << "Items in category '" << cat << "':\n";
                for (const auto& item : categoryQueues[cat].items) {
                    cout << "ID: " << item.id << ", Name: " << item.name << ", Price: " << item.price << ", Expiry: " << item.expiry << endl;
                }
            } else {
                cout << "Category not found.\n";
            }
        } else if (choice == 8) {
            cout << "Exiting.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 8);

    return 0;
}