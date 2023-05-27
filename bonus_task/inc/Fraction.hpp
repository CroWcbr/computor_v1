#pragma once

#include <string>
#include <iostream>

class Fraction
{
private:
	int		_numerator;
	int		_denominator;
 
private:
	int	_abs(int a)
	{
		if (a < 0)
			return -a;
		return a;
	}
	int _gcd(int a, int b) // greatest common divisor
	{
		while (b > 0) 
		{
			int c = a % b;
			a = b;
			b = c;
		}
		return a;
	}

	int _lcm(int a, int b) //least common multiple
	{
		return _gcd(a, b) * a * b;
	}

	void _reduce()
	{
		if (_numerator == 0)
			return ;
		int gcd = _gcd(_abs(_numerator), _abs(_denominator));
		if (gcd != 1) 
		{
			_numerator = _numerator / gcd;
			_denominator = _denominator / gcd;
		}
		if (_denominator < 0)
		{
			_denominator *= -1;
			_numerator *= -1;
		}
	}

	double _sqrt(double n) const //method Newton
	{
		const double EPS = 1E-15;
		double x = 1;
		for (;;) 
		{
			double nx = (x + n / x) / 2;
			double delta = x - nx;
			if (delta < 0)
				delta *= -1;
			if (delta < EPS)
				break;
			x = nx;
		}
		return x;
	}

public:
	Fraction()
	{
		_numerator = 0;
		_denominator = 1;
	} ;

	Fraction(int n, int d)
	{
		_numerator = n;
		_denominator = d;
		_reduce();
	}

	Fraction(std::string num_str)
	{
		_denominator = 1;

	 	if (num_str.find('.') != std::string::npos)
		{
			while (num_str.back() == '0')
				num_str.pop_back();
			if (num_str.back() == '.')
				num_str.pop_back();
			int pos = num_str.find('.');
			std::string num_str_before_point = num_str.substr(0, pos);
			std::string num_str_after_point = num_str.substr(pos + 1);
			int len = num_str_after_point.size();
			int denom = 1;
			for (int ii = 0; ii < len; ii++)
				denom *= 10;
			_numerator = std::atoi(num_str_before_point.c_str()) * denom + std::atoi(num_str_after_point.c_str());
			_denominator = denom;
		}
		else
			_numerator = std::atoi(num_str.c_str());
		_reduce();
	}

	Fraction(const Fraction& other)
	{
		*this = other;
	}

	Fraction& operator=(const Fraction& other)
	{
		if(this == &other)
			return *this;
		_numerator = other._numerator; 
		_denominator = other._denominator;
		return *this;
	}

	~Fraction() {};

	int getNumerator() const { return _numerator; }

	void setArg(int n, int d)
	{
		_numerator = n;
		_denominator = d;
		_reduce();
	}

	std::string toString() const
	{
		if (_numerator == 0)
			return "0";
		if (_denominator == 1)
			return std::to_string(_numerator);
		return std::to_string(_numerator) + "/" + std::to_string(_denominator);
	}

	double toDouble() const
	{
		return 1.0*_numerator/_denominator;
	}

	std::string toStringDouble() const
	{
		std::string arg_str = std::to_string(toDouble());
		while (arg_str.back() == '0')
			arg_str.pop_back();
		if (arg_str.back() == '.')
			arg_str.pop_back();
		return arg_str;
	}

	Fraction operator*(const Fraction &fraction)
	{
		return Fraction(_numerator * fraction._numerator, _denominator * fraction._denominator);
	}

	Fraction operator*(int znak)
	{
		return Fraction(_numerator * znak, _denominator);
	}

	Fraction operator/(const Fraction &fraction)
	{
		return Fraction( _numerator * fraction._denominator, _denominator * fraction._numerator);
	}

	Fraction operator+(const Fraction &fraction)
	{
		return Fraction(_numerator * fraction._denominator + _denominator * fraction._numerator,_denominator * fraction._denominator);
	}

	Fraction operator-(const Fraction &fraction)
	{
		return Fraction(_numerator * fraction._denominator - _denominator * fraction._numerator,_denominator * fraction._denominator);
	}

	Fraction& operator*=(const Fraction &fraction)
	{
		_numerator = _numerator * fraction._numerator;
		_denominator = _denominator * fraction._denominator;
		_reduce();
		return *this;
	}

	Fraction& operator*=(int znak)
	{
		_numerator = _numerator * znak;
		_reduce();
		return *this;
	}

	Fraction& operator/=(const Fraction &fraction)
	{
		_numerator = _numerator * fraction._denominator;
		_denominator = _denominator * fraction._numerator;
		_reduce();
		return *this;
	}

	Fraction& operator+=(const Fraction &fraction)
	{
		_numerator = _numerator * fraction._denominator + _denominator * fraction._numerator;
		_denominator = _denominator * fraction._denominator;
		_reduce();
		return *this;
	}

	Fraction& operator-=(const Fraction &fraction)
	{
		_numerator = _numerator * fraction._denominator - _denominator * fraction._numerator;
		_denominator = _denominator * fraction._denominator;
		_reduce();
		return *this;
	}

	double _sqrtDouble() const //method Newton
	{
		return _sqrt(1.0 * _numerator/_denominator);
	}

	Fraction _sqrtFraction() const //method Newton
	{
		double num = _sqrt(_numerator);
		double den = _sqrt(_denominator);

		int num_int = num;
		int den_int = den;

		if (num_int * num_int == _numerator && den_int * den_int == _denominator)
			return Fraction(num_int, den_int);

		return Fraction(-1, 1);
	}

	friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction)
	{
		os << fraction._numerator << '/' << fraction._denominator;
		return os;
	}
};

