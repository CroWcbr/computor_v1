#pragma once

# include "Token.hpp"
# include "Lexer.hpp"
# include "Parse.hpp"

# include <string>
# include <vector>
# include <map>
# include <iostream>

class Computor_v1
{
private:
	std::string				_input;
	std::vector<Token>		_tokens;
	std::map<int, double>	_pol;
	
	std::string				_reduce_form;
	int						_degree;
	std::string				_msg_decision;


	double					_a;
	double					_b;
	double					_c;
	std::string				_x;
	double					_D;

private:
	Computor_v1();

	std::string			_check_input(int argc, char **argv);
	void 				_update_polinom();
	void				_update_reduce_form();

	std::string			_clear_arg(double arg) const;
	double				_sqrt(double n) const;
public:
	Computor_v1(int argc, char **argv);
	~Computor_v1();

	void			decision();
	void			polinom_print();
};
