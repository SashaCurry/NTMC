#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cmath>
#include <iomanip>

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


	static long long usualEuclid(long long a, long long b) {
		if (a < b)
			swap(a, b);
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };
		else if (b == 0)
			return a;

		long long r = a % b;
		return usualEuclid(b, r);
	}


	static pair <long long, long long> advancedEuclid(long long a, long long b) {
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };

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
};



class discrLogarithm
{
private:
	//Проверка первообразного элемента
	static void checkPrimitEl(long long g, long long m)
	{
		if (g >= m)
			throw string{ "Первообразный элемент превышает порядок!" };

		long long x = g;
		for (long long i = 2; i < m; i++)
		{
			x = (x * g) % m;
			if (i != m - 1 && x == 1)
				throw string{ "Элемент " + to_string(g) + " не является образующим элементом!" };
		}
	}


	static pair <short, long long> findEl(vector <pair <short, long long>> as, long long el)
	{
		for (unsigned short i = 0; i < as.size(); i++)
			if (as[i].second == el)
				return as[i];
		return make_pair(0, 0);
	}


	//Вычисляется новая тройка во 2-ом алгоритме
	static vector <long long> newThrees(long long m, long long a, long long b, vector <long long> elPrev)
	{
		long long u1 = m / 3.0;
		long long u2 = u1 * 2;
		long long u3 = m;

		if (elPrev[0] < u1)
			return vector <long long> {b * elPrev[0] % m, elPrev[1], (elPrev[2] + 1) % (m - 1)};
		else if (elPrev[0] < u2)
			return vector <long long> {elPrev[0] * elPrev[0] % m, 2 * elPrev[1] % (m - 1), 2 * elPrev[2] % (m - 1) };
		else
			return vector <long long> {a * elPrev[0] % m, (elPrev[1] + 1) % (m - 1), elPrev[2]};
	}


	//Создаётся факторная база
	static vector <short> createBase(short B)
	{
		vector <short> base;
		for (unsigned short i = 1; i <= B; i++)
			if (Pattern::miller_rabin(i))
				base.push_back(i);
		return base;
	}


	//Каноническое представление числа относительно факторной базы
	static pair <map <short, long long>, short> factorizationNum(vector <short> base, long long n)
	{
		pair <map <short, long long>, short > res;
		for (unsigned short i = 0; i < base.size(); i++)
			res.first.insert(make_pair(base[i], 0));

		for (unsigned short i = 0; i < base.size(); i++)
			while (n % base[i] == 0)
			{
				res.first[base[i]] += 1;
				n /= base[i];
			}

		res.second = n;
		return res;
	}


	//Проверка, имеет ли СЛУ решение
	static bool haveSolutionSLU(vector <vector <long long>> matrix)
	{
		for (unsigned short i = 0, rawSize = matrix[0].size() - 1; i < rawSize; i++)
		{
			bool flag = false;
			for (unsigned short j = 0, colSize = matrix.size(); j < colSize; j++)
				if (matrix[j][i] > 0) {
					flag = true;
					break;
				}
			if (!flag)
				return false;
		}
		return true;
	}


	//Перестановка строк, чтобы на главной диагонали не было нулей
	static bool swapColms(vector <vector <long long>>& matrix, short indexColm)
	{
		for (unsigned short i = indexColm + 1; i < matrix.size(); i++)
			if (matrix[i][indexColm] != 0)
			{
				swap(matrix[indexColm], matrix[i]);
				return true;
			}
		return false;
	}


	//Умножение строки на число
	static vector <long long> multRawToNum(vector <long long> raw, long long num, long long field) {
		vector <long long> res;
		for (unsigned short i = 0, rawSize = raw.size(); i < rawSize; i++)
		{
			int x = (raw[i] * num) % field;
			while (x < 0)
				x += field;
			res.push_back(x);
		}
		return res;
	}


