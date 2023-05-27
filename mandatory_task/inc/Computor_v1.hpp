#pragma once

# define DEBUG 0

# include "Token.hpp"
# include "Lexer.hpp"
# include "Parse.hpp"

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
	std::map<int, double>		_pol;
	std::string					_x;

	int							_degree;
	double						_a;
	double						_b;
	double						_c;
	double						_D;

	std::string					_msg_decision;

private:
	Computor_v1();
	Computor_v1(Computor_v1 const &copy);
	Computor_v1 &operator=(Computor_v1 const &copy);

	std::string			_check_input(int argc, char **argv);
	void				_update_polinom_argument();

	std::string 		_reduce_form_double() const;
	void				_print_input();
	void				_print_polinom_tokens();
	void				_print_polinom_map();
	void				_print_polinom_argument();

	std::string			_double_to_string(double n) const;
	double				_sqrt(double n) const;
	double				_abs(double n) const;

public:
	Computor_v1(int argc, char **argv);
	~Computor_v1();

	void			decision();
	void			polinom_print();
};
