#include <iostream>
#include <stdlib.h>
#include "libxl.h"
#include <string>

using namespace std;

class excelParser	{

public:

private:

};

class authenticate {

public: 
	void start();

private:
	void login();
	void new_user();
	bool chk_uname(string uname);	

};


class user {

public:
	void menu(string uname);

private:
	void lend(string uname) {};
	void rent(string uname) {};
	void status(string uname);
	void edit(string uname);


};


class lender {

public:

private:

};

