#include <iostream>
#include <stdlib.h>
#include "libxl.h"
#include <string>
#include <ctime>

using namespace std;

// struct db_access	{

// libxl::Book* book;
// libxl::Sheet* sheet1;
// libxl::Sheet* sheet2;
// libxl::Format* format;


// public:
// 	void createDB(string sheet);						//Create book and add new sheet
// 	void loadDB(string db_name,int sheet);				//Load book and load Sheet1
// 	void loadDB(string db_name,int sheet1,sheet2);		//Create/load book and load Sheet1 and Sheet2
// 	void saveDB(string db_name);

// private:

// };



struct authenticate {

public: 
	void start();

private:
	void login();
	void new_user();
	bool chk_uname(string uname);	

};


struct user {


public:
	void menu();
	int* getDate(int i);
	string getName(string uname);
	string getContact(string uname);
	int getBrands(string* brands);

	string uname,name,contact;
	bool rented,lended;
	int money_pay,money_rec;



private:
	void lend();
	void rent();
	void status();
	void edit();
	void runQuery(int* filters);
	void confirmRent(int choice,int* till_date,int days);
	


};

