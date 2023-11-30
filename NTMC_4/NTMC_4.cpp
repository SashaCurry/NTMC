#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using namespace boost::multiprecision;

class Factorization
{
private:
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
			bool allNot_1 = true;

			for (unsigned short k = 0; k < xs.size(); k++)
			{
				dk = usualEuclid((xNext - xs[k] + n) % n, n);
				if (dk == n)
					goto gen_x0;
				else if (dk == 1)
					allNot_1 = false;
			}

			if (allNot_1)
				return dk;
			xCur = xNext;
			xs.push_back(xNext);
		}

		goto gen_x0;
		return 0;
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

			cpp_int res = Factorization::roMethodPollarda(n, eps);
			cout << endl << n << " = " << res << " * " << n / res;
		}

		cout << "\n\n";
	}
}
