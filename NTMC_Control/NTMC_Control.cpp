#include "iostream"
#include "vector"
#include "algorithm"
#include "cmath"
#include <iomanip>
#include <map>
#include <string>
#include <set>
#include <climits>

using namespace std;

class Pattern
{
private:
	static vector <long long> deg2(long long el, long long n) {//Раскладываем число на степени двойки
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


	static long long multMod(long long n, long long mod, vector <pair <long long, long long>> lst) {//Умножаем число по модулю
		if (lst.size() == 1) {
			long long res = 1;
			for (unsigned short i = 0; i < lst[0].second; i++)
				res = res * lst[0].first % mod;
			return res;
		}
		else if (lst[0].second == 1) {
			long long el = lst[0].first;
			lst.erase(lst.begin());
			return (el * multMod(n, mod, lst)) % mod;
		}
		else {
			for (unsigned short i = 0; i < lst.size(); i++)
				if (lst[i].second > 1) {
					lst[i].first = (lst[i].first * lst[i].first) % mod;
					lst[i].second /= 2;
				}
			return multMod(n, mod, lst);
		}
	}


	static long long partition(vector <pair <short, long long>>& a, long long start, long long end) {
		long long pivot = a[end].second;
		long long pIndex = start;

		for (unsigned short i = start; i < end; i++) {
			if (a[i].second <= pivot) {
				swap(a[i], a[pIndex]);
				pIndex++;
			}
		}

		swap(a[pIndex], a[end]);
		return pIndex;
	}

public:
	static long long powClosed(long long x, long long y, long long mod) {//Возводим число в степени по модулю
		if (y == 0)
			return 1;

		vector <long long> lst = deg2(1, y);
		vector <pair <long long, long long>> xDegs;
		for (unsigned short i = 0; i < lst.size(); i++)
			xDegs.push_back(make_pair(x, lst[i]));

		long long res = multMod(x, mod, xDegs);
		return res;
	}


	static int usualEuclid(int a, int b) {
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


	static pair <long long, long long> advancedEuclid(long long a, long long b) {
		if (b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };
		while (a < 0)
			a += b;

		long long q, aPrev = a, aCur = b, aNext = -1;
		long long xPrev = 1, xCur = 0, xNext;
		long long yPrev = 0, yCur = 1, yNext;
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


	//Провека взаимной простоты m
	static bool checkMutualSimplicity(vector <pair <int, int>> raws) {
		for (int i = 0; i < raws.size(); i++)
			for (int j = i + 1; j < raws.size(); j++) {
				if (raws[i].second > raws[j].second) {
					if (usualEuclid(raws[i].second, raws[j].second) != 1)
						return false;
				}
				else
					if (usualEuclid(raws[j].second, raws[i].second) != 1)
						return false;
			}
		return true;
	}


	static bool miller_rabin(long long n, long long k = 10) {
		if (n == 0 || n == 1)
			return false;
		else if (n == 2 || n == 3)
			return true;

		long long d = n - 1;
		long long s = 0;
		while (d % 2 == 0) {
			s++;
			d = d / 2;
		}

		long long nDec = n - 1;
		for (unsigned short i = 0; i < k; i++) {
			long long a = rand() % nDec;
			if (a == 0 || a == 1)
				a = a + 2;

			long long x = powClosed(a, d, n);
			if (x == 1 || x == nDec)
				continue;

			bool flag = false;
			for (unsigned short j = 0; j < s; j++) {
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


	static void quicksort(vector <pair <short, long long>>& a, long long start, long long end) {
		if (start >= end) {
			return;
		}

		long long pivot = partition(a, start, end);
		quicksort(a, start, pivot - 1);
		quicksort(a, pivot + 1, end);
	}
};



void contFraction() {
	cout << "\n\tРазложение чисел в цепную дробь";
	int a0, a1;
	cout << "\na0 = ";
	cin >> a0;
	cout << "a1 = ";
	cin >> a1;

	if (Pattern::usualEuclid(a0, a1) != 1) {
		cout << "\nНОД(" << a0 << ", " << a1 << ") != 1";
		return;
	}

	vector <int> fractions;
	int q = 1, r = 1;
	for (int a = 2; r != 0; a++)
	{
		q = a0 / a1;
		r = a0 % a1;
		cout << "\nq = " << q << ", a"<< a << " = " << r;

		fractions.push_back(q);
		if (r == 1)
		{
			fractions.push_back(a1);
			cout << "\nq = " << a1 << ", a" << a + 1 << " = " << 0;
			break;
		}
		a0 = a1;
		a1 = r;
	}

	vector <int> P{ 0, 1 };
	vector <int> Q{ 1, 0 };
	for (int i = 0; i < fractions.size(); i++) {
		P.push_back(fractions[i] * P[i + 1] + P[i]);
		Q.push_back(fractions[i] * Q[i + 1] + Q[i]);
	}

	cout << "\nЧислитель подходящих дробей: " << P[0];
	for (int i = 1; i < P.size(); i++)
		cout << ", " << P[i];
	cout << "\nЗнаменатель подходящих дробей: " << Q[0];
	for (int i = 1; i < Q.size(); i++)
		cout << ", " << Q[i];
}


void symbolJacobi() {
	cout << "\n\tСимвол Якоби";
	int a, p;
	cout << "\na = ";
	cin >> a;
	cout << "p = ";
	cin >> p;

	int res = 1;
	while (a != 1) {
		a = a % p;
		cout << "\nb = " << a << " (mod " << p << ") = " << a;
		if (abs(a) >= p / 2) {
			a = abs(a - p);
			res *= pow(-1, (p - 1) / 2);
			cout << ". Т.к. |b| > p/2, то по св-ву 4 и 5: b = " << a << ", j = j * -1^((p-1) / 2) = " << res;
		}
		else
			cout << ". Т.к. |b| <= p/2, то b = " << a;

		if (a % 2 == 0) {
			int t = 0;
			while (a % 2 == 0) {
				t++;
				a /= 2;
				if (a == 0)
				{
					cout << "\nj = " << 0;
					return;
				}
			}
			cout << "\nТ.к. b - чётное, то b = 2^t * a1 = 2^" << t << " * " << a;

			if (t % 2 == 1)
			{
				res *= pow(-1, (p * p - 1) / 8);
				cout << "\nТ.к. t - нечётное, то по св-ву 6: j = j * -1^((p^2 - 1) / 8) = " << res;
			}
		}

		swap(a, p);
		res *= pow(-1, ((p - 1) / 2) * ((a - 1) / 2));
		cout << "\nКвадратичный закон взаимности: ";
		cout << "a = " << a << ", p = " << p << ", j = -1^(((p-1) / 2) * ((a-1) / 2)) = " << res << endl;
	}
	cout << "\nj = " << res;
}


void gcTheorem()
{
	cout << "\n\tРешение СЛС";
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

	if (!Pattern::checkMutualSimplicity(raws))
		throw string{ "Модули m не являются попарно взаимно простыми!" };

	int M = 1;
	for (int i = 0; i < raws.size(); i++)
		M *= raws[i].second;
	cout << "\nM = " << M;

	int res = 0;
	for (int i = 0; i < raws.size(); i++) {
		int Mi = M / raws[i].second;
		cout << "\n\nM" << i + 1 << " = M / m" << i + 1 << " = " << Mi;

		for (int j = 1;; j++)
			if ((Mi * j) % raws[i].second == raws[i].first) {
				res += Mi * j;
				cout << "\nz : [M" << i + 1 << " * z" << i + 1 << "(mod m" << i + 1 << ") = " << raws[i].first << "] = " << j;
				break;
			}
	}

	cout << "\n\nРешение данной системы линейных сравнений: ";
	cout << "x = z1M1 + ... + z" << raws.size() - 1 << "M" << raws.size() - 1 << " = " << res % M << " (mod " << M << ")";
}


void gelfondThanks()
{
	cout << "\n\tОпределение порядка элемента";
	int a, m;
	cout << "\nПоле: ";
	cin >> m;
	cout << "Элемент: ";
	cin >> a;

	if (!Pattern::miller_rabin(m))
		throw string{ "Порядок группы " + to_string(m) + " не является простым числом!" };

	long long B = m - 1;
	long long r = round(sqrt(m)) + 1;
	vector <pair <short, long long>> as;
	for (unsigned short i = 1; i < r; i++)
		as.push_back(make_pair(i, Pattern::powClosed(a, i, m)));
	Pattern::quicksort(as, 0, as.size() - 1);

	cout << "\nШаг 1. B = |Z" << m << "| = " << B << ", r = [sqrt(B)] + 1 = " << r << "\n\tВычисляем ";
	for (int i = 0; i < r; i++)
		cout << "a^" << i + 1 << " = " << Pattern::powClosed(a, i + 1, m) << "  ";
	cout << "\n\tУпорядочиваем по 2-й координате пары (k, a^k): ";
	for (int i = 0; i < as.size(); i++)
		cout << "(" << as[i].first << ", " << as[i].second << ") ";


	long long a1 = (Pattern::powClosed(Pattern::advancedEuclid(a, m).first, r, m) + m) % m;
	cout << "\nШаг 2. a1 = a^-r = " << Pattern::powClosed(a, r, m) << "^-1 = " << a1;
	cout << "\n\tВычисляем a1^i (0 <= i < r) и проверяем, является ли a1^i 2-й координатой какой-нибудь пары выше:";

	for (unsigned short i = 0; i < r; i++)
	{
		long long a1i = Pattern::powClosed(a1, i, m);
		cout << "\n\t\ta1^" << i << " = " << a1i;
		pair <short, long long> k_ak = make_pair(-1, -1);
		for (short i = 0; i < as.size(); i++)
			if (a1i == as[i].second)
			{
				k_ak = as[i];
				break;
			}

		if (k_ak.first != -1)
		{
			long long res = k_ak.first + r * i;
			cout << " => k = " << k_ak.first << ", i = " << i << ". k + ri = " << k_ak.first << " + " << r << " * " << i << " = ";
			cout << k_ak.first + r * i << " = log" << a << "(1) = ord(" << a << ")";
			cout << "\n\tПроверка: a^" << res << " (mod m) = " << Pattern::powClosed(a, res, m);
			break;
		}
	}
}


void buildGroupEC()
{
	int field;
	cout << "\nРазмер поля: ";
	cin >> field;
	int *elField = new int[field];
	cout << "Минимальный и максимальный элемент поля: ";
	int min, max;
	cin >> min >> max;
	for (int i = 0; i < field; i++)
		elField[i] = min + i;

	int a, b;
	cout << "Параметры a, b: ";
	cin >> a >> b;

	int *sqrElField = new int[field];
	cout << endl;
	for (int i = 0; i < field; i++) {
		sqrElField[i] = elField[i] * elField[i] % field;
		if (sqrElField[i] > max)
			sqrElField[i] -= field;
	}

	vector <pair <int, int>> points;
	int offset = to_string(field).length() == 1 ? 2 : to_string(field).length();
	for (int i = 0; i < field; i++)
	{
		int x = elField[i];
		int yy = x * x * x + a * x + b;
		while (yy < min)
			yy += field;
		while (yy > max)
			yy -= field;
		cout << "X = " << setw(offset) << elField[i] << ", X^2 = " << setw(offset) << sqrElField[i] << ", ";
		cout << "X^3 + aX + b = " << setw(offset) << yy;
		
		for (int j = 0; j < field; j++)
			if (yy == sqrElField[j])
			{
				points.push_back(make_pair(x, elField[j]));
				cout << ", Y = " << setw(offset) << elField[j];
			}
		cout << endl;
	}

	cout << "\n\nТочки кривой: {O, ";
	for (int i = 0; i < points.size(); i++)
	{
		if (i == points.size() - 1)
			cout << "(" << points[i].first << ", " << points[i].second << ")}";
		else
			cout << "(" << points[i].first << ", " << points[i].second << "), ";
	}
	cout << "\nВсего точек: " << points.size() + 1;

	cout << "\n\nОпределение порядка элементов: ";
	pair <int, int> acc = make_pair(INT_MAX, INT_MAX);
	for (int i = 0; i < points.size(); i++)
	{
		acc = points[i];
		pair <int, int> P = points[i];
		cout << "\n\nP = (" << points[i].first << ", " << points[i].second << ")";

		for (int j = 2;; j++)
		{
			cout << "\n\t" << j << "P = " << j - 1 << "P + P = (" << acc.first << ", " << acc.second << ") + (" << P.first << ", " << P.second << ") = ";
			if (acc.first == P.first && (acc.second != P.second || (acc.second == 0 && P.second == 0)))
			{
				cout << "O \n\tПорядок точки = " << j;
				break;
			}
			if (acc.first == INT_MAX && acc.second == INT_MAX)
			{
				cout << "(" << P.first << ", " << P.second << ")";
				acc = P;
				break;
			}

			int rev2Y1 = Pattern::advancedEuclid(2 * acc.second, field).first;
			while (rev2Y1 < min)
				rev2Y1 += field;
			while (rev2Y1 > max)
				rev2Y1 -= field;
			int revSubX1X2 = Pattern::advancedEuclid(acc.first - P.first >= min ? acc.first - P.first : acc.first - P.first + field, field).first;
			while (revSubX1X2 < min)
				revSubX1X2 += field;
			while (revSubX1X2 > max)
				revSubX1X2 -= field;

			int alpha = P.first == acc.first ? (3 * acc.first * acc.first + a) * rev2Y1 % field : (acc.second - P.second) * revSubX1X2 % field;
			while (alpha < min)
				alpha += field;
			while (alpha > max)
				alpha -= field;

			int x3 = alpha * alpha - acc.first - P.first;
			while (x3 < min)
				x3 += field;
			while (x3 > max)
				x3 -= field;

			int y3 = alpha * (acc.first - x3) - acc.second;
			while (y3 < min)
				y3 += field;
			while (y3 > max)
				y3 -= field;

			cout << "(" << x3 << ", " << y3 << ")";
			if (acc.first == P.first)
			{
				cout << "\n\t\tТ.к. x1 = x2, то: alpha = (3*x1^2 + a)(2 * y1)^-1 = ";
				cout << "(3*" << acc.first << "^2 + " << a << ")(2 * ";
				cout << acc.second << ")^-1 (mod " << field << ") = " << alpha;
			}
			else
			{
				cout << "\n\t\tТ.к. x1 != x2, то: alpha = (y1 - y2)(x1 - x2)^-1 = ";
				cout << "(" << acc.second << " - " << P.second << ")(";
				cout << acc.first << " - " << P.first << ")^-1 (mod " << field << ") = " << alpha;
			}
			cout << "\n\t\tx3 = alpha^2 - x1 - x2 = " << alpha << "^2 - " << acc.first << " - " << P.first << " (mod " << field << ") = " << x3;
			cout << "\n\t\ty3 = alpha(x1 - x3) - y1 = " << alpha << "(" << acc.first << " - " << x3 << ") - " << acc.second;
			cout << " (mod " << field << ") = " << y3;

			acc = make_pair(x3, y3);
		}
	}
}


int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));
	cout << "\tКонтрольная по ТЧМК";

	int choice;
	for (;;)
	{
		cout << "\n1 - Цепная и подходящая дробь \n2 - символ Якоби \n3 - Решение системы линейных сравнений";
		cout << "\n4 - Определение порядка элемента в группе \n6 - Построение группы ЭК и определение порядка элемента \n";
		cin >> choice;

		switch (choice)
		{
		case 1:
			contFraction();
			cout << endl;
			break;
		case 2:
			symbolJacobi();
			cout << endl;
			break;
		case 3:
			gcTheorem();
			cout << endl;
			break;
		case 4:
			gelfondThanks();
			cout << endl;
			break;
		case 6:
			buildGroupEC();
			cout << endl;
			break;
		default:
			cout << "\nIncorrect! Try again \n";
		}
	}

	cout << endl;
	return 0;
}