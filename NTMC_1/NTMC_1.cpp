#include "iostream"
#include "vector"

using namespace std;
/////////////////////////////////////////////////////////////////////АЛГОРИТМ ЕВКЛИДА//////////////////////////////////////////////////////////////////////////
//Обычный алгоритм Евклида
int usualEuclid(int a, int b) {
	if (a < b)
		swap(a, b);
	if (a < 0 || b < 0)
		throw string{ "Выполнение невозможно: a < 0 или b < 0" };
	else if (b == 0)
		return a;

	int r = a % b;
	return usualEuclid(b, r);
}


//Бинарный алгоритм Евклида
int binaryEuclid(int a, int b) {
	if (a < 0 || b < 0)
		throw string{ "Выполнение невозможно: a < 0 или b < 0" };

	if (a == 0)
		return b;
	else if (b == 0 || a == b)
		return a;
	else if (a == 1 || b == 1)
		return 1;
	else if ((a & 1) == 0 && (b & 1) == 0)
		return binaryEuclid(a >> 1, b >> 1) << 1;
	else if ((a & 1) == 0 && (b & 1) == 1)
		return binaryEuclid(a >> 1, b);
	else if ((a & 1) == 1 && (b & 1) == 0)
		return binaryEuclid(a, b >> 2);
	else {
		if (b > a)
			return binaryEuclid((b - a) >> 1, a);
		else
			return binaryEuclid((a - b) >> 1, b);
	}
}


//Расширенный алгоритм Евклида
pair <int, int> advancedEuclid (int a, int b) {
	if (a < 0 || b < 0)
		throw string{ "Выполнение невозможно: a < 0 или b < 0" };

	int q, aPrev = a, aCur = b, aNext = -1;
	int xPrev = 1, xCur = 0, xNext;
	int yPrev = 0, yCur = 1, yNext;
	while (aNext != 0) {
		q = aPrev / aCur;
		aNext = aPrev % aCur;
		aPrev = aCur; aCur = aNext;

		xNext = xPrev - (xCur * q);
		xPrev = xCur; xCur = xNext;

		yNext = yPrev - (yCur * q);
		yPrev = yCur; yCur = yNext;
	}

	return make_pair(xPrev, yPrev);
}


//Выбор алгоритма Евклида
void mainEuclid() {
	for (;;) {
		cout << "\n1 - Обычный алгоритм Евклида \n2 - Бинарный алгоритм Евклида \n3 - Расширенный алгоритм Евклида \n4 - Назад \n";
		int x;
		cin >> x;
		int a, b;
		switch (x) {
		case 1:
			cout << "\n\tОбычный алгоритм Евклида нахождения НОД \n";
			cout << "a = ";
			cin >> a;
			cout << "b = ";
			cin >> b;

			try {
				cout << "НОД(" << a << ", " << b << ") = " << usualEuclid(a, b);
			}
			catch (string& error_message) {
				cout << error_message;
			}
			cout << endl;
			break;
		case 2:
			cout << "\n\tБинарный алгоритм Евклида нахождения НОД \n";
			cout << "a = ";
			cin >> a;
			cout << "b = ";
			cin >> b;

			cout << "НОД(" << a << ", " << b << ") = " << binaryEuclid(a, b);
			cout << endl;
			break;
		case 3:
			cout << "\n\tPасширенный алгоритм Евклида нахождения НОД \n";
			cout << "a = ";
			cin >> a;
			cout << "b = ";
			cin >> b;

			try {
				pair <int, int> xy = advancedEuclid(a, b);
				cout << "НОД(" << a << ", " << b << ") = " << a << "*" << xy.first << " + " << b << "*" <<
					    xy.second << " = " << a * xy.first + b * xy.second;
			}
			catch (string& error_message) {
				cout << error_message;
			}
			cout << endl;
			break;
		case 4:
			return;
		default:
			cout << "Incorrect. Try again \n\n";
		}
	}
}


/////////////////////////////////////////////////////////////РЕШЕНИЕ СИСТЕМЫ ЛИНЕЙНЫХ СРАВНЕНИЙ///////////////////////////////////////////////////////////////
//Провека взаимной простоты m
bool checkMutualSimplicity(vector <pair <int, int>> raws) {
	for (int i = 0; i < raws.size(); i++)
		for (int j = i + 1; j < raws.size(); j++) {
			if (raws[i].second > raws[j].second) {
				if (binaryEuclid(raws[i].second, raws[j].second) != 1)
					return false;
			}
			else
				if (binaryEuclid(raws[j].second, raws[i].second) != 1)
					return false;
		}
	return true;
}


