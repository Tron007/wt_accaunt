/*
 * main.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: judge
 */

#include "wtaccounts.h"

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new WtAccounts(env);
}

int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}




