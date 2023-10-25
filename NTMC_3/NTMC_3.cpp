#include "iostream"
#include "vector"

using namespace std;

vector <long long> deg2(long long el, long long n) {//Раскладываем число на степени двойки
	vector <long long> res;
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


long long multMod(long long n, long long mod, vector <pair <long long, long long>> lst) {//Умножаем число по модулю
	if (lst.size() == 1) {
		long long res = 1;
		for (short i = 0; i < lst[0].second; i++)
			res = res * lst[0].first % mod;
		return res;
	}
	else if (lst[0].second == 1) {
		long long el = lst[0].first;
		lst.erase(lst.begin());
		return (el * multMod(n, mod, lst)) % mod;
	}
	else {
		for (short i = 0; i < lst.size(); i++)
			if (lst[i].second > 1) {
				lst[i].first = (lst[i].first * lst[i].first) % mod;
				lst[i].second /= 2;
			}
		return multMod(n, mod, lst);
	}
}


long long powClosed(long long x, long long y, long long mod) {//Возводим число в степени по модулю
	if (y == 0)
		return 1;

	vector <long long> lst = deg2(1, y);
	vector <pair <long long, long long>> xDegs;
	for (short i = 0; i < lst.size(); i++)
		xDegs.push_back(make_pair(x, lst[i]));

	long long res = multMod(x, mod, xDegs);
	return res;
}


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


int symbolLegendre(int a, int p) {
	if (a == 0)
		return 0;
	int res = powClosed(a, (p - 1) / 2, p);
	return res == 1 ? 1 : -1;
}


bool ferma(int n, short k) {
	for (short i = 0; i < k; i++) {
		int a = rand() % (n - 2) + 2;

		int d = binaryEuclid(a, n);
		if (d > 1)
			return false;
		
		if (powClosed(a, n - 1, n) != 1)
			return false;
	}
	return true;
}


bool soloveyStrassen(int n, short k) {
	int halfed = n >> 1;
	for (short i = 0; i < k; i++) {
		int a = rand() % (n - 2) + 2;

		int d = binaryEuclid(a, n);
		if (d > 1)
			return false;

		int pow = powClosed(a, halfed, n);
		int symLegendre = symbolLegendre(a, n);
		if ((pow == 1 && symLegendre != 1) || (pow == n - 1 && symLegendre != -1))
			return false;
	}
	return true;
}


bool millerRabin(int n, short k) {
	int t = n - 1;
	int s = 0;
	while (t % 2 == 0) {
		s++;
		t = t / 2;
	}

	for (short i = 0; i < k; i++) {
		int a = rand() % (n - 2) + 2;

		int d = binaryEuclid(a, n);
		if (d > 1)
			return false;

		long long r = powClosed(a, t, n);
		if (r == 1 || r == n - 1)
			continue;

		bool isSimple = false;
		for (short j = 1; j < s; j++) {
			r = (r * r) % n;
			if (r == n - 1) {
				isSimple = true;
				break;
			}
		}
		if (!isSimple)
			return false;
	}
	return true;
}


int main() {
	setlocale(LC_ALL, "ru");
	for (;;) {
		srand(time(0));
		cout << "\tПроверка числа n на простоту с помощью теста Ферма, Соловея-Штрассена и Миллера-Рабина \nВведите n > 5: ";
		int n;
		cin >> n;
		cout << "Введите число проверок k > 0: ";
		short k;
		cin >> k;

		if (n < 6 || k < 0) {
			cout << "Incorrect. Try again \n\n";
			continue;
		}

		if (ferma(n, k))
			cout << "\nТест Ферма: число " << n << ", вероятно, простое";
		else
			cout << "\nТест Ферма: число " << n << " составное";

		if (soloveyStrassen(n, k))
			cout << "\nТест Соловея-Штрассена: число " << n << ", вероятно, простое";
		else
			cout << "\nТест Соловея-Штрассена: число " << n << " составное";

		if (millerRabin(n, k))
			cout << "\nТест Миллера-Рабина: число " << n << ", вероятно, простое";
		else
			cout << "\nТест Миллера-Рабина: число " << n << " составное";

		cout << "\n\n";
	}
	return 0;
}