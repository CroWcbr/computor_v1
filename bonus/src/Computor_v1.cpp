#include "../include/Computor_v1.hpp"

Computor_v1::Computor_v1(int argc, char **argv)
{
	// (void)argc;
	// (void)argv;
	// _input = "-3*x^2 = 8";
	_input = _check_input(argc, argv);

	Lexer lex(_input);
	_tokens = lex.getTokens();

	Parse par(_tokens);
	_pol = par.getPolinom();
	_x = par.getX();

	_update_polinom();
}

Computor_v1::~Computor_v1()
{
}

std::string Computor_v1::_check_input(int argc, char **argv)
{
	std::string	polinom;

	if (argc == 1)
	{
		std::cout << "Enter polinom : ";
		getline(std::cin, polinom);
	}
	else if (argc == 2)
		polinom = argv[1];
	else
		throw std::runtime_error("INPUT ERROR! too much arguments!");
	return polinom;
}


void Computor_v1::_update_polinom()
{
	_degree = 0;
	for (std::map<int, Fraction>::iterator it = _pol.begin(), it_end = _pol.end(); it != it_end; it++)
	{
		if (it->first == 2)
			_a = it->second;
		else if (it->first == 1)
			_b = it->second;
		else if (it->first == 0)
			_c = it->second;
		
		if (it->second.getNumerator() != 0)
			_degree = it->first;
	}
}

std::string Computor_v1::_reduce_form_fraction()
{
	std::string reduce_form;
	for (std::map<int, Fraction>::iterator it = _pol.begin(), it_end = _pol.end(); it != it_end; it++)
	{	
		if (it->second.getNumerator() != 0)
		{
			if (it->second.getNumerator() < 0)
				reduce_form += "- ";
			else if (!(reduce_form.empty()))
				reduce_form += "+ ";
			std::string tmp_abs=  it->second.toString();
			if (tmp_abs.front()== '-')
				tmp_abs = tmp_abs.substr(1);
			reduce_form += tmp_abs + " ";
			if (it->first == 1)
				reduce_form += "* X ";		
			else if (it->first > 1)
				reduce_form += "* X^" + std::to_string(it->first) + " ";
		}
	}
	reduce_form += "= 0";
	return reduce_form;
}

std::string Computor_v1::_reduce_form_double()
{
	std::string reduce_form;
	for (std::map<int, Fraction>::iterator it = _pol.begin(), it_end = _pol.end(); it != it_end; it++)
	{	
		if (it->second.getNumerator() != 0)
		{
			if (it->second.getNumerator() < 0)
				reduce_form += "- ";
			else if (!(reduce_form.empty()))
				reduce_form += "+ ";
			std::string tmp_abs=  it->second.toStringDouble();
			if (tmp_abs.front()== '-')
				tmp_abs = tmp_abs.substr(1);
			reduce_form += tmp_abs + " ";
			if (it->first == 1)
				reduce_form += "* X ";		
			else if (it->first > 1)
				reduce_form += "* X^" + std::to_string(it->first) + " ";
		}
	}
	reduce_form += "= 0";
	return reduce_form;
}

void Computor_v1::decision()
{
	std::cout << "11!!!!!" << std::endl;
	if (_degree == 0 && _c.getNumerator() == 0)
		_msg_decision = "X may be any" ;
	else if (_degree == 0)
		_msg_decision = "Inequality is wrong : " + _c.toString() + " = 0" ;
	else if (_degree == 1)
	{
		_msg_decision = "The solution is : " + Fraction(_c / _b * -1).toString();
		_msg_decision_double = "The solution is : " + Fraction(_c / _b * -1).toStringDouble();
	}
	else if (_degree == 2)
	{
		std::cout << "22!!!!!" << std::endl;
		_D = _b * _b + _a * _c * (-4);
std::cout << "33!!!!!" << std::endl;
	std::cout << "\t : " <<  (_a * 2).toString() << std::endl;
std::cout << "33!!!!!" << std::endl;
		_part_1 = (_b * -1)/(_a * 2);
		std::cout << "44!!!!!" << std::endl;
		_part_2_sqrt = _D / (_a * _a * 2 * 2);
		std::cout << "55!!!!!" << std::endl;
		if (_D.getNumerator() < 0)
		{
			_part_2_sqrt *= -1;
			Fraction desc = _part_2_sqrt._sqrtFraction();
			if (desc.getNumerator() == -1)
				_msg_decision = "First solution : " + _part_1.toString() + " + i * sqrt(" + _part_2_sqrt.toString() + ")" + \
								"\nSecond solution : " + _part_1.toString() + " - i * sqrt(" + _part_2_sqrt.toString() + ")";
			else
				_msg_decision = "First solution : " + _part_1.toString() + " + i * " + _part_2_sqrt._sqrtFraction().toString() + \
								"\nSecond solution : " + _part_1.toString() + " - i * " + _part_2_sqrt._sqrtFraction().toString();		
			_msg_decision_double = "First solution : " + _part_1.toStringDouble()  + " + i * " + _clear_arg(_part_2_sqrt._sqrtDouble()) + \
								   "\nSecond solution : " + _part_1.toStringDouble() + " - i * " + _clear_arg(_part_2_sqrt._sqrtDouble());
		}
		else if (_D.getNumerator() == 0)
		{
			_msg_decision = "The solution is : " + _part_1.toString();
			_msg_decision_double = "The solution is : " + _part_1.toStringDouble();
		}
		else
		{
			Fraction desc = _part_2_sqrt._sqrtFraction();
			if (desc.getNumerator() == -1)			
				_msg_decision = "First solution : " + _part_1.toString() + " + sqrt(" + _part_2_sqrt.toString() + ")" + \
								"\nSecond solution : " + _part_1.toString() + " - sqrt(" +  _part_2_sqrt.toString() + ")";
			else
				_msg_decision = "First solution : " + (_part_1 + _part_2_sqrt._sqrtFraction()).toString() + \
								"\nSecond solution : " + (_part_1 - _part_2_sqrt._sqrtFraction()).toString();
			_msg_decision_double = "First solution : " + _clear_arg(_part_1.toDouble() + _part_2_sqrt._sqrtDouble()) + \
								   "\nSecond solution : " + _clear_arg(_part_1.toDouble() - _part_2_sqrt._sqrtDouble());
		}
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
	std::cout << "Reduced form : " << _reduce_form_fraction() << std::endl;
	std::cout << "Reduced form : " << _reduce_form_double() << std::endl;
	std::cout << "Polynomial degree : " << _degree << std::endl;
	if (_degree == 2)
	{
		std::cout << "D : " << _D.toString() << std::endl;
		std::cout << "_part_1 : " << _part_1.toString() << " : " << _part_1.toStringDouble() << std::endl;
		std::cout << "_part_2_sqrt : " << _part_2_sqrt.toString() << " : " << _part_2_sqrt.toStringDouble() << std::endl;
	}
	std::cout << _msg_decision << std::endl;
	if (_msg_decision_double.empty())
		return ;
	std::cout << "++++++++++++" << std::endl;
	std::cout << _msg_decision_double << std::endl;
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
