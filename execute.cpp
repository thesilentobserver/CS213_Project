#include "header.h"

void authenticate::start()	{
	
	cout<<"Welcome to Rent-a-bike!\n";
	int n;
	while(true)	{
		cout<<"\n1.Existing user? Login\n2.New user? Register\n0.Quit\n\n";	
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
				int row=2; 
				/* Assuming username are in first column and passwords in the second one.
				Also row 1 can't be used because of Libxl banner at (0,0) and row 2 has column headings.*/
				for(;row!=(*sheet).lastRow();row++)	{
					if(uname==(*sheet).readStr(row,0) && pwd==(*sheet).readStr(row,1))	{
						flag=true;
						cout<<"\nWelcome "<<(*sheet).readStr(row,2)<<"\nWhat can we do for you today?\n";
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
				user customer;
				customer.menu(uname);
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

	string uname;
	if(!system("test -e users_list.xlsx"))	{
			libxl::Book* user_db=xlCreateXMLBook();
			if((*user_db).load("users_list.xlsx"))	{
				libxl::Sheet* sheet=(*user_db).getSheet(0);
				string pwd;
				bool flag=true;
				while(flag)	{
					cout<<"Enter the desired username:";
					cin>>uname;
					if(!chk_uname(uname))	{
						int row=(*sheet).lastRow();
						(*sheet).writeStr(row,0,uname.c_str());
						cout<<"Pease enter password:";
						cin>>pwd;
						(*sheet).writeStr(row,1,pwd.c_str());
						string c_no;
						cout<<"Please enter your name:";
						cin.ignore();
						char n[100];
						cin.getline(n,100);
						string name(n);
						// getline(cin,name);
						(*sheet).writeStr(row,2,name.c_str());
						cout<<"Please enter your contact number:";
						cin>>c_no;
						(*sheet).writeStr(row,3,c_no.c_str());
						for(int i=4;i<=9;i++)	
							(*sheet).writeStr(row,i,"-");
						(*sheet).writeNum(row,10,0);

						cout<<"\nWelcome "<<(*sheet).readStr(row,2)<<"\nWhat can we do for you today?\n";

						(*user_db).save("users_list.xlsx");

						// user customer;
						// customer.menu(uname);
						flag=false;
					}
					else
						cout<<"Sorry,the username already exists.Please choose another\n";
				
				}
			}
			else
				cout<<"Failed to load users' database\n";
		(*user_db).release();
		}
	else	{
		//Create new database
		libxl::Book* user_db=xlCreateXMLBook();
		libxl::Sheet* sheet=(*user_db).addSheet("Sheet1");
		(*sheet).writeStr(1,0,"Username");
		(*sheet).writeStr(1,1,"Password");
		(*sheet).writeStr(1,2,"Name");
		(*sheet).writeStr(1,3,"Contact Number");
		(*sheet).writeStr(1,4,"Rented from");
		(*sheet).writeStr(1,5,"Contact Number ");
		(*sheet).writeStr(1,6,"Due Date");
		(*sheet).writeStr(1,7,"Rented to");
		(*sheet).writeStr(1,8,"Contact Number");
		(*sheet).writeStr(1,9,"Due Date");
		(*sheet).writeStr(1,10,"Money Due");

		string uname,pwd;
		cout<<"Enter the desired username:";
		cin>>uname;
		int row=(*sheet).lastRow();
		(*sheet).writeStr(row,0,uname.c_str());
		cout<<"Pease enter password:";
		cin>>pwd;
		(*sheet).writeStr(row,1,pwd.c_str());
		string c_no;
		cout<<"Please enter your name:";
		cin.ignore();
		char n[100];
		cin.getline(n,100);
		string name(n);
		// getline(cin,name);
		(*sheet).writeStr(row,2,name.c_str());
		cout<<"Please enter your contact number:";
		cin>>c_no;
		(*sheet).writeStr(row,3,c_no.c_str());
		for(int i=4;i<=9;i++)	
			(*sheet).writeStr(row,i,"-");
		(*sheet).writeNum(row,10,0);
		(*user_db).save("users_list.xlsx");
	
		cout<<"\nWelcome "<<(*sheet).readStr(row,2)<<"\nWhat can we do for you today?\n";

		(*user_db).release();
	}

	user customer;
	customer.menu(uname);
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
		std::cout<<"Failed to load users' database\n";

	(*user_db).release();

	return status;
}

void user::menu(string uname)	{

	int choice;
	bool flag=true;
	while(flag)	{
		cout<<"\n1.Rent a bike\n2.Put up a bike for renting\n3.View profile\n4.Edit Profile\n0.Logout\n\n";
		cin>>choice;
		switch(choice)	{
			case 1:	rent(uname);
					break;
			case 2:	lend(uname);
					break;
			case 3:	status(uname);
					break;
			case 4:	edit(uname);
					break;
			case 0:	flag=false;
					break;
			default:cout<<"Incorrect input.Please enter a valid choice.\n";
					break; 

		}

	}
}

void user::status(string uname)	{

	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==uname)
			break;
	cout<<"\n---- Customer Profile ----\n\n";
	cout<<"\nName-"<<(*sheet).readStr(row,2);
	cout<<"\nUsername-"<<(*sheet).readStr(row,0);
	cout<<"\nContact Number-"<<(*sheet).readStr(row,3);
	cout<<"\n\n**CYCLES RENTED**\n";
	cout<<"\nRented from-"<<(*sheet).readStr(row,4);
	cout<<"\nContact Number-"<<(*sheet).readStr(row,5);
	cout<<"\nReturn on-"<<(*sheet).readStr(row,6);
	cout<<"\n\n**CYCLES PUT OUT ON RENT**\n";
	cout<<"\nRented to-"<<(*sheet).readStr(row,7);
	cout<<"\nContact Number-"<<(*sheet).readStr(row,8);
	cout<<"\nTo be returned on-"<<(*sheet).readStr(row,9);
	cout<<"\n\nMoney due-"<<(*sheet).readNum(row,10)<<"\n\n";
	cout<<"--------------------\n\n";

	(*user_db).release();
	return;
}

void user::edit(string uname)	{

	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==uname)
			break;

	int n;
	bool flag=true;
	while(flag)	{
		cout<<"\n1.Edit Contact Number\n2.Change Password\n3.Return to previous menu\n\n";
		cin>>n;

		switch(n)	{

			case 1:	{
						string no;
						cout<<"\nPlease enter your new contact number-";
						cin>>no;
						(*sheet).writeStr(row,3,no.c_str());
						cout<<"\nContact number updated successfully!\n";
						(*user_db).save("users_list.xlsx");
						break;
					}

			case 2:	{
						string pwd;
						cout<<"Please enter your new password:";
						cin>>pwd;
						(*sheet).writeStr(row,1,pwd.c_str());
						cout<<"Password changed successfully!\n";
						(*user_db).save("users_list.xlsx");
						break;
					}
						
			case 3: flag=false;
					break;
			default:cout<<"Invalid choice.Please enter a valid choice\n";
					break;
		}

	}

	(*user_db).release();
	return;
}

int* user::getDate(int i)	{

	time_t t_now=time(0);	
	struct tm* date=localtime(&t_now);
	time_t t_sec=mktime(date)+i*24*60*60;
	struct tm* n_date=localtime(&t_sec);
	
	int* arr=new int[3];
	arr[0]=(*n_date).tm_year+1900;
	arr[1]=(*n_date).tm_mon+1;
	arr[2]=(*n_date).tm_mday;

	return arr;
}

void user::lend(string uname)	{

	libxl::Book* rental_db=xlCreateXMLBook();
	
	if(!system("test -e rental_list.xlsx"))	{

		(*rental_db).load("rental_list.xlsx");
		libxl::Sheet* sheet=(*rental_db).getSheet(0); //Sheet 1 contains the bikes available for renting
		libxl::Format* dformat=(*rental_db).addFormat();
		(*dformat).setNumFormat(libxl::NUMFORMAT_DATE);

		int row=(*sheet).lastRow();
		user u;
		(*sheet).writeStr(row,6,(u.getName(uname)).c_str());
		(*sheet).writeStr(row,7,(u.getContact(uname)).c_str());

		string  inp;
		cout<<"Please fill out the necessary details-\n";
		
		cout<<"\nBrand-";
		cin>>inp;
		for(int i=0;i<inp.length();i++)
			inp[i]=tolower(inp[i]);

		(*sheet).writeStr(row,0,inp.c_str());
		
		cout<<"\nGeared [y/n]-";
		cin>>inp;
		if(inp=="y")
			inp="Yes";
		else
			inp="No";
		(*sheet).writeStr(row,1,inp.c_str());
		
		cout<<"\nWhich rent bracket do you wish to place your bike in-\n1)Rs.500/day  2)Rs 800/day  3)Rs.1000/day\n";
		int s;
		cin>>s;
		switch(s)	{
			case 1:	inp="Rs.500/day";
						break;
			case 2:	inp="Rs.800/day";
						break;
			case 3:	inp="Rs.1000/day";
						break;
		}

		(*sheet).writeStr(row,2,inp.c_str());
		
		cout<<"\nPlease enter your location-";
		cin.ignore();
		char str[100];
		cin.getline(str,100); 	
		(*sheet).writeStr(row,3,str);
		
		cout<<"\nPlease enter the date till you want to list the bike for rent in DD MM YYYY format\n[If you want to list it indefinitely/till deletion, please enter '00 00 0000']\n";
		int d,m,y;
		cin>>d>>m>>y;
		(*sheet).writeNum(row,5,(*rental_db).datePack(y,m,d),dformat);
		
		cout<<"\nPlease enter additional details (if any)\n[Press 'Enter' to skip/end comment]\n";
		char com[1000];
		cin.ignore();
		cin.getline(com,1000);	
		(*sheet).writeStr(row,4,com);

		cout<<"\nList the bike for rental? [y/n]-";
		cin>>inp;
		if(inp=="y")	{
			(*rental_db).save("rental_list.xlsx");
			cout<<"\nBike listed up for rent successfully!\n\n";
		}
		else
			cout<<"\nBike listing request discarded!\n\n";

		(*rental_db).release();
	}
	else	{
		//Create new
		libxl::Sheet* sheet=(*rental_db).addSheet("Sheet1"); //Sheet 1 contains the bikes available for renting
		libxl::Format* dformat=(*rental_db).addFormat();
		(*dformat).setNumFormat(libxl::NUMFORMAT_DATE);
		string arr[]={"Brand","Geared","Price","Location","Details","Available till","Owner","Contact Number"};

		for(int i=0;i<=7;i++)
			(*sheet).writeStr(1,i,arr[i].c_str());

		int row=(*sheet).lastRow();

		user u;
		(*sheet).writeStr(row,6,(u.getName(uname)).c_str());
		(*sheet).writeStr(row,7,(u.getContact(uname)).c_str());

		string  inp;
		cout<<"Please fill out the necessary details-\n";
		
		cout<<"\nBrand-";
		cin>>inp;
		(*sheet).writeStr(row,0,inp.c_str());
		
		cout<<"\nGeared [y/n]-";
		cin>>inp;
		if(inp=="y")
			inp="Yes";
		else
			inp="No";
		(*sheet).writeStr(row,1,inp.c_str());
		
		cout<<"\nWhich rent bracket do you wish to place your bike in-\n1)Rs.500/day  2)Rs 800/day  3)Rs.1000/day\n";
		int s;
		cin>>s;
		switch(s)	{
			case 1:	inp="Rs.500/day";
						break;
			case 2:	inp="Rs.800/day";
						break;
			case 3:	inp="Rs.1000/day";
						break;
		}

		(*sheet).writeStr(row,2,inp.c_str());
		
		cout<<"\nPlease enter your location-";
		cin.ignore();
		char str[100];
		cin.getline(str,100);		
		(*sheet).writeStr(row,3,str);
		
		cout<<"\nPlease enter the date till you want to list the bike for rent in DD MM YYYY format\n[If you want to list it indefinitely/till deletion, please enter '00 00 0000']\n";
		int d,m,y;
		cin>>d>>m>>y;
		(*sheet).writeNum(row,5,(*rental_db).datePack(y,m,d),dformat);
		
		cout<<"\nPlease enter additional details (if any)\n[Press 'Enter' to skip/end comment]\n";
		char com[1000];
		cin.ignore();
		cin.getline(com,1000);
		(*sheet).writeStr(row,4,com);

		cout<<"\nList the bike for rental? [y/n]-";
		cin>>inp;
		if(inp=="y")	{
			(*rental_db).save("rental_list.xlsx");
			cout<<"\nBike put up for rent successfully!\n\n";
		}
		else
			cout<<"\nBike listing request discarded!\n\n";

		(*rental_db).release();
	}

return;
}

void user::rent(string uname)	{
	
	user u;
	// string filters[4];
	cout<<"Please choose the price filter [press '0' to skip]\n1.Rs.500/day only\n2.Rs.800/day only\n3.Rs.1000/day only\n4.<=Rs.800/day\n";
	// cin.clear();
	// cin.ignore();
	// char c[100];
	// cin.getline(c,100,'\n');
	int pr;
	cin>>pr;


	cout<<"Please choose the brand filter [press '0' to skip]\n";
	
	string* brands=new string[25];
	int l=u.getBrands(brands);
	for(int i=0;i<l;i++)
		cout<<i+1<<"."<<brands[i]<<endl;
	// cout<<"Here";
	// cin.clear();
	// cin.ignore();
	// char c1[100];
	// cin.getline(c1,100,'\n');
	int br;
	cin>>br;
	// filter[1]=brands[ch-1];

	cout<<"Please choose the geared bikes filter [press '0' to skip]\n1.Geared only\n2.Non-geared only\n"; 
	// char c2[100];
	// cin.clear();
	// cin.ignore();
	// cin.getline(c2,100,'\n');
	int gr;
	cin>>gr;

	cout<<"Please enter the number of days you want to rent-";
	// cin.clear();
	// cin.ignore();
	// char c3[100];
	// cin.getline(c3,100,'\n');
	int d;
	cin>>d;

	// cout<<pr<<" "<<br<<" "<<gr<<" "<<d<<endl;
	int filters[4]={pr,br,gr,d};
	// if(br!=0)	{
	// 	// int i=stoi(br);
	// 	// cout<<"i="<<i<<endl;
	// 	filters[1]=brands[br-1];
		// cout<<"filter="<<filters[3]<<endl;
	

	// cout<<"Before\n"<<filters[0]<<" "<<filters[1]<<" "<<filters[2]<<" "<<filters[3];

	u.runQuery(filters);
	
	return;

}

int user::getBrands(string* brands)	{

	libxl::Book* rental_db=xlCreateXMLBook();
	(*rental_db).load("rental_list.xlsx");
	libxl::Sheet* sheet=(*rental_db).getSheet(0);

	int row=(*sheet).lastRow();
	string brand_data[row-2];//Since row data starts from row index 2 and continues upto 'row'-1
	for(int i=2;i<row;i++)
		brand_data[i-2]=(*sheet).readStr(i,0);

	int length=0;

	for(int i=0;i<row-2;i++)	{

		if(brand_data[i]=="*")
			continue;

		string s=brand_data[i];
		brands[length]=s;
		length++;
		for(int j=i+1;j<row-2;j++)	{
			
			if(brand_data[j]==s)
				brand_data[j]="*";
		}
	}

	return length;
}

void user::runQuery(int* filters)	{

	// cout<<"Here"<<filters[0]<<" "<<filters[1]<<" "<<filters[2]<<" "<<filters[3];
	
	user u;
	libxl::Book* rental_db=xlCreateXMLBook();
	(*rental_db).load("rental_list.xlsx");
	libxl::Sheet* r_sheet=(*rental_db).getSheet(0);

	// libxl::Book* query=xlCreateXMLBook();
	// libxl::Sheet* q_sheet=(*query).addSheet("Sheet1");
	string brand_f="";

	//Brand filter
	if(filters[1]!=0)	{
		string* br_list=new string[25];
		getBrands(br_list);
		brand_f=br_list[(filters[1]-1)];
	}
	else
		brand_f="";
	// cout<<filters[0]<<" "<<filters[1]<<" "<<filters[2]<<" "<<filters[3];

	//Geared filter
	string geared_f="";
	// int i=stoi(filters[2]);
	switch(filters[2])	{
		case 1:	geared_f="Yes";
				break;
		case 2:	geared_f="No";
				break;
		case 0:	geared_f="";
				break;
		default:geared_f="";
				break;
	}

	// int days=stoi(filters[3]);

	int* till_date=u.getDate(filters[3]);
	
	//Price filter
	string price_f[2]={"",""};
	// int k=stoi(filters[0]);
	switch(filters[0])	{
		case 1:	price_f[0]="Rs.500/day";
				price_f[1]="";
				break;
		case 2:	price_f[0]="Rs.800/day";
				price_f[1]="";
				break;
		case 3:	price_f[0]="Rs.1000/day";
				price_f[1]="";
				break;
		case 4:	price_f[0]="Rs.500/day";
				price_f[1]="Rs.800/day";
				break;
		case 0:	price_f[0]="";
				price_f[1]="";
				break;
		default:price_f[0]="";
				price_f[1]="";
				break;
	}


	for(int i=2;i!=(*r_sheet).lastRow();i++)	{

		if(brand_f!="")	{
			
			if((*r_sheet).readStr(i,0)!=brand_f)	{
				(*r_sheet).removeRow(i,i);
				i--;
				continue;
			}
		}

		if(price_f[0]!="")	{

			if((*r_sheet).readStr(i,2)!=price_f[0] || (*r_sheet).readStr(i,2)!=price_f[1])	{
				(*r_sheet).removeRow(i,i);
				i--;
				continue;
			}
		}

		if(geared_f!="")	{

			if((*r_sheet).readStr(i,1)!=geared_f)	{
				(*r_sheet).removeRow(i,i);
				i--;
				continue;				
			}
		}

		int yr,mon,day;
		double read_date=(*r_sheet).readNum(i,5);
		(*rental_db).dateUnpack(read_date,&yr,&mon,&day);
		// cout<<yr<<" ";  //<<mon<<" "<<day<<endl;
		if(yr!=0)	{
			if(yr>=till_date[0]){
				if(mon>till_date[1]){
					continue;
				}
				else
					if(mon==till_date[1])	{
						if(day>=till_date[2])
							continue;
						else	{
							(*r_sheet).removeRow(i,i);
							i--;
							continue;				
						}
					}
				else	{
					(*r_sheet).removeRow(i,i);
					i--;
					continue;				
				}
			}
			else	{
				(*r_sheet).removeRow(i,i);
				i--;
				continue;				
			}
		}

	}
	(*r_sheet).insertCol((*r_sheet).lastCol(),(*r_sheet).lastCol());
	int col=(*r_sheet).lastCol();
	(*r_sheet).writeStr(1,(*r_sheet).lastCol(),"uniqueID");
	for(int i=2;i!=(*r_sheet).lastRow();i++)
		(*r_sheet).writeNum(i,col,i-1);

	(*rental_db).save("queryResults.xlsx");
	cout<<"\n\nA 'queryResults.xlsx' has been created.Please enter the 'uniqueID' of the bike you want to rent or enter '0' to discard query\n";
	int ch;
	cin>>ch;

	libxl::Sheet* l_sheet=(*rental_db).getSheet(1);
	if(l_sheet!=NULL)	{
		int row=(*l_sheet).lastRow();
		for(int i=0;i<=9;i++)	{
			if(i!=5)
				(*l_sheet).writeStr(row,i,(*r_sheet).readStr(ch-1,i));
			else
				(*l_sheet).writeNum(row,i,(*r_sheet).readNum(ch-1,i));
		}
		
	}	
	else	{
		//Create new sheet
	}


	(*rental_db).release();
	return;
}



string user::getName(string uname)	{
	
	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==uname)
			break;

	string name=(*sheet).readStr(row,2);
	
	(*user_db).release();

	return name;	
}

string user::getContact(string uname)	{
	
	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==uname)
			break;

	string contact=(*sheet).readStr(row,3);
	
	(*user_db).release();
	
	return contact;	
}


int main()	{
	authenticate auth;
	auth.start();
}
