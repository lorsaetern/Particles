#include "Matrices.h"

namespace Matrices
{
	Matrix::Matrix(int _rows, int _cols) {
		rows = _rows;
		cols = _cols;
		a.resize(rows, vector<double>(cols));
	}
	Matrix operator+(const Matrix& a, const Matrix& b) {
		Matrix c(a.getRows(), a.getCols());
		if (a.getRows() != b.getRows() && a.getCols() != b.getCols()) {
			throw runtime_error("Error: dimensions must agree");
		}
		else {
			for (int i = 0; i < a.getRows(); i++) {
				for (int j = 0; j < a.getCols(); j++) {
					c(i, j) = a(i, j) + b(i, j);
				}
			}
		}
		return c;
	}
	Matrix operator*(const Matrix& a, const Matrix& b) {
		if (a.getCols() != b.getRows()) {
			throw runtime_error("Error: dimensions must agree");
		}
		Matrix c(a.getRows(), b.getCols());
		for (int k = 0; k < b.getCols(); k++) {
			for (int i = 0; i < a.getRows(); i++) {
				for (int j = 0; j < b.getRows(); j++) {
					c(i, k) += a(i, j) * b(j, k);
				}
			}
		}
		return c;
	}
	bool operator==(const Matrix& a, const Matrix& b) {
		bool tf;
		cout << "a" << endl;
		if (a.getRows() == b.getRows() && a.getCols() == b.getCols()) {
			cout << "b" << endl;
			for (int i = 0; i < a.getRows(); i++) {
				for (int j = 0; j < a.getCols(); j++) {
					if (a(i, j) == b(i, j)) {
						cout << "c" << endl;
						tf = true;
					}
					else {
						cout << "cc" << endl;
						tf = false;
					}
				}
			}
		}
		else {
			cout << "d" << endl;
			tf = false;
		}
		return tf;
	}
	bool operator!=(const Matrix& a, const Matrix& b) {
		bool tf;
		cout << "a" << endl;
		if (a.getRows() == b.getRows() && a.getCols() == b.getCols()) {
			cout << "b" << endl;
			for (int i = 0; i < a.getRows(); i++) {
				for (int j = 0; j < a.getCols(); j++) {
					if (a(i, j) == b(i, j)) {
						cout << "c" << endl;
						tf = false;
					}
					else {
						tf = true;
					}
				}
			}
		}
		else {
			cout << "d" << endl;
			tf = true;
		}
		return tf;
	}
	ostream& operator<<(ostream& os, const Matrix& a) {
		for (int i = 0; i < a.getRows(); i++) {
			for (int j = 0; j < a.getCols(); j++) {
				os << setw(10) << a(i, j);
			}
			os << "\n";
		}
		return os;
	}
}