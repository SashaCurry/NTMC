#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using namespace boost::multiprecision;

class Pattern {
private:
	static vector <cpp_int> deg2(cpp_int el, cpp_int n) {//Раскладываем число на степени двойки
		vector <cpp_int> res;
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


	static cpp_int multMod(cpp_int n, cpp_int mod, vector <pair <cpp_int, cpp_int>> lst) {//Умножаем число по модулю
		if (lst.size() == 1) {
			cpp_int res = 1;
			for (int i = 0; i < lst[0].second; i++)
				res = res * lst[0].first % mod;
			return res;
		}
		else if (lst[0].second == 1) {
			cpp_int el = lst[0].first;
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


	static int partition(vector <cpp_int>& a, int start, int end) {
		cpp_int pivot = a[end];
		int pIndex = start;

		for (int i = start; i < end; i++) {
			if (a[i] <= pivot) {
				swap(a[i], a[pIndex]);
				pIndex++;
			}
		}

		swap(a[pIndex], a[end]);
		return pIndex;
	}
public:
	static cpp_int powClosed(cpp_int x, cpp_int y, cpp_int mod) {//Возводим число в степени по модулю
		if (y == 0)
			return 1;

		vector <cpp_int> lst = deg2(1, y);
		vector <pair <cpp_int, cpp_int>> xDegs;
		for (int i = 0; i < lst.size(); i++)
			xDegs.push_back(make_pair(x, lst[i]));

		cpp_int res = multMod(x, mod, xDegs);
		return res;
	}


	//Возведение в степень
	static cpp_int pow(cpp_int x, cpp_int y) {
		cpp_int res = 1;
		for (int i = 0; i < y; i++)
			res *= x;
		return res;
	}


	//Символ Лежандра через критерий Эйлера
	static cpp_int symbolLegendre(cpp_int a, cpp_int p) {
		if (a == 0)
			return 0;
		cpp_int res = powClosed(a, (p - 1) / 2, p);
		return res == 1 ? 1 : -1;
	}


	//Символ Якоби
	static cpp_int symbolJacobi(cpp_int  a, cpp_int  b) {
		if (usualEuclid(a, b) != 1)
			return 0;

		cpp_int  r = 1;
		if (a < 0) {
			a = abs(a);
			if (b % 4 == 3)
				r = -r;
		}

		while (a != 0) {
			cpp_int t = 0;
			while (a % 2 == 0) {
				t++;
				a /= 2;
			}
			if (t % 2 == 1 && (b % 8 == 3 || b % 8 == 5))
				r = -r;

			if (a % 4 == 3 && b % 4 == 3)
				r = -r;

			cpp_int c = a;
			a = b % c;
			b = c;
		}
		return r;
	}


	//Из бинарной формы в десятичную
	static cpp_int decForm(string x) {
		cpp_int res = 0, deg = 1;
		if (!x.empty() && x.back() == '1')
			res += 1;
		for (short i = x.length() - 2; i >= 0; i--) {
			deg = deg * 2;
			if (x[i] == '1')
				res += deg;
		}
		return res;
	}


	//Из десятчной формы в бинарную
	static string binForm(cpp_int x) {
		string bitter = "";
		while (x != 0) {
			bitter = (x % 2 == 0 ? "0" : "1") + bitter;
			x = x / 2;
		}
		if (bitter == "")
			return "0";
		return bitter;
	}


	//Тест на простоту Миллера-Рабина
	static bool miller_rabin(cpp_int n, int k = 10) {
		if (n == 0)
			return false;
		else if (n == 1 || n == 2 || n == 3)
			return true;

		cpp_int d = n - 1;
		cpp_int s = 0;
		while (d % 2 == 0) {
			s++;
			d = d / 2;
		}

		cpp_int nDec = n - 1;
		for (int i = 0; i < k; i++) {
			cpp_int a = rand() % nDec;
			if (a == 0 || a == 1)
				a = a + 2;

			cpp_int x = powClosed(a, d, n);
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


	//Обычный алгоритм Евклида
	static cpp_int usualEuclid(cpp_int a, cpp_int b) {
		if (a < b)
			swap(a, b);
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };
		else if (b == 0)
			return a;

		cpp_int r = a % b;
		return usualEuclid(b, r);
	}


	//Бинарный алгоритм Евклида
	static int binaryEuclid(int a, int b) {
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
	static pair <cpp_int, cpp_int> advancedEuclid(cpp_int a, cpp_int b) {
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };

		cpp_int q, aPrev = a, aCur = b, aNext = -1;
		cpp_int xPrev = 1, xCur = 0, xNext;
		cpp_int yPrev = 0, yCur = 1, yNext;
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


	//Функция Эйлера
	static cpp_int funEuler(cpp_int n) {
		cpp_int res = 1;
		for (int i = 2; i < n; i++)
			if (usualEuclid(n, i) == 1)
				res++;
		return res;
	}


	//Хэш-функция
	static string hashing(string str) {
		string res;
		hash <string> hashStr;
		return to_string(hashStr(res));
	}


	//Генерация простого числа и его первообразного корня
	static pair <cpp_int, cpp_int> generateGN() {
		cpp_int q = rand() % 1000;
		while (funEuler(q) != q - 1)
			q++;

		cpp_int s, n = 2, nDec;
		while (!miller_rabin(n)) {
			string sBin = "";
			int sBinSize = rand() % 50 + 1;
			for (int i = 0; i < sBinSize; i++)
				sBin = sBin + to_string(rand() % 2);
			s = decForm(sBin);

			n = (q * s) + 1;
			nDec = n - 1;
		}

		cpp_int a = 2;
		while (nDec > a) {
			cpp_int g = powClosed(a, nDec / q, n);
			if (g == 1) {
				a++;
				continue;
			}

			return make_pair(g, n);
		}
		return make_pair(0, 0);//Cтрока для обхода warning'а в Linux
	}


	//Квадратный корень в Zp
	static cpp_int sqrtFromZp(cpp_int a, cpp_int p) {
		a = a % p;
		cpp_int m = 0, q = p - 1;
		while (q % 2 != 1) {
			m++;
			q /= 2;
		}

		cpp_int b = rand() % p;
		while (symbolLegendre(b, p) != -1)
			b = (b + 1) % p;

		vector <cpp_int> kArr;
		for (int i = 1;; i++) {
			cpp_int k = 0;
			while (powClosed(a, pow(2, k) * q, p) != 1)
				k++;
			kArr.push_back(k);
			if (k == 0)
				break;
			a = (a * pow(b, pow(2, m - kArr.back()))) % p;
		}

		cpp_int r = powClosed(a, (q + 1) / 2, p);
		for (int i = kArr.size() - 2; i >= 0; i--)
			r = (r * advancedEuclid(pow(b, pow(2, m - kArr[i] - 1)), p).first) % p;

		return r;
	}


	static cpp_int generateSimpleNum(unsigned short k) {
		cpp_int q = rand() % 1000;
		while (funEuler(q) != q - 1)
			q++;

		cpp_int s, n = 2, nDec;
		while (!miller_rabin(n)) {
			string sBin = "1";
			int sBinSize = rand() % (k / 2) + k / 2;
			for (int i = 0; i < sBinSize; i++)
				sBin = sBin + to_string(rand() % 2);
			s = decForm(sBin);

			n = (q * s) + 1;
			nDec = n - 1;
		}

		return n;
	}


	//Быстрая сортировка
	static void quicksort(vector <cpp_int>& a, int start, int end) {
		if (start >= end) {
			return;
		}

		int pivot = partition(a, start, end);
		quicksort(a, start, pivot - 1);
		quicksort(a, pivot + 1, end);
	}
};



class Factorization
{
private:
	static cpp_int baseForPollard(cpp_int n, int B)
	{
		cpp_int res = 1;

		if (B < 2)
			throw string{ "База B должна быть > 1!" };
		if (B >= 2)
			res *= Pattern::pow(2, cpp_int(log(cpp_dec_float_50(n)) / 0.69314718));
		if (B >= 3)
			res *= Pattern::pow(3, cpp_int(log(cpp_dec_float_50(n)) / 1.09861228));

		for (int i = 5; i <= B; i++)
			if (Pattern::miller_rabin(i))
				res *= Pattern::pow(i, cpp_int(log(cpp_dec_float_50(n)) / log(cpp_dec_float_50(i))));
		return res;
	}


	static vector <cpp_int> factorsNum(cpp_int n)
	{
		vector <cpp_int> res;
		if (n < 0)
		{
			res.push_back(-1);
			n = -n;
		}

		while (!Pattern::miller_rabin(n))
		{
			cpp_int factor = roMethodPollarda(n, 0.1);
			res.push_back(factor);
			n /= factor;
		}

		res.push_back(n);
		return res;
	}


	static set <cpp_int> baseForBM(vector <vector <cpp_int>> factorsP)
	{
		set <cpp_int> res;

		map <cpp_int, unsigned short> countFactors;
		for (unsigned short i = 0; i < factorsP.size(); i++)
		{
			if (countFactors.find(factorsP[i][0]) == countFactors.end())
				countFactors.insert(make_pair(factorsP[i][0], 1));

			unsigned short deg = 1;
			for (unsigned short j = 1; j < factorsP[i].size(); j++)
			{
				if (factorsP[i][j] == factorsP[i][j - 1])
					deg++;
				else
				{
					if (deg % 2 == 0)
						countFactors[factorsP[i][j - 1]] += 2;
					else
						countFactors[factorsP[i][j - 1]] += 1;
					deg = 1;
				}
			}

			if (deg % 2 == 0)
				countFactors[factorsP[i].back()] += 2;
			else
				countFactors[factorsP[i].back()] += 1;
		}

		for (auto i = countFactors.begin(); i != countFactors.end(); i++)
			if (i->second >= 2)
				res.insert(i->first);

		if (res.find(1) != res.end())
			res.erase(1);
		return res;
	}


	static vector <vector <unsigned int>> getValueEs(vector <cpp_int> P, set <cpp_int> base, cpp_int n)
	{
		P.erase(P.begin());
		vector <vector <unsigned int>> es;
		for (unsigned short i = 0; i < P.size(); i++)
		{
			cpp_int Pdeduct = P[i] * P[i] % n;
			Pdeduct = Pdeduct > n / 2 ? Pdeduct -= n : Pdeduct;

			vector <unsigned int> e;
			e.push_back(Pdeduct < 0 ? 1 : 0);
			Pdeduct = abs(Pdeduct);
			for (auto j = ++base.begin(); j != base.end(); j++)
			{
				unsigned int deg = 0;
				while (Pdeduct % *j == 0)
				{
					deg++;
					Pdeduct /= *j;
				}
				e.push_back(deg);
			}

			if (Pdeduct == 1)
				es.push_back(e);
			else
				es.push_back(vector <unsigned int> (base.size(), 0));
		}

		return es;
	}


	static cpp_int xorRaws(vector <unsigned int> a, vector <unsigned int> b)
	{
		unsigned int res = 0;
		for (unsigned short i = 0; i < a.size(); i++)
			res += a[i] + b[i];
		return res % 2;
	}
public:
	static cpp_int roMethodPollarda(cpp_int n, cpp_dec_float_50 eps)
	{
		cpp_int T = sqrt(2 * sqrt(n) * cpp_int(log(1 / eps))) + 1;
		vector <cpp_int> xs;

	gen_x0:
		cpp_int xCur = rand() % (n - 1) + 1, xNext;
		xs.clear();
		xs.push_back(xCur);

		for (unsigned short i = 1; i <= T; i++)
		{
			xNext = (xCur * xCur + 1) % n;
			cpp_int dk;

			for (unsigned short k = 0; k < xs.size(); k++)
			{
				dk = Pattern::usualEuclid((xNext - xs[k] + n) % n, n);
				if (1 < dk && dk < n)
					return dk;
				else if (dk == n)
					goto gen_x0;
			}

			xCur = xNext;
			xs.push_back(xNext);
		}

		goto gen_x0;
		return 0;
	}


	static cpp_int roDecMethodPollarda(cpp_int n, int B)
	{
		if (Pattern::miller_rabin(n))
			throw string{ "Число " + to_string(n) + " - простое!" };

	step0:
		cpp_int T = baseForPollard(n, B);

		unsigned short countA = 0;
	step1:
		cpp_int a = rand() % (n - 2) + 2;
		countA++;
		cpp_int d = Pattern::usualEuclid(a, n);
		if (1 < d && d < n)
			return d;

		cpp_int b = Pattern::powClosed(a, T, n) - 1;
		cpp_int n1 = Pattern::usualEuclid(b, n);
		if (1 < n1 && n1 < n)
			return n1;
		else if (n1 == 1)
		{
			B++;
			goto step0;
		}
		else if (n1 == n)
		{
			if (countA == 10)
			{
				countA = 0;
				B--;
				goto step0;
			}
			goto step1;
		}
	}


	static cpp_int BrillhartMorrison(cpp_int n)
	{
		if (Pattern::miller_rabin(n))
			throw string{ "Число " + to_string(n) + " - простое!" };
		else if (sqrt(n) * sqrt(n) == n)
			throw string{ "Число " + to_string(n) + " является квадратом некоторого числа!" };

		vector <cpp_int> P{ 1, sqrt(n) };
		cpp_int a = sqrt(n);
		cpp_dec_float_50 x = sqrt(cpp_dec_float_50(n)) - cpp_dec_float_50(a);
		vector <vector <cpp_int>> factorsP;

		cpp_int Pdeduct = P.back() * P.back() % n;
		Pdeduct = Pdeduct > n / 2 ? Pdeduct -= n : Pdeduct;
		factorsP.push_back(factorsNum(Pdeduct));
		Pattern::quicksort(factorsP.back(), 0, factorsP.back().size() - 1);

		for (unsigned short k = 1; k <= log2(cpp_dec_float_50(n)); k++)
		{
			a = cpp_int(1 / x);
			x = 1 / x - cpp_dec_float_50(a);
			P.push_back((cpp_int(a) * P.back() + P[P.size() - 2]) % n);

			Pdeduct = P.back() * P.back() % n;
			Pdeduct = Pdeduct > n / 2 ? Pdeduct -= n : Pdeduct;
			factorsP.push_back(factorsNum(Pdeduct));
			Pattern::quicksort(factorsP.back(), 0, factorsP.back().size() - 1);
		}

		cout << "\nPk: { ";
		for (unsigned short i = 0; i < P.size(); i++)
			cout << P[i] << " ";
		cout << "}";

		set <cpp_int> base = baseForBM(factorsP);
		cout << "\nBase: { ";
		for (auto i = base.begin(); i != base.end(); i++)
			cout << *i << " ";
		cout << "}";

		vector <vector <unsigned int>> es = getValueEs(P, base, n);
		cout << endl;
		for (unsigned short i = 0; i < es.size(); i++)
		{
			cout << "\n(";
			for (unsigned short j = 0; j < es[i].size(); j++)
				cout << es[i][j] << " ";
			cout << ")";
		}

		P.erase(P.begin());
		for (unsigned short i = 0; i < es.size(); i++)
			for (unsigned short j = i + 1; j < es.size(); j++)
				if (xorRaws(es[i], es[j]) == 0)
				{
					cpp_int s = P[i] * P[j] % n;
					cpp_int t = 1;

					unsigned short iter = 1;
					for (auto k = ++base.begin(); k != base.end(); k++, iter++)
					{
						unsigned int gamma = (es[i][iter] + es[j][iter]) / 2;
						t *= Pattern::pow(*k, gamma);
					}

					cpp_int q = Pattern::usualEuclid(s + t, n);
					if (q != n && q != 1)
						return q;
					q = Pattern::usualEuclid(s - t, n);
					if (q != n && q != 1)
						return q;
				}

		return roMethodPollarda(n, 0.1);
	}
};




int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	for (;;)
	{
		cout << "\tФакторизация целых чисел \n1 - Факторизация p-методом Полларда \n";
		cout << "2 - Факторизация (р-1)-методом Полларда \n3 - Факторизация методом цепных дробей \n";
		unsigned short x;
		cin >> x;

		if (x == 1)
		{
			cpp_int n;
			cpp_dec_float_50 eps;
			cout << "\nСоставное число n: ";
			cin >> n;
			cout << "Значение eps, 0 < eps < 1: ";
			cin >> eps;

			try
			{
				cpp_int res = Factorization::roMethodPollarda(n, eps);
				cout << endl << n << " = " << res << " * " << n / res;
			}
			catch (string& error)
			{
				cout << endl << error;
			}
		}
		else if (x == 2)
		{
			cpp_int n;
			int B;
			cout << "\nСоставное число n: ";
			cin >> n;
			cout << "База В: ";
			cin >> B;

			try
			{
				cpp_int res = Factorization::roDecMethodPollarda(n, B);
				cout << endl << n << " = " << res << " * " << n / res;
			}
			catch (string& error)
			{
				cout << endl << error;
			}
		}
		else if (x == 3)
		{
			cpp_int n;
			cpp_dec_float_50 a;
			cout << "\nСоставное число n: ";
			cin >> n;

			try
			{
				cpp_int res = Factorization::BrillhartMorrison(n);
				cout << "\n\n" << n << " = " << res << " * " << n / res;
			}
			catch (string& error)
			{
				cout << endl << error;
			}
		}

		cout << "\n\n";
	}
}
