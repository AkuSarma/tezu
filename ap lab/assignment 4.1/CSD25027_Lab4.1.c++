#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

using Matrix = vector<vector<double>>;


Matrix readMatrix(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    Matrix M;
    string line;

    while (getline(file, line)) {
        replace(line.begin(), line.end(), ',', ' '); // convert comma → space
        stringstream ss(line);
        vector<double> row;
        double val;
        while (ss >> val) row.push_back(val);

        if (!row.empty()) M.push_back(row);
    }

    file.close();
    return M;
}

void printMatrix(const Matrix &M, const string &title) {
    cout << "\n--- " << title << " (" << M.size() << "x" 
         << (M.empty() ? 0 : M[0].size()) << ") ---\n";

    for (auto &r : M) {
        for (auto &v : r) cout << setw(10) << fixed << setprecision(4) << v << " ";
        cout << "\n";
    }
}

Matrix transpose(const Matrix &A) {
    int r = A.size(), c = A[0].size();
    Matrix T(c, vector<double>(r));

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            T[j][i] = A[i][j];

    return T;
}


double determinant(const Matrix &A) {
    int n = A.size();
    if (n == 0) return NAN;
    if (A[0].size() != n) return NAN; 

    if (n == 1) return A[0][0];
    if (n == 2)
        return A[0][0]*A[1][1] - A[0][1]*A[1][0];

    double det = 0.0;
    for (int col = 0; col < n; ++col) {
        Matrix minor;
        minor.reserve(n - 1);
        for (int i = 1; i < n; ++i) {
            vector<double> row;
            row.reserve(n - 1);
            for (int j = 0; j < n; ++j) {
                if (j == col) continue;
                row.push_back(A[i][j]);
            }
            minor.push_back(move(row));
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * A[0][col] * determinant(minor);
    }
    return det;
}

// Gauss-Jordan inverse
Matrix inverse(Matrix A) {
    int n = A.size();
    if (n != A[0].size()) return {};

    double detA = determinant(A);
    if (fabs(detA) < 1e-9) {
        cout << "Matrix not invertible.\n";
        return {};
    }

    Matrix M(n, vector<double>(2*n));

    // build [A | I]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) M[i][j] = A[i][j];
        M[i][n+i] = 1;
    }

    // elimination
    for (int i = 0; i < n; i++) {
        double pivot = M[i][i];
        for (int j = 0; j < 2*n; j++) M[i][j] /= pivot;

        for (int r = 0; r < n; r++) {
            if (r != i) {
                double factor = M[r][i];
                for (int c = 0; c < 2*n; c++)
                    M[r][c] -= factor * M[i][c];
            }
        }
    }

    // extract inverse
    Matrix Inv(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Inv[i][j] = M[i][j+n];

    return Inv;
}

Matrix multiply(const Matrix &A, const Matrix &B) {
    if (A[0].size() != B.size()) {
        cout << "Cannot multiply: incompatible shapes.\n";
        return {};
    }

    int r = A.size(), c = B[0].size(), klen = B.size();
    Matrix C(r, vector<double>(c, 0));

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            for (int k = 0; k < klen; k++)
                C[i][j] += A[i][k] * B[k][j];

    return C;
}

int main() {
    string fa;
    cout << "Enter matrix A filename: ";
    cin >> fa;

    Matrix A = readMatrix(fa);
    printMatrix(A, "Matrix A");

    printMatrix(transpose(A), "Transpose A^T");

    if (A.size() == A[0].size()) {
        cout << "Determinant: " << determinant(A) << "\n";
        Matrix Inv = inverse(A);
        if (!Inv.empty()) printMatrix(Inv, "Inverse A^-1");
    } else {
        cout << "Matrix not square → determinant & inverse undefined.\n";
    }

    string fb;
    cout << "Enter matrix B filename for multiplication (or 'skip'): ";
    cin >> fb;

    if (fb != "skip") {
        Matrix B = readMatrix(fb);
        printMatrix(B, "Matrix B");
        Matrix C = multiply(A, B);
        if (!C.empty()) printMatrix(C, "A * B");
    }

    return 0;
}
