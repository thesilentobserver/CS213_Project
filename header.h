#include <iostream>
#include <stdlib.h>
#include "libxl.h"
#include <string>
#include <ctime>

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
	int* getDate(int i);
	string getName(string uname);
	string getContact(string uname);
	int getBrands(string* brands);



private:
	void lend(string uname);
	void rent(string uname);
	void status(string uname);
	void edit(string uname);
	void runQuery(int* filters);
	void confirmRent(int choice);


};


class lender {

public:

private:

};

