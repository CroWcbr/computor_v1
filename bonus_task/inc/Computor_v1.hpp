#pragma once

# define DEBUG 1

# include "Token.hpp"
# include "Lexer.hpp"
# include "Parse.hpp"
# include "Fraction.hpp"

# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <signal.h>

class Computor_v1
{
private:
	std::string					_input;
	std::vector<Token>			_tokens;
	std::map<int, Fraction>		_pol;
	std::string					_x;

	int							_degree;
	Fraction					_a;
	Fraction					_b;
	Fraction					_c;
	Fraction					_D;

	std::string					_msg_decision;
	std::string					_msg_decision_double;

	Fraction					_part_1;
	Fraction					_part_2_sqrt;

private:
	Computor_v1();
	Computor_v1(Computor_v1 const &copy);
	Computor_v1 &operator=(Computor_v1 const &copy);

	std::string			_check_input(int argc, char **argv);
	void				_update_polinom_argument();

	std::string 		_reduce_form_fraction() const;
	std::string 		_clear_arg(double arg) const;

	void				_print_input();
	void				_print_polinom_tokens();
	void				_print_polinom_map();
	void				_print_polinom_argument();

public:
	Computor_v1(int argc, char **argv);
	~Computor_v1();

	void			decision();
	void			polinom_print_bonus();
};
