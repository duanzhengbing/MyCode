#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <iomanip>
#include <ctime>
using namespace std;


double Mysqrt(double num, double error)
{
	double low = 0, high = num;
	double guess = 0;
	double last = 0;
	do
	{
		last = guess;
		guess = (low + high) / 2;
		double tmp = guess * guess;
		if(tmp > num)
			high = guess;
		else
			low = guess;
	}while(std::abs(last - guess) > error);

	return guess;
}

double sqrtNewton(double num, double error)
{
	double x = num;
	double last  = 0;
	
	do
	{
		last = x;
		x = (x + num / x) / 2;
	}while(last - x > error);

	return x;
}
float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86- (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy

	return 1/x;
}

/**
 * 牛顿迭代法计算立方根
 * @param  num   [description]
 * @param  error [description]
 * @return       [description]
 */
double sqrt3Newton(double num, double error)
{
	double x = num;
	double last = 0;
	do
	{
		last = x;
		x = (2 * x + num / (x * x)) / 3;
	}while(last - x > error);

	return x;
}
double sqrt4Newton(double num, double error)
{
	double x = num;
	double last = 0;
	do
	{
		last = x;
		x = (3 * x + num / (x * x * x)) / 4;
	}while(last - x > error);

	return x;
}

/**
 * 牛顿迭代法计算n次方
 * @param  num [description]
 * @param  n   [description]
 * @return     [description]
 */
double sqrtNewton_n(double num, int n)
{
	double x = num;
	double last = 0;
	double error = 0.00000000000001;
	do{
		last = x;
		x = ((n - 1) * x + num /(std::pow(x, n - 1))) / n;
	}while(last - x > error);

	return x;
}

int main(int argc, char const *argv[])
{
	// double error = 0.000000000000001;

	// cout << std::setprecision(20) << Mysqrt(6553500000, error) << endl;
	// cout << std::setprecision(20) << sqrtNewton(6553500000, error) << endl;
	// cout << std::setprecision(20) << InvSqrt(6553500000) << endl;
	// cout << std::setprecision(20) << sqrt3Newton(216, error) << endl;
	// cout << std::setprecision(20) << sqrt4Newton(81, error) << endl;

	cout << std::setprecision(20) << sqrtNewton_n(81,10) << endl;

	return 0;
}