	//Сложение строк
	static vector <long long> addRaws(vector <long long> a, vector <long long> b, long long field) {
		vector <long long> res;
		for (unsigned short i = 0; i < a.size(); i++) {
			long long x = (a[i] + b[i]) % field;
			while (x < 0)
				x += field;
			res.push_back(x);
		}
		return res;
	}

public:
	static long long gelfondThanks(long long m, long long a, long long b)
	{
		if (!Pattern::miller_rabin(m))
			throw string{ "Порядок группы " + to_string(m) + " не является простым числом!" };
		checkPrimitEl(a, m);
		
		long long r = round(std::sqrt(m)) + 1;
		vector <pair <short, long long>> as;
		for (unsigned short i = 1; i < r; i++)
			as.push_back(make_pair(i, Pattern::powClosed(a, i, m)));
		Pattern::quicksort(as, 0, as.size() - 1);

		long long a1 = (Pattern::powClosed(Pattern::advancedEuclid(a, m).first, r, m) + m) % m;
		set <long long> k_ri;
		for (unsigned short i = 0; i < r; i++)
		{
			long long a1i = Pattern::powClosed(a1, i, m);
			pair <short, long long> k_ak = findEl(as, a1i * b % m);
 			if (k_ak.first != 0)
				k_ri.insert((k_ak.first + r * i) % m);
		}

		for (auto i = k_ri.begin(); i != k_ri.end(); i++)
			if (Pattern::powClosed(a, *i, m) == b)
				return *i;
		for (unsigned short i = 1;; i++)
			if (Pattern::powClosed(a, i, m) == b)
				return i;
	}


	static long long roMethodPollarda(long long m, long long a, long long b, double eps)
	{
		if (!Pattern::miller_rabin(m))
			throw string{ "Порядок группы " + to_string(m) + " не является простым числом!" };
		checkPrimitEl(a, m);

		long long k = round(std::sqrt(2 * std::sqrt(m - 1) * log(1 / eps))) + 1;
		unsigned short count = 0;
	step2:
		count++;
		long long s = rand() % (m - 1);

		vector <vector <long long>> threes(2 * k + 2);
		threes[0] = { Pattern::powClosed(a, s, m), s, 0 };
		threes[1] = newThrees(m, a, b, threes[0]);
		for (unsigned short i = 2; i <= k; i++)
		{
			threes[i] = newThrees(m, a, b, threes[i - 1]);
			vector <long long> helpThrees = newThrees(m, a, b, threes[2 * i - 2]);
			threes[2 * i] = newThrees(m, a, b, helpThrees);
		}

		for (unsigned short i = 2; i <= k; i++)
			if (threes[i][0] == threes[2 * i][0])
			{
				long long mDec = m - 1;
				long long d = Pattern::usualEuclid((threes[i][2] - threes[2 * i][2] + mDec) % mDec, mDec);
				if (std::sqrt(m) < d && d <= m)
					goto step2;
				
				long long alphaRes = (threes[2 * i][1] - threes[i][1] + mDec) % mDec;
				long long betaRes = (threes[i][2] - threes[2 * i][2] + mDec) % mDec;
				vector <long long> res;
				for (long long x = 1; x < m && res.size() != d; x++)
					if (betaRes * x % mDec == alphaRes)
						res.push_back(x);

				for (unsigned short j = 0; j < res.size(); j++)
					if (Pattern::powClosed(a, res[j], m) == b)
						return res[j];
			}
		if (count < m / eps)
			goto step2;

		throw string{ "Дискретный логарифм вычислить не удалось!" };
		return 0;
	}


