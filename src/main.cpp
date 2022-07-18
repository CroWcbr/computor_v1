#include "../include/Computor_v1.hpp"

void _signal_handler(int signal)
{
	{
		std::string msg = "\rSTOP SIGNAL";
		if (signal == 2)
			msg.append(" Ctrl + C ");
		else if (signal == 3)
			msg.append(" Ctrl + / ");
		else if (signal == 20)
			msg.append(" Ctrl + Z ");

		msg.append("from user");

		std::cout << "\033[93m";
		std::cout << msg << std::endl;
		std::cout << "\033[0m";
	}
	exit(signal);
}

int main(int argc, char **argv)
{
	signal(SIGINT, _signal_handler);	//Ctrl + 'C'
	signal(SIGQUIT, _signal_handler);	//Ctrl + '\'
	signal(SIGTSTP, _signal_handler);	//Ctrl + 'Z'

	try
	{
		Computor_v1 polinom(argc, argv);
		polinom.decision();

		// if (BONUS == 1)
			polinom.polinom_print_bonus();
		// else
		std::cout << std::endl;
			polinom.polinom_print();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