//Греко-китайская теорема об остатках
pair <int, int> gcTheorem(vector <pair <int, int>> raws) {
	if (!checkMutualSimplicity)
		throw string{ "Модули m не являются попарно взаимно простыми!" };

	int M = 1;
	for (int i = 0; i < raws.size(); i++)
		M *= raws[i].second;

	int res = 0;
	for (int i = 0; i < raws.size(); i++) {
		int Mi = 1;
		for (int j = 0; j < raws.size(); j++) {
			if (i == j)
				continue;
			Mi *= raws[j].second;
		}
		for (int j = 1;; j++)
			if ((Mi * j) % raws[i].second == raws[i].first) {
				res += Mi * j;
				break;
			}
	}

	return make_pair(res % M, M);
}


//Алгоритм Гарнера
pair <int, int> garner(vector <pair <int, int>> raws) {
	if (!checkMutualSimplicity)
		throw string{ "Модули m не являются попарно взаимно простыми!" };

	int M = 1;
	for (int i = 0; i < raws.size(); i++)
		M *= raws[i].second;

	vector <int> c(raws.size(), 1);
	int u, x;
	for (int i = 1; i < raws.size(); i++) {
		for (int j = 0; j < i; j++) {
			u = advancedEuclid(raws[j].second, raws[i].second).first;
			c[i] = (u * c[i]) % raws[i].second;
		}
	}

	u = raws[0].first;
	x = u;
	int mAcc = 1;
	for (int i = 1; i < raws.size(); i++) {
		u = ((raws[i].first - x) * c[i]) % raws[i].second;
		mAcc *= raws[i - 1].second;
		x = x + u * mAcc;
	}

	if (x < 0)
		x += M;
	return make_pair(x, M);
}


//Ввод системы линейных сравнений
void mainSLC() {
	for (;;) {
		cout << "\n1 - Греко-китайская теорема об остатках \n2 - Алгоритм Гарнера \n3 - Назад \n";
		int x;
		cin >> x;
		if (x == 3)
			return;

		vector <pair <int, int>> raws;
		cout << "\nКоличество сравнений: ";
		int k, a, m;
		cin >> k;
		for (int i = 0; i < k; i++) {
			cout << "Введите a" << i + 1 << ", m" << i + 1 << ": ";
			cin >> a >> m;
			raws.push_back(make_pair(a % m, m));
		}

		cout << "\nСистема линейных сравнений: \n";
		for (int i = 0; i < raws.size(); i++)
			cout << "x = " << raws[i].first << "(mod " << raws[i].second << ") \n";

		switch (x) {
		case 1:
			try {
				pair <int, int> res = gcTheorem(raws);
				cout << "Решение данной системы линейных сравнений: x = " << res.first << " (mod " << res.second << ")" << endl;
			}
			catch (string& error_message) {
				cout << error_message;
			}
			cout << endl;
			break;
		case 2:
			try {
				pair <int, int> res = garner(raws);
				cout << "Решение данной системы линейных сравнений: x = " << res.first << " (mod " << res.second << ")" << endl;
			}
			catch (string& error_message) {
				cout << error_message;
			}
			cout << endl;
			break;
		default:
			cout << "Incorrect. Try again \n\n";
		}
	}
}


/////////////////////////////////////////////////////////////////////////МЕТОД ГАУССА/////////////////////////////////////////////////////////////////////////
//Проверка, имеет ли решения СЛУ
bool haveSolution(vector <vector <int>> matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		bool flag = true;
		for (int j = 0; j < matrix[i].size(); j++) {
			if (j == matrix[i].size() - 1 && matrix[i][j] == 1 && flag)
				return false;
			else if (matrix[i][j] != 0)
				break;
		}
	}
	return true;
}


//Умножение строки на число
vector <int> multRawtoNum(vector <int> raw, int num, int field) {
	vector <int> res;
	for (int i = 0; i < raw.size(); i++) {
		int x = (raw[i] * num) % field;
		res.push_back(x >= 0 ? x : x + field);
	}
	return res;
}


//Сложение строк
vector <int> addRaws(vector <int> a, vector <int> b, int field) {
	vector <int> res;
	for (int i = 0; i < a.size(); i++) {
		int x = (a[i] + b[i]) % field;
		res.push_back(x >= 0 ? x : x + field);
	}
	return res;
}