	static long long indexMethod(long long p, long long a, long long h)
	{
		if (!Pattern::miller_rabin(p))
			throw string{ "Порядок группы " + to_string(p) + " не является простым числом!" };
		checkPrimitEl(a, p);

		short B = p < 8193 ? 13 : round(log2(p));
		vector <short> base = createBase(B);

	step2:
		vector <vector <long long>> canonDecompose;
		while (canonDecompose.size() != base.size() + 10)
		{
			int m = rand() % (p - 2) + 1;
			long long b = Pattern::powClosed(a, m, p);

			pair <map <short, long long>, long long> factorsNum = factorizationNum(base, b);
			if (factorsNum.second != 1)
				continue;

			vector <long long> degs;
			for (auto i = factorsNum.first.begin(); i != factorsNum.first.end(); i++)
				degs.push_back(i->second);
			degs.push_back(m);
			canonDecompose.push_back(degs);
		}
		vector <vector <long long>> copyCanonDecompose = canonDecompose;

		if (!haveSolutionSLU(canonDecompose))
			goto step2;
		for (unsigned short i = 0; i < canonDecompose[0].size() - 1; i++)
			if (canonDecompose[i][i] == 0)
				if (!swapColms(canonDecompose, i))
					goto step2;

		long long pDec = p - 1;
		for (unsigned short i = 0; i < canonDecompose[0].size(); i++)
		{
			if (canonDecompose[i][i] > 1) {
				for (unsigned short j = i + 1; j < canonDecompose.size(); j++)
					if (canonDecompose[j][i] != 0 && Pattern::usualEuclid(canonDecompose[i][i], canonDecompose[j][i]) == 1)
					{
						pair <long long, long long> coefs = Pattern::advancedEuclid(canonDecompose[i][i], canonDecompose[j][i]);
						canonDecompose[i] = addRaws(multRawToNum(canonDecompose[i], coefs.first, pDec),
													multRawToNum(canonDecompose[j], coefs.second, pDec),
													pDec);

						break;
					}
				if (canonDecompose[i][i] > 1)
					goto step2;
			}
			for (unsigned short k = i + 1; k < canonDecompose.size(); k++)
			{
				canonDecompose[k] = addRaws(canonDecompose[k], multRawToNum(canonDecompose[i], -canonDecompose[k][i], pDec), pDec);
				if (k < canonDecompose[0].size() - 1 && canonDecompose[k][k] == 0)
				{
					if (!swapColms(canonDecompose, k))
						goto step2;
					k--;
				}
			}
		}

		while (canonDecompose.size() != base.size())
			canonDecompose.pop_back();

		cout << "\nСЛУ: ";
		for (unsigned short i = 0, size = copyCanonDecompose.size(); i < size; i++)
		{
			string res = "\n(";
			for (unsigned short j = 0, iSize = copyCanonDecompose[i].size(); j < iSize; j++)
				res += j == copyCanonDecompose[i].size() - 1 ? to_string(copyCanonDecompose[i][j]) + ")" : to_string(copyCanonDecompose[i][j]) + ", ";
			cout << res;
		}

		cout << "\n\nРазрешённая СЛУ: ";
		for (unsigned short i = 0, sizeP = to_string(p).length(), size = canonDecompose.size(); i < size; i++)
		{
			cout << "\n(" << canonDecompose[i][0] << ", ";
			for (unsigned short j = 1, iSize = canonDecompose[i].size(); j < iSize; j++)
			{
				if (j == canonDecompose[i].size() - 1)
					cout << setw(sizeP) << canonDecompose[i][j] << ")";
				else
					cout << setw(sizeP) << canonDecompose[i][j] << ", ";
			}
		}

		vector <long long> solSLU(canonDecompose.size(), 0);
		for (short i = canonDecompose.size() - 1; i >= 0; i--)
			for (short j = i + 1; j < canonDecompose[i].size(); j++)
			{
				solSLU[i] = j == canonDecompose[i].size() - 1 ? (solSLU[i] + canonDecompose[i][j]) % pDec : solSLU[i] - 1 * canonDecompose[i][j] * solSLU[j];
				while (solSLU[i] < 0)
					solSLU[i] += pDec;
			}

		cout << "\n\nРешение СЛУ: (";
		for (unsigned short i = 0, sizeSolSLU = solSLU.size(); i < sizeSolSLU; i++)
		{
			if (i == sizeSolSLU - 1)
				cout << solSLU[i] << ")";
			else
				cout << solSLU[i] << ", ";
		}


		while (true)
		{
			int m = rand() % (p - 2) + 1;
			long long b = Pattern::powClosed(a, m, p) * h % p;

			pair <map <short, long long>, long long> factorsNum = factorizationNum(base, b);
			if (factorsNum.second != 1)
				continue;

			long long res = 0;
			for (unsigned short i = 0; i < base.size(); i++)
				res = (res + factorsNum.first[base[i]] * solSLU[i]) % (p - 1);
			res = (res - m) % (p - 1);
			return res;
		}
	}
};



int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	cout << "\tДискретное логарифмирование в конечном поле";

	unsigned short choice;
	for (;;)
	{
		cout << "\n1 - Метод Гельфонда-Шенкса \n2 - ро-метод Полларда \n3 - индекс-метод \n";
		cin >> choice;

		long long p, a, b;
		cout << "\nПорядок конечной циклической группы p: ";
		cin >> p;
		cout << "Образующий элемент группы a: ";
		cin >> a;
		cout << "Элемент группы b: ";
		cin >> b;

		try
		{
			if (choice == 1)
			{
				long long res = discrLogarithm::gelfondThanks(p, a, b);
				cout << "\nx = " << res << "\nПроверка: a^x (mod m) = " << Pattern::powClosed(a, res, p);
			}
			else if (choice == 2)
			{
				cout << "Значение epsilon: ";
				double eps;
				cin >> eps;

				long long res = discrLogarithm::roMethodPollarda(p, a, b, eps);
				cout << "\n\nx = " << res << "\nПроверка: a^x (mod m) = " << Pattern::powClosed(a, res, p);
			}
			else if (choice == 3)
			{
				long long res = discrLogarithm::indexMethod(p, a, b);
				while (res < 0)
					res += p - 1;
				cout << "\nx = " << res << "\nПроверка: a^x (mod m) = " << Pattern::powClosed(a, res, p);
			}
		}
		catch (string& error)
		{
			cout << endl << error;
		}

		cout << endl;
	}
}