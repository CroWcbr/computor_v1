#include "../inc/Computor_v1.hpp"

Computor_v1::Computor_v1(int argc, char **argv)
{
	_input = _check_input(argc, argv);

	Lexer lex(_input);
	_tokens = lex.getTokens();
	if (DEBUG)
		_print_polinom_tokens();

	Parse par(_tokens);
	_pol = par.getPolinom();
	_x = par.getX();
	if (DEBUG)
		_print_polinom_map();

	_update_polinom_argument();
	if (DEBUG)
		_print_polinom_argument();
}

Computor_v1::~Computor_v1() {}

void Computor_v1::_print_input()
{
	std::cout << _input << std::endl;
}

void Computor_v1::_print_polinom_tokens()
{
	std::cout << "_print_polinom_tokens : " << std::endl;
	std::cout << "\t";
	for (auto &t : _tokens)
		std::cout << t.getLexem() << " ";
	std::cout << std::endl;
}

void Computor_v1::_print_polinom_map()
{
	std::cout << "_print_polinom_map : " << std::endl;
	if (_pol.empty())
		std::cout << "\t0\t:\t0\tpolinom map is empty" << std::endl;
	for (auto &p : _pol)
		std::cout << "\t" << p.first << "\t:\t" << p.second << std::endl;
}

void Computor_v1::_print_polinom_argument()
{
	std::cout << "_print_polinom_argument : " << std::endl;
	std::cout << "\ta\t:\t" << _a << std::endl;
	std::cout << "\tb\t:\t" << _b << std::endl;
	std::cout << "\tc\t:\t" << _c << std::endl;
	std::cout << "\tD\t:\t" << _D << std::endl;
}

std::string Computor_v1::_check_input(int argc, char **argv)
{
	std::string	polinom;

	if (argc == 1)
	{
		while (polinom.empty())
		{
			std::cout << "Enter polinom : ";
			getline(std::cin, polinom);
		}
	}
	else if (argc == 2)
		polinom = argv[1];
	else
		throw std::runtime_error("INPUT ERROR! too much arguments!");
	return polinom;
}

void Computor_v1::_update_polinom_argument()
{
	_a = _b = _c = _D = 0;
	if (_pol.size() > 0)
		_degree = _pol.rbegin()->first;
	else
		_degree = 0;
	if (_degree > 2)
		return ;
	if (_pol.find(2) != _pol.end())
		_a = _pol.find(2)->second;
	if (_pol.find(1) != _pol.end())	
		_b = _pol.find(1)->second;
	if (_pol.find(0) != _pol.end())
		_c = _pol.find(0)->second;
	_D = _b * _b - 4 * _a * _c;
}

std::string Computor_v1::_reduce_form_double() const
{
	std::string reduce_form;
	for (auto &p : _pol)
	{	
		if (p.second < 0)
			reduce_form += "- ";
		else if (!(reduce_form.empty()))
			reduce_form += "+ ";
		reduce_form += _double_to_string(_abs(p.second)) + " ";
		reduce_form += "* " + _x + "^" + std::to_string(p.first) + " ";
	}
	if (reduce_form.empty())
		reduce_form = "0 ";
	reduce_form += "= 0";
	return reduce_form;
}

void	 Computor_v1::decision()
{
	if (_degree == 0 && _c == 0)
	{
		if (_x.empty())
			_msg_decision = "Unknown may be any";
		else
			_msg_decision = "\"" + _x + "\" may be any";
	}
	else if (_degree == 0)
	{
		_msg_decision = "Inequality is wrong : " + _double_to_string(_c) + " = 0" ;
	}
	else if (_degree == 1)
	{
		_msg_decision = "The solution is : " + _double_to_string(_c / _b * -1);
	}
	else if (_degree > 2)
	{
		_msg_decision = "The polynomial degree is strictly greater than 2, I can't solve.";
	}
	else
	{
		if (_D < 0)
		{
			_msg_decision = "No desicion. D < 0";
		}
		else if (_D == 0)
		{
			_msg_decision = "The solution is : " + _double_to_string((_b * -1) / (_a * 2));
		}
		else
		{
			_msg_decision = "First solution : " +  _double_to_string((-_b + _sqrt(_D)) / (2 * _a)) + \
							"\nSecond solution : " + _double_to_string((-_b - _sqrt(_D)) / (2 * _a));
		}
	}
}

void	Computor_v1::polinom_print()
{
	std::cout << "Input : " << _input << std::endl;
	std::cout << "Reduced form : " << _reduce_form_double() << std::endl;
	std::cout << "Polynomial degree : " << _degree << std::endl;
	std::cout << _msg_decision << std::endl;
}

std::string	Computor_v1::_double_to_string(double n) const
{
	std::string tmp =  std::to_string(n);
	while (tmp.back() == '0')
		tmp.pop_back();
	if (tmp.back() == '.')
		tmp.pop_back();
	return tmp;
}

double	Computor_v1::_sqrt(double n) const//method Newton
{
	const double EPS = 1E-15;
	double x = 1;
	while (42)
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

double	Computor_v1::_abs(double n) const
{
	if (n < 0)
		return -n;
	return n;
}