#pragma once

# include "Token.hpp"
# include "Lexer.hpp"
# include "Parse.hpp"
# include "Fraction.hpp"

# include <string>
# include <vector>
# include <map>
# include <iostream>

class Computor_v1
{
private:
	std::string					_input;
	std::vector<Token>			_tokens;
	std::map<int, Fraction>		_pol;
	
	int							_degree;
	std::string					_msg_decision;
	std::string					_msg_decision_sqrt;
	std::string					_msg_decision_double;

	std::string					_x;
	Fraction					_a;
	Fraction					_b;
	Fraction					_c;
	Fraction					_D;

	Fraction					_part_1;
	Fraction					_part_2_sqrt;

private:
	Computor_v1();

	std::string			_check_input(int argc, char **argv);

	void 				_update_polinom();

	std::string 		_reduce_form_fraction();
	std::string 		_reduce_form_double();	
	std::string 		_clear_arg(double arg) const;

public:
	Computor_v1(int argc, char **argv);
	~Computor_v1();

	void			decision();
	void			polinom_print();

	
};
