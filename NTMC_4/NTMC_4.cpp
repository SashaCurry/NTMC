#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Factorization
{
private:
	static int binaryEuclid(int a, int b)
	{
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

public:
	static long long roMethodPollarda(long long n, double epsilon)
	{
		long long T = round(sqrt(2 * sqrt(n) * log(1 / epsilon))) + 1;
		vector <long long> xs;

	gen_x0:
		xs.clear();
		xs.push_back(rand() % (n - 1) + 1);
		long long xCur = xs.back(), xNext;

		for (unsigned short i = 1; i <= T; i++)
		{
			xNext = (xCur * xCur + 1) % n;
			long long dk;
			bool allNot_1 = true;

			for (unsigned short k = 0 ; k < xs.size(); k++)
			{
				dk = binaryEuclid(xNext - xs[k], n);
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
	cout << "\tФакторизация целых чисел \n1 - Факторизация p-методом Полларда \n";
	cout << "2 - Факторизация (р-1)-методом Полларда \n3 - Факторизация методом цепных дробей"; 

	long long res = Factorization::roMethodPollarda(12345, 0.1);
	cout << endl << "12345 = " << res << " * " << 12345 / res;
}
