#include "header.h"

void authenticate::start()	{
	
	int n;
	while(true)	{
		cout<<"1.Existing user? Login\n2.New user? Register\n0.Quit\n\n";	
		cin>>n;
		if(n==1)
			login();
		else
			if(n==2)
				new_user();
		else
			if(n==0)
				return;
		else
			cout<<"Wrong Input! Please enter a valid choice\n";
	}
}

void authenticate::login()	{
	if(!system("test -e users_list.xlsx"))	{
		libxl::Book* user_db=xlCreateXMLBook();
		if((*user_db).load("users_list.xlsx"))	{
			libxl::Sheet* sheet=(*user_db).getSheet(0); //Sheet INDEX is provided as argument
			string uname,pwd;
			bool flag=false;
			int trial_count=1;
			while(!flag && trial_count<=3)	{
				cout<<"Enter username:";
				cin>>uname;
				cout<<"Enter password:";
				cin>>pwd;
				int row=2,column=0; 
				/* Assuming username are in first column and passwords in the second one.
				Also row 1 can't be used because of Libxl banner at (0,0) and row 2 has column headings.*/
				for(;row!=(*sheet).lastRow();row++)	{
					if(uname==(*sheet).readStr(row,column) && pwd==(*sheet).readStr(row,column+1))	{
						flag=true;
						cout<<"Welcome "<<(*sheet).readStr(row,column+2)<<endl;
						break;						
					}
				}

				if(flag==false)	{
					trial_count++;
					cout<<"Wrong username and/or password! Please try again.\n";
				}
			}

			if(flag==false)
				cout<<"\nLogin attempts exceeded. Please try again\n\n";
			else	{
				// user customer;
				// customer.menu();
			}
		}
		else
			cout<<"Failed to load users' database\n";
	(*user_db).release();
	}
	else
		cout<<"Users' database not found!\n";
	
	return;
}

void authenticate::new_user()	{

	if(system("test -e users_list.xlsx"))	{
			libxl::Book* user_db=xlCreateXMLBook();
			if((*user_db).load("users_list.xlsx"))	{
				libxl::Sheet* sheet=(*user_db).getSheet(0);
				string uname,pwd;
				while(true)	{
					cout<<"Enter the desired username:";
					cin>>uname;
					if(!chk_uname(uname))	{
						// int row=(*sheet).lastRow();
						(*sheet).writeStr((*sheet).lastRow(),0,uname);
						cout<<"Pease enter password:";
						cin>>pwd;
						(*sheet).writeStr((*sheet).lastRow(),1,pwd);
						string name,c_no;
						cout<<"Please enter your name:";
						cin>>name;
						(*sheet).writeStr((*sheet).lastRow(),2,name);
						cout<<"Please enter your contact number:";
						cin>>c_no;
						(*sheet).writeStr((*sheet).lastRow(),3,c_no);
						for(int i=4;i<=9;i++)	
							(*sheet).writeStr((*sheet).lastRow(),i,"-");
						(*sheet).writeNum((*sheet).lastRow(),10,0);
						(*user_db).save("users_list.xlsx");
					}
					else
						cout<<"Sorry,the username already exists.Please choose another";
				}
			}
			else
				cout<<"Failed to load users' database\n";
		(*user_db).release();
		}
	else	{
		//Create new database
		Book* user_db=xlCreateXMLBook();
		Sheet* sheet=(*user_db).addSheet("Sheet1");
		(*sheet).writeStr(1,0,"Username");
		(*sheet).writeStr(1,1,"Password");
		(*sheet).writeStr(1,2,"Name");
		(*sheet).writeStr(1,3,"Contact Number");
		(*sheet).writeStr(1,4,"Rented from");
		(*sheet).writeStr(1,5,"Contact Number ");
		(*sheet).writeStr(1,6,"Due Date");
		(*sheet).writeStr(1,7,"Lended to");
		(*sheet).writeStr(1,8,"Contact Number");
		(*sheet).writeStr(1,9,"Due Date");
		(*sheet).writeStr(1,10,"Money Due");

		string uname,pwd;
		cout<<"Enter the desired username:";
		cin>>uname;
		// int row=(*sheet).libxl::lastRow();
		(*sheet).writeStr((*sheet).libxl::lastRow(),0,uname);
		cout<<"Pease enter password:";
		cin>>pwd;
		(*sheet).writeStr((*sheet).libxl::lastRow(),1,pwd);
		string name,c_no;
		cout<<"Please enter your name:";
		cin>>name;
		(*sheet).writeStr((*sheet).libxl::lastRow(),2,name);
		cout<<"Please enter your contact number:";
		cin>>c_no;
		(*sheet).writeStr((*sheet).libxl::lastRow(),3,c_no);
		for(int i=4;i<=9;i++)	
			(*sheet).writeStr((*sheet).libxl::lastRow(),i,"-");
		(*sheet).writeNum((*sheet).libxl::lastRow(),10,0);
		(*user_db).save("users_list.xlsx");

		(*user_db).release();
	}

	// user.menu();
	return;
}

bool authenticate::chk_uname(string uname)	{

	bool status=false;
	libxl::Book* user_db=xlCreateXMLBook();
	if((*user_db).load("users_list.xlsx"))	{
		libxl::Sheet* sheet=(*user_db).getSheet(0);
		int row=2,column=0;
		for(;row!=(*sheet).lastRow();row++)	{
			if((*sheet).readStr(row,0)==uname)	{
				status=true;
				break;
			}
		}
	}
	else
		std::cout<<"Failed to load users' database\n"

	(*user_db).release();
	return status;
}










int main()	{
	authenticate auth;
	cout<<"Welcome to Rent-a-bike!\n\n";
	auth.start();
}