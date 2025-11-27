#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//------------------ STUDENT RECORD STRUCT ---------------------
struct Student
{
    string roll;
    double marks;
};

//---------------------- BST NODE ------------------------------
struct BSTNode
{
    Student data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Student d)
    {
        data = d;
        left = right = nullptr;
    }
};

//---------------------- AVL NODE ------------------------------
struct AVLNode
{
    Student data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Student d)
    {
        data = d;
        left = right = nullptr;
        height = 1;
    }
};

//==================== BST FUNCTIONS ============================

int bstComparisons = 0;

BSTNode *bstInsert(BSTNode *root, Student d)
{
    if (!root)
        return new BSTNode(d);
    if (d.roll < root->data.roll)
        root->left = bstInsert(root->left, d);
    else
        root->right = bstInsert(root->right, d);
    return root;
}

BSTNode *findMin(BSTNode *root)
{
    while (root->left)
        root = root->left;
    return root;
}

BSTNode *bstDelete(BSTNode *root, string roll)
{
    if (!root)
        return nullptr;
    if (roll < root->data.roll)
        root->left = bstDelete(root->left, roll);
    else if (roll > root->data.roll)
        root->right = bstDelete(root->right, roll);
    else
    {
        if (!root->left)
            return root->right;
        else if (!root->right)
            return root->left;

        BSTNode *temp = findMin(root->right);
        root->data = temp->data;
        root->right = bstDelete(root->right, temp->data.roll);
    }
    return root;
}

BSTNode *bstSearch(BSTNode *root, string roll)
{
    while (root)
    {
        bstComparisons++;
        if (roll == root->data.roll)
            return root;
        root = (roll < root->data.roll) ? root->left : root->right;
    }
    return nullptr;
}

int bstHeight(BSTNode *root)
{
    if (!root)
        return 0;
    return 1 + max(bstHeight(root->left), bstHeight(root->right));
}

void inorderBST(BSTNode *root)
{
    if (!root)
        return;
    inorderBST(root->left);
    cout << root->data.roll << "\t" << root->data.marks << endl;
    inorderBST(root->right);
}

//==================== AVL FUNCTIONS ============================

int avlComparisons = 0;

int height(AVLNode *n) { return n ? n->height : 0; }

AVLNode *rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    y->left = x->right;
    x->right = y;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode *rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode *n)
{
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode *avlInsert(AVLNode *root, Student d)
{
    if (!root)
        return new AVLNode(d);

    if (d.roll < root->data.roll)
        root->left = avlInsert(root->left, d);
    else
        root->right = avlInsert(root->right, d);

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && d.roll < root->left->data.roll)
        return rotateRight(root);
    if (balance < -1 && d.roll > root->right->data.roll)
        return rotateLeft(root);
    if (balance > 1 && d.roll > root->left->data.roll)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && d.roll < root->right->data.roll)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

AVLNode *avlSearch(AVLNode *root, string roll)
{
    while (root)
    {
        avlComparisons++;
        if (roll == root->data.roll)
            return root;
        root = (roll < root->data.roll) ? root->left : root->right;
    }
    return nullptr;
}

void inorderAVL(AVLNode *root)
{
    if (!root)
        return;
    inorderAVL(root->left);
    cout << root->data.roll << "\t" << root->data.marks << endl;
    inorderAVL(root->right);
}

//========================== MAIN ================================

int main()
{
    ifstream file("students.txt");
    string roll;
    double marks;

    BSTNode *bstRoot = nullptr;
    AVLNode *avlRoot = nullptr;

    while (file >> roll >> marks)
    {
        Student s{roll, marks};
        bstRoot = bstInsert(bstRoot, s);
        avlRoot = avlInsert(avlRoot, s);
    }

    int choice;
    while (true)
    {
        cout << "\n===== MENU =====\n";
        cout << "1. Insert\n2. Delete (BST only)\n3. Search\n4. Display Sorted (AVL)\n5. Compare Heights\n6. Exit\n7. Display Sorted (BST)\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter roll: ";
            cin >> roll;
            cout << "Enter marks: ";
            cin >> marks;
            Student s{roll, marks};
            bstRoot = bstInsert(bstRoot, s);
            avlRoot = avlInsert(avlRoot, s);
            cout << "Inserted Successfully\n";
        }

        else if (choice == 2)
        {
            cout << "Enter roll to delete: ";
            cin >> roll;
            bstRoot = bstDelete(bstRoot, roll);
            cout << "Deleted from BST\n";
        }

        else if (choice == 3)
        {
            cout << "Enter roll to search: ";
            cin >> roll;

            bstComparisons = 0;
            bstSearch(bstRoot, roll);
            cout << "BST Comparisons: " << bstComparisons << endl;

            avlComparisons = 0;
            avlSearch(avlRoot, roll);
            cout << "AVL Comparisons: " << avlComparisons << endl;
        }

        else if (choice == 4)
        {
            cout << "\nStudents Sorted (AVL):\n";
            inorderAVL(avlRoot);
        }

        else if (choice == 5)
        {
            cout << "Height of BST: " << bstHeight(bstRoot) << endl;
            cout << "Height of AVL: " << height(avlRoot) << endl;
        }

        else if (choice == 6)
        {
            break;
        }

        else if (choice == 7)
        {
            cout << "\nStudents Sorted (BST):\n";
            inorderBST(bstRoot);
        }

        else
            cout << "Invalid Choice\n";
    }

    return 0;
}
