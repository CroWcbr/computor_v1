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
		std::cout << "\t" << p.first << "\t:\t" << p.second.toDouble() << std::endl;
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
	_D = _b * _b + _a * _c * (-4);
}

std::string Computor_v1::_reduce_form_fraction() const
{
	std::string	reduce_form;
	for (auto &p : _pol)
	{
		if (p.second.getNumerator() < 0)
			reduce_form += "- ";
		else if (!(reduce_form.empty()))
			reduce_form += "+ ";
		std::string tmp_abs=  p.second.toString();
		if (tmp_abs.front()== '-')
			tmp_abs = tmp_abs.substr(1);
		reduce_form += tmp_abs + " ";
		reduce_form += "* " + _x + "^" + std::to_string(p.first) + " ";
	}
	if (reduce_form.empty())
		reduce_form = "0 ";
	reduce_form += "= 0";
	return reduce_form;
}

void Computor_v1::decision()
{
	if (_degree == 0 && _c.getNumerator() == 0)
	{
		_msg_decision = _msg_decision_double = "\t\"" + _x + "\" may be any";
	}
	else if (_degree == 0)
	{
		_msg_decision = "\tInequality is wrong : " + _c.toString() + " = 0" ;
		_msg_decision_double = "\tInequality is wrong : " + _c.toStringDouble() + " = 0" ;
	}
	else if (_degree == 1)
	{
		_msg_decision = "\tThe solution is : " + Fraction(_c / _b * -1).toString();
		_msg_decision_double = "\tThe solution is : " + Fraction(_c / _b * -1).toStringDouble();
	}
	else if (_degree > 2)
		_msg_decision = _msg_decision_double = "\tThe polynomial degree is strictly greater than 2, I can't solve.";
	else
	{
		_part_1 = (_b * -1)/(_a * 2);
		_part_2_sqrt = _D / (_a * _a * 2 * 2);
		if (_D.getNumerator() < 0)
		{
			_part_2_sqrt *= -1;
			Fraction desc = _part_2_sqrt._sqrtFraction();
			if (desc.getNumerator() == -1)
				_msg_decision = "\tFirst solution : " + _part_1.toString() + " + i * sqrt(" + _part_2_sqrt.toString() + ")" + \
								"\n\tSecond solution : " + _part_1.toString() + " - i * sqrt(" + _part_2_sqrt.toString() + ")";
			else
				_msg_decision = "\tFirst solution : " + _part_1.toString() + " + i * " + _part_2_sqrt._sqrtFraction().toString() + \
								"\n\tSecond solution : " + _part_1.toString() + " - i * " + _part_2_sqrt._sqrtFraction().toString();
			_msg_decision_double = "\tFirst solution : " + _part_1.toStringDouble()  + " + i * " + _clear_arg(_part_2_sqrt._sqrtDouble()) + \
								   "\n\tSecond solution : " + _part_1.toStringDouble() + " - i * " + _clear_arg(_part_2_sqrt._sqrtDouble());
		}
		else if (_D.getNumerator() == 0)
		{
			_msg_decision = "\tThe solution is : " + _part_1.toString();
			_msg_decision_double = "\tThe solution is : " + _part_1.toStringDouble();
		}
		else
		{
			Fraction desc = _part_2_sqrt._sqrtFraction();
			if (desc.getNumerator() == -1)			
				_msg_decision = "\tFirst solution : " + _part_1.toString() + " + sqrt(" + _part_2_sqrt.toString() + ")" + \
								"\n\tSecond solution : " + _part_1.toString() + " - sqrt(" +  _part_2_sqrt.toString() + ")";
			else
				_msg_decision = "\tFirst solution : " + (_part_1 + _part_2_sqrt._sqrtFraction()).toString() + \
								"\n\tSecond solution : " + (_part_1 - _part_2_sqrt._sqrtFraction()).toString();
			_msg_decision_double = "\tFirst solution : " + _clear_arg(_part_1.toDouble() + _part_2_sqrt._sqrtDouble()) + \
								   "\n\tSecond solution : " + _clear_arg(_part_1.toDouble() - _part_2_sqrt._sqrtDouble());
		}
	}
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

void Computor_v1::polinom_print_bonus()
{
	std::cout << "Clear input : " << _input << std::endl;
	std::cout << "Reduced form : " << _reduce_form_fraction() << std::endl;
	std::cout << "Polynomial degree : " << _degree << std::endl;
	if (_degree == 2)
	{
		std::cout << "Intermediate steps : " << std::endl;
		std::cout << "\ta = " << _a.toString() << std::endl;
		std::cout << "\tb = " << _b.toString() << std::endl;
		std::cout << "\tc = " << _c.toString() << std::endl;
		std::cout << "\tD( b*b-4*a*c ) = " << _D.toString() << std::endl;
		std::cout << "\t-b/(2*a) = " << _part_1.toString() << std::endl;
		std::cout << "\tD/(4*a*a) = " << _part_2_sqrt.toString() << std::endl;
	}
	std::cout << "Solution : " << std::endl;
	if (_msg_decision == _msg_decision_double)
		std::cout << _msg_decision << std::endl;	
	else
	{
		std::cout << "Solution in Fraction : " << _degree << std::endl;
		std::cout << _msg_decision << std::endl;
		std::cout << "Solution in Double : " << _degree << std::endl;
		std::cout << _msg_decision_double << std::endl;
	}
}
