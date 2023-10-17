#include "iostream"
#include "vector"
#include "algorithm"
#include "cmath"

using namespace std;
int myPow(int x, int y) {
	int res = 1;
	for (int i = 0; i < y; i++)
		res *= x;
	return res;
}


//Обычный алгоритм Евклида
int usualEuclid(int a, int b) {
	if (a < b)
		swap(a, b);
	if (a < 0 || b < 0) {
		cout << "Алгоритм Евклида не может найти НОД(" << a << ", " << b << ")!";
		throw string{ "Выполнение невозможно: a < 0 или b < 0" };
	}
	else if (b == 0)
		return a;

	int r = a % b;
	return usualEuclid(b, r);
}


//Расширенный алгоритм Евклида
pair <int, int> advancedEuclid(int a, int b) {
	if (a < 0 || b < 0) {
		cout << "Алгоритм Евклида не может найти НОД(" << a << ", " << b << ")!";
		throw string{ "Выполнение невозможно: a < 0 или b < 0" };

	}

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


vector <int> deg2(int el, int n) {//Раскладываем число на степени двойки
	vector <int> res;
	while (n != 0) {
		if (n / el == 1) {
			res.push_back(el);
			n -= el;
			el = 1;
		}
		else
			el *= 2;
	}
	return res;
}


int multMod(int n, int mod, vector <pair <int, int>> lst) {//Умножаем число по модулю
	if (lst.size() == 1) {
		int res = 1;
		for (int i = 0; i < lst[0].second; i++)
			res = res * lst[0].first % mod;
		return res;
	}
	else if (lst[0].second == 1) {
		int el = lst[0].first;
		lst.erase(lst.begin());
		return (el * multMod(n, mod, lst)) % mod;
	}
	else {
		for (int i = 0; i < lst.size(); i++)
			if (lst[i].second > 1) {
				lst[i].first = (lst[i].first * lst[i].first) % mod;
				lst[i].second /= 2;
			}
		return multMod(n, mod, lst);
	}
}


int powClosed(int x, int y, int mod) {//Возводим число в степени по модулю
	if (y == 0)
		return 0;

	vector <int> lst = deg2(1, y);
	vector <pair <int, int>> xDegs;
	for (int i = 0; i < lst.size(); i++)
		xDegs.push_back(make_pair(x, lst[i]));

	int res = multMod(x, mod, xDegs);
	return res;
}


bool miller_rabin(int n, int k = 10) {
	if (n == 0 || n == 1)
		return false;

	int d = n - 1;
	int s = 0;
	while (d % 2 == 0) {
		s++;
		d = d / 2;
	}

	int nDec = n - 1;
	for (int i = 0; i < k; i++) {
		int a = rand() % nDec;
		if (a == 0 || a == 1)
			a = a + 2;

		int x = powClosed(a, d, n);
		if (x == 1 || x == nDec)
			continue;

		bool flag = false;
		for (int j = 0; j < s; j++) {
			x = (x * x) % n;
			if (x == nDec) {
				flag = true;
				break;
			}
		}
		if (!flag)
			return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////РАЗЛОЖЕНИЕ ЧИСЕЛ В ЦЕПНУЮ ДРОБЬ///////////////////////////////////////////////////////////
//Разложение в цепную дробь
void continuousFraction(int a0, int a1, vector <int>& fractions) {
	int q = a0 / a1, r = a0 % a1;
	fractions.push_back(q);
	if (r == 0)
		return;
	else if (r == 1)
		fractions.push_back(a1);
	else
		continuousFraction(a1, r, fractions);
}


void mainFraction() {
	cout << "\n\tРазложение чисел в цепную дробь";
	int a0, a1;
	cout << "\na0 = ";
	cin >> a0;
	cout << "a1 = ";
	cin >> a1;

	if (usualEuclid(a0, a1) != 1) {
		cout << "\nНОД(" << a0 << ", " << a1 << ") != 1";
		return;
	}

	vector <int> fractions;
	continuousFraction(a0, a1, fractions);
	vector <int> P{ 0, 1 };
	vector <int> Q{ 1, 0 };
	for (int i = 0; i < fractions.size(); i++) {
		P.push_back(fractions[i] * P[i + 1] + P[i]);
		Q.push_back(fractions[i] * Q[i + 1] + Q[i]);
	}

	cout << "\na0 / a1 = (" << fractions[0] << "; ";
	for (int i = 1; i < fractions.size(); i++) {
		if (i == fractions.size() - 1)
			cout << fractions[i] << ")";
		else
			cout << fractions[i] << ", ";
	}
	cout << "\nЧислитель подходящих дробей: " << P[0];
	for (int i = 1; i < P.size(); i++)
		cout << ", " << P[i];
	cout << "\nЗнаменатель подходящих дробей: " << Q[0];
	for (int i = 1; i < Q.size(); i++)
		cout << ", " << Q[i];
}


////////////////////////////////////////////////////////////////////ПРИЛОЖЕНИЯ ЦЕПНЫХ ДРОБЕЙ////////////////////////////////////////////////////////////////
//Решение линейных диофантовых уравнений
void solDiophant() {
	cout << "\n\tРешение линейных диофантовых уравнений ax - by = c";
	int a, b, c;
	cout << "\na = ";
	cin >> a;
	cout << "b = ";
	cin >> b;
	cout << "c = ";
	cin >> c;

	if (usualEuclid(a, b) != 1) {
		cout << "\nЧисла " << a << " и " << b << " не являются взаимнопростыми!";
		return;
	}

	vector <int> fractions;
	continuousFraction(a, b, fractions);

	vector <int> P{ 0, 1 };
	vector <int> Q{ 1, 0 };
	for (int i = 0; i < fractions.size() - 1; i++) {
		P.push_back(fractions[i] * P[i + 1] + P[i]);
		Q.push_back(fractions[i] * Q[i + 1] + Q[i]);
	}

	cout << "\n" << a << "x - " << b << "y = " << c;
	cout << "\nx = " << myPow(-1, fractions.size()) * Q.back() * c;
	cout << "\ny = " << myPow(-1, fractions.size()) * P.back() * c;
}


//Вычисление обратных элементов в кольце вычетов Zm
int revEl(int a, int m) {
	if (usualEuclid(a, m) != 1)
		return -1;

	vector <int> fractions;
	continuousFraction(a, m, fractions);
	vector <int> Q{ 1, 0 };
	for (int i = 0; i < fractions.size() - 1; i++)
		Q.push_back(fractions[i] * Q[i + 1] + Q[i]);

	int res = myPow(-1, fractions.size()) * Q.back() % m;
	while (res < 0)
		res += m;

	return res;
}


void mainRevEl() {
	cout << "\n\tВычисление обратных элементов в кольце вычетов Zm";
	int a, m;
	cout << "\na = ";
	cin >> a;
	cout << "m = ";
	cin >> m;

	int revA = revEl(a, m);
	if (revA == -1)
		cout << "\nЧисла " << a << " и " << m << " не являются взаимнопростыми!";
	else
		cout << "\nОбратный элемент для а: " << revA;
}


//Решение линейных сравнений ax = b(mod m)
void solCompr() {
	cout << "\n\tРешение линейных сравнений ax = b(mod m)";
	int a, b, m;
	cout << "\na = ";
	cin >> a;
	cout << "b = ";
	cin >> b;
	cout << "m = ";
	cin >> m;

	if (usualEuclid(a, m) != 1) {
		cout << "\nЧисла " << a << " и " << m << " не являются взаимнопростыми!";
		return;
	}

	vector <int> fractions;
	continuousFraction(a, m, fractions);
	vector <int> Q{ 1, 0 };
	for (int i = 0; i < fractions.size() - 1; i++)
		Q.push_back(fractions[i] * Q[i + 1] + Q[i]);

	int x = myPow(-1, fractions.size()) * Q.back() * b % m;
	while (x < 0)
		x += m;

	cout << endl << a << "x = " << b << "(mod " << m << ")" << "\nx = " << x;
}


///////////////////////////////////////////////////////////////ВЫЧИСЛЕНИЕ СИМВОЛА ЯКОБИ И ЛЕЖАНДРА////////////////////////////////////////////////////////////
//Вычисление символа Якоби и Лежандра
int symbolJacobi(int a, int p) {
	int res = 1;
	while (a != 1) {
		a = a % p;
		if (abs(a) >= p / 2) {
			a = abs(a - p);
			res *= myPow(-1, (p - 1) / 2);
		}

		if (a % 2 == 0) {
			int t = 0;
			while (a % 2 == 0) {
				t++;
				a /= 2;
				if (a == 0)
					return 0;
			}
			if (t % 2 == 1)
				res *= myPow(-1, (p * p - 1) / 8);
		}

		swap(a, p);
		res *= myPow(-1, ((p - 1) / 2) * ((a - 1) / 2));
	}
	return res;
}


void mainSymbolJacobi() {
	cout << "\n\tВычисление символов Лежандра и Якоби";
	int a, p;
	cout << "\na = ";
	cin >> a;
	cout << "p = ";
	cin >> p;

	int jac = symbolJacobi(a, p);
	if (!miller_rabin(p))
		cout << "\nСимвол Лежандра: не может быть вычислен, т.к. p - не простое \nСимвол Якоби: " << jac;
	else
		cout << "\nСимволы Лежандра и Якоби (" << a << ", " << p << ") = " << jac;
}


//////////////////////////////////////////////////////ИЗВЛЕЧЕНИЕ КВАДРАТНОГО КОРНЯ В КОЛЬЦЕ ВЫЧЕТОВ/////////////////////////////////////////////////////////////
//С помощью полиномиальной арифметики
int pdf(int a, int p) {
	int b = rand() % p;
	while (symbolJacobi(b * b - 4 * a, p) != -1)
		b = (b + 1) % p;

	int degP1 = (p + 1) / 2, degP2 = 2;
	int* p1 = new int[degP1 + 1];
	p1[degP1] = 1;
	for (int i = 0; i < degP1; i++)
		p1[i] = 0;
	int* p2 = new int[degP2 + 1];
	p2[0] = a, p2[1] = -b, p2[2] = 1;

	vector <int> q(degP1 - degP2 + 1);
	for (int k = degP1 - degP2; k >= 0; k--) {
		q[k] = (p1[degP2 + k] * revEl(p2[degP2], p)) % p;
		for (int j = degP2 + k - 1; j >= k; j--)
			p1[j] -= q[k] * p2[j - k];
	}

	int res = p1[0];
	while (res < 0)
		res += p;
	return res % p;
}


//С арифметикой только этого поля
int sqrtFromZp(int a, int p) {
	int m = 0, q = p - 1;
	while (q % 2 != 1) {
		m++;
		q /= 2;
	}

	int b = rand() % p;
	while (symbolJacobi(b, p) != -1)
		b = (b + 1) % p;

	vector <int> kArr;
	for (int i = 1;; i++) {
		int k = 0;
		while (powClosed(a, myPow(2, k) * q, p) != 1)
			k++;
		kArr.push_back(k);
		if (k == 0)
			break;
		a = (a * myPow(b, myPow(2, m - kArr.back()))) % p;
	}

	int r = powClosed(a, (q + 1) / 2, p);
	for (int i = kArr.size() - 2; i >= 0; i--)
		r = (r * advancedEuclid(myPow(b, myPow(2, m - kArr[i] - 1)), p).first) % p;

	return r;
}


void mainSqrtFromZp() {
	cout << "\n\tИзвлечение квадратного корня в кольце вычетов";
	int a, p;
	cout << "\na = ";
	cin >> a;
	cout << "p = ";
	cin >> p;

	if (!miller_rabin(p)) {
		cout << "\nЧисло p должно быть простым!";
		return;
	}
	if (symbolJacobi(a, p) != 1) {
		cout << "Символ Лежандра (" << a << ", " << p << ") != 1";
		return;
	}

	int resChip = pdf(a, p);
	while (resChip * resChip % p != a)
		resChip = pdf(a, p);
	cout << "\nРешение, полученное алгоритмом Чипполы: " << resChip;
	cout << "\nПроверка: " << resChip << " * " << resChip << " (mod " << p << ") = " << resChip * resChip % p;

	int res = sqrtFromZp(a, p);
	while (res * res % p != a)
		res = sqrtFromZp(a, p);
	cout << "\n\nРешение с арифметикой только этого поля: " << res;
	cout << "\nПроверка: " << res << " * " << res << " (mod " << p << ") = " << res * res % p;
}


int main() {
	srand(time(0));
	setlocale(LC_ALL, "ru");
	for (;;) {
		cout << "\tЦепные дроби и квадратные сравнения";
		cout << "\n1 - Разложение чисел в цепную дробь \n2 - Решение линейных диофантовых уравнений ax - by = c";
		cout << "\n3 - Вычисление обратных элементов в кольце вычетов Zm \n4 - Решение линейных сравнений ax = b(mod m)";
		cout << "\n5 - Вычисление символов Якоби и Лежандра \n6 - Извлечение квадратного корня в кольце вычетов \n7 - Выход \n";
		int x;
		cin >> x;
		switch (x) {
		case 1:
			mainFraction();
			cout << "\n\n";
			break;
		case 2:
			solDiophant();
			cout << "\n\n";
			break;
		case 3:
			mainRevEl();
			cout << "\n\n";
			break;
		case 4:
			solCompr();
			cout << "\n\n";
			break;
		case 5:
			mainSymbolJacobi();
			cout << "\n\n";
			break;
		case 6:
			mainSqrtFromZp();
			cout << "\n\n";
			break;
		case 7:
			return 0;
		default:
			cout << "Incorrect. Try again \n\n";
		}
	}
}
