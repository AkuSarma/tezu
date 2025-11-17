#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

// ----------- Utility to print matrix --------------
void printMatrix(const vector<vector<double>> &M)
{
    for (auto &row : M)
    {
        for (auto &val : row)
            cout << setw(10) << val << " ";
        cout << endl;
    }
}

// ----------- Matrix transpose ---------------------
vector<vector<double>> transpose(const vector<vector<double>> &A)
{
    int r = A.size(), c = A[0].size();
    vector<vector<double>> T(c, vector<double>(r));

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            T[j][i] = A[i][j];

    return T;
}

// ----------- Determinant (recursive) -------------
double determinant(const vector<vector<double>> &A)
{
    int n = A.size();

    if (n == 1)
        return A[0][0];
    if (n == 2)
        return (A[0][0] * A[1][1]) - (A[0][1] * A[1][0]);

    double det = 0;

    for (int p = 0; p < n; p++)
    {
        vector<vector<double>> sub(n - 1, vector<double>(n - 1));
        for (int i = 1; i < n; i++)
        {
            int colIndex = 0;
            for (int j = 0; j < n; j++)
            {
                if (j == p)
                    continue;
                sub[i - 1][colIndex++] = A[i][j];
            }
        }
        det += A[0][p] * pow(-1, p) * determinant(sub);
    }

    return det;
}

// ----------- Matrix inverse (adjoint/det) ---------
vector<vector<double>> inverse(const vector<vector<double>> &A)
{
    int n = A.size();
    double det = determinant(A);

    if (det == 0)
        throw runtime_error("Matrix is not invertible.");

    vector<vector<double>> adj(n, vector<double>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            vector<vector<double>> sub(n - 1, vector<double>(n - 1));
            int r = 0, c = 0;

            for (int row = 0; row < n; row++)
            {
                if (row == i)
                    continue;
                c = 0;
                for (int col = 0; col < n; col++)
                {
                    if (col == j)
                        continue;
                    sub[r][c++] = A[row][col];
                }
                r++;
            }

            adj[j][i] = pow(-1, i + j) * determinant(sub);
        }
    }

    for (auto &row : adj)
        for (auto &val : row)
            val /= det;

    return adj;
}

int main()
{
    cout << "Choose file no.: \n 1\n 2" << endl;
    int choice;
    cin >> choice;
    ifstream file;
    if (choice == 1)
    {

        file.open("asn4_1a.txt");
    }
    else
    {
        file.open("asn4_1b.txt");
    }
    if (!file)
    {
        cout << "Error: Cannot open input.txt\n";
        return 0;
    }

    vector<vector<double>> A;
    string line;

    // ----------- Read matrix from file ------------
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<double> row;
        double num;
        char comma;

        while (ss >> num)
        {
            row.push_back(num);
            ss >> comma;
        }

        A.push_back(row);
    }

    cout << "\n===== MATRIX A =====\n";
    printMatrix(A);

    // ----------- Transpose ------------
    cout << "\n===== TRANSPOSE (A^T) =====\n";
    printMatrix(transpose(A));

    // ----------- Determinant ----------
    if (A.size() == A[0].size())
    {
        cout << "\n===== DETERMINANT |A| =====\n";
        double det = determinant(A);
        cout << det << endl;

        // ----------- Inverse -----------
        if (det != 0)
        {
            cout << "\n===== INVERSE (A^-1) =====\n";
            printMatrix(inverse(A));
        }
        else
        {
            cout << "\nMatrix is NOT invertible.\n";
        }
    }
    else
    {
        cout << "\nMatrix is NOT square, determinant and inverse not possible.\n";
    }

    return 0;
}
