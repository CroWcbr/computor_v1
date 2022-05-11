#include "../include/Computor_v1.hpp"

Computor_v1::Computor_v1(int argc, char **argv)
{
	_input = _check_input(argc, argv);

	Lexer lex(_input);
	_tokens = lex.getTokens();

	Parse par(_tokens);
	_pol = par.getPolinom();
	_x = par.getX();

	_update_polinom();
	_update_reduce_form();
}

Computor_v1::~Computor_v1()
{
}

std::string Computor_v1::_check_input(int argc, char **argv)
{
	std::string	polinom;

	if (argc == 1)
	{
		// std::cout << "Enter polinom : ";
		// getline(std::cin, polinom);
		//polinom = "+-+-+5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
		//polinom = "91*x^0 - x^1 + 42 * x^2 = 1 - 20 * x + 36*x^2 + 10 * x ^5";
		polinom = "x^2-4*x+0=0";
	}
	else if (argc == 2)
		polinom = argv[1];
	else
		throw std::runtime_error("INPUT ERROR! too much arguments!");
	return polinom;
}

void Computor_v1::_update_polinom()
{
	_a = _b = _c = _degree = 0;
	for (std::map<int, double>::iterator it = _pol.begin(), it_end = _pol.end(); it != it_end; it++)
	{
		if (it->first == 2)
			_a = it->second;
		else if (it->first == 1)
			_b = it->second;
		else if (it->first == 0)
			_c = it->second;
		
		if (it->second != 0)
			_degree = it->first;
	}
}

void Computor_v1::_update_reduce_form()
{
	std::string arg;
	std::string znak;
	for (int i = 0; i <= _degree; i++)
	{
		znak = "+";
		std::map<int, double>::iterator it = _pol.find(i);
		if (it != _pol.end())
		{
			arg = _clear_arg(it->second);
			if (arg.front() == '-')
			{
				znak = "-";
				arg = arg.substr(1, arg.size());
			}
		}
		else
			arg = "0";
		
		if (_reduce_form.empty() && znak == "+")
			;
		else 
			_reduce_form += znak + " ";
		
		_reduce_form += arg + " * ";
		_reduce_form += _x + " ^ " + std::to_string(i) + " ";
	}
	_reduce_form += "= 0";
}

void Computor_v1::decision()
{
	if (_degree == 0 && _c == 0)
		_msg_decision = "X may be any" ;
	else if (_degree == 0)
		_msg_decision = "Inequality is wrong : " + _clear_arg(_c) + " = 0" ;
	else if (_degree == 1)
		_msg_decision = "The solution is : " + _clear_arg(-_c/_b);
	else if (_degree == 2)
	{
		_D = _b * _b - 4 * _a * _c;
		if (_D < 0)
			_msg_decision = "No desision : Discriminant < 0 : " +  _clear_arg(_D);
		else if (_D == 0)
			_msg_decision = "The solution is : " + _clear_arg(-_b/(2 * _a));
		else
			_msg_decision = "First solution : " + _clear_arg((-_b + _sqrt(_D))/(2 * _a)) + \
							"\nSecond solution : " + _clear_arg((-_b - _sqrt(_D))/(2 * _a));
	}
	else
		_msg_decision = "The polynomial degree is strictly greater than 2, I can't solve.";
}

void Computor_v1::polinom_print()
{
	std::cout << "Clear input : ";
	for (const auto &symbol : _tokens)
		std::cout << symbol.getLexem() << " ";
	std::cout << std::endl;
	std::cout << "Reduced form : " << _reduce_form << std::endl;
	std::cout << "Polynomial degree : " << _degree << std::endl;
	if (_degree == 2)
		std::cout << "D : " << _D << std::endl;
	std::cout << _msg_decision << std::endl;
}

std::string Computor_v1::_clear_arg(double arg) const
{
	std::string arg_str;
	arg_str = std::to_string(arg);
	while (arg_str.back() == '0')
		arg_str.pop_back();
	if (arg_str.back() == '.')
		arg_str.pop_back();
	return arg_str;
}

double Computor_v1::_sqrt(double n) const //method Newton
{
	const double EPS = 1E-15;
	double x = 1;
	for (;;) 
	{
		double nx = (x + n / x) / 2;
		if (abs (x - nx) < EPS)
			break;
		x = nx;
	}
	return x;
}