//Удаление нулевых строк
void delZeroRaws(vector <vector <int>>& matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		bool flag = true;
		for (int j = 0; j < matrix[i].size(); j++)
			if (matrix[i][j] != 0) {
				flag = false;
				break;
			}
		if (flag) {
			matrix.erase(matrix.begin() + i);
			i--;
		}
	}
}


//Меняем столбцы, если на главной диагонали 0
void swapColms(vector <vector <int>>& matrix, int colm) {
	for (int i = colm + 1; i < matrix[colm].size() - 1; i++)
		if (matrix[colm][i] != 0) {
			for (int j = 0; j < matrix.size(); j++)
				swap(matrix[j][colm], matrix[j][i]);
			return;
		}
}


//Метод Гаусса
void gauss() {
	cout << "\n\tРешение системы линейных уравнений методом Гаусса \n";
	cout << "Поле: ";
	int field;
	cin >> field;
	cout << "Количество строк и столбцов матрицы системы: ";
	int raws, cols;
	cin >> raws >> cols;
	vector <vector <int>> matrix(raws);
	cout << "Матрица системы: \n";
	for (int i = 0; i < raws; i++)
		for (int j = 0; j < cols; j++) {
			int x;
			cin >> x;
			matrix[i].push_back(x);
		}

	for (int i = 0; i < matrix.size(); i++) {
		if (matrix[i][i] == 0)
			swapColms(matrix, i);

		int revEl = advancedEuclid(matrix[i][i], field).first;
		matrix[i] = multRawtoNum(matrix[i], revEl, field);
		for (int j = 0; j < matrix.size(); j++) {
			if (i == j)
				continue;
			matrix[j] = addRaws(matrix[j], multRawtoNum(matrix[i], -matrix[j][i], field), field);
		}	
	}
	delZeroRaws(matrix);

	if (!haveSolution) {
		cout << "Решений нет! \n";
		return;
	}

	cout << "\nРазрешённое решение исходной системы: \n";
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			if (j == matrix[i].size() - 2)
				cout << matrix[i][j] << "x" << j + 1 << " = ";
			else if (j == matrix[i].size() - 1)
				cout << matrix[i][j] << endl;
			else
				cout << matrix[i][j] << "x" << j + 1 << " + ";
		}
	}

	cout << "\nОбщее решение исходной системы: \n";
	for (int i = 0; i < matrix.size(); i++) {
		cout << "x" << i + 1 << " = ";
		for (int j = i + 1; j < matrix[i].size() - 1; j++) {
			if (matrix[i][j] == 0)
				continue;
			matrix[i][j] = -matrix[i][j] + field;
			cout << matrix[i][j] << "x" << j + 1 << " + ";
		}
		cout << matrix[i].back() << endl;
	}

	cout << "\nСвободные неизвестные ";
	raws = matrix.size();
	int numVals = 0;
	for (int i = raws; i < matrix[0].size() - 1; i++) {
		numVals++;
		if (i == matrix[0].size() - 2)
			cout << "x" << i + 1 << ": ";
		else
			cout << "x" << i + 1 << ", ";
	}
	vector <int> vals;
	for (int i = 0; i < numVals; i++) {
		int x;
		cin >> x;
		vals.push_back(x);
	}

	vector <int> res;
	for (int i = 0; i < matrix.size(); i++) {
		int ans = 0;
		for (int j = raws; j < matrix[i].size() - 1; j++)
			ans += vals[j - raws] * matrix[i][j];
		ans += matrix[i].back();
		res.push_back(ans);
	}
	res.insert(res.end(), vals.begin(), vals.end());

	cout << "Частное решение: (";
	for (int i = 0; i < res.size(); i++) {
		if (i == res.size() - 1)
			cout << res[i] << ")";
		else
			cout << res[i] << ", ";
	}
	cout << endl;
}

 
int main() {
	setlocale(LC_ALL, "ru");
	for (;;) {
		cout << "1 - Алгоритм Евклида \n2 - Решение систем сравнений \n3 - Метод Гаусса \n4 - Выход \n";
		int x;
		cin >> x;
		switch (x) {
		case 1:
			mainEuclid();
			cout << endl;
			break;
		case 2:
			mainSLC();
			cout << endl;
			break;
		case 3:
			gauss();
			cout << endl;
			break;
		case 4:
			return 0;
		default:
			cout << "Incorrect. Try again \n\n";
		}
	}
}