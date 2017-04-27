#include "header.h"

// void db_access::createDB(string sheet)	{

// 	this->book=xlCreateXMLBook();
// 	this->sheet1=(*(this->book)).addSheet(sheet);
// 	this->format=(*(this->book)).addFormat();
// 	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

// 	return;
// }


// void db_access::loadDB(string db_name,int sheet)	{

// 	this->book=xlCreateXMLBook();
// 	(*(this->book)).load(db_name);
// 	this->sheet1=(*(this->book)).getSheet(sheet);
// 	this->format=(*(this->book)).addFormat();
// 	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

// 	return;
// }


// void db_access::loadDB(string db_name,int sheet1,int sheet2)	{

// 	this->book=xlCreateXMLBook();
// 	(*(this->book)).load(db_name);
// 	this->sheet1=(*(this->book)).getSheet(sheet1);
// 	this->sheet2=(*(this->book)).getSheet(sheet2);
// 	this->format=(*(this->book)).addFormat();
// 	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

// 	return;
// }

// void db_access::saveDB(string db_name)	{

// 	(*(this->book)).save(db_name);
// 	(*(this->book)).release();

// 	return;
// }

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
			string uname,pwd,name,contact;
			int money_p,money_r,row;
			bool flag=false;
			int trial_count=1;
			while(!flag && trial_count<=3)	{
				cout<<"Enter username:";
				cin>>uname;
				cout<<"Enter password:";
				cin>>pwd;
				row=2; 
				/* Assuming username are in first column and passwords in the second one.
				Also row 1 can't be used because of Libxl banner at (0,0) and row 2 has column headings.*/
				for(;row!=(*sheet).lastRow();row++)	{
					if(uname==(*sheet).readStr(row,0) && pwd==(*sheet).readStr(row,1))	{
						flag=true;
						name=(*sheet).readStr(row,2);
						contact=(*sheet).readStr(row,3);
						money_p=(*sheet).readNum(row,10);
						money_r=(*sheet).readNum(row,11);
						cout<<"\nWelcome "<<name<<"\nWhat can we do for you today?\n";
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
				//Make constructor
				user customer;
				customer.uname=uname;
				customer.name=name;
				customer.contact=contact;
				customer.money_pay=money_p;
				customer.money_rec=money_r;
				if((*sheet).readStr(row,4)!="-")
					customer.rented=true;
				else
					customer.rented=false;
				if((*sheet).readStr(row,7)!="-")
					customer.lended=true;
				else
					customer.lended=false;
				
				// customer.postedAd=false;
				customer.menu();
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

	string uname,name,contact;
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
						

						cout<<"Please enter your name:";
						cin.ignore();
						char n[100];
						cin.getline(n,100);
						string c_name(n);
						(*sheet).writeStr(row,2,c_name.c_str());
						name=c_name;

						string c_no;
						cout<<"Please enter your contact number:";
						cin>>c_no;
						(*sheet).writeStr(row,3,c_no.c_str());
						contact=c_no;
						
						for(int i=4;i<=9;i++)	
							(*sheet).writeStr(row,i,"-");
						(*sheet).writeNum(row,10,0);
						(*sheet).writeNum(row,11,0);

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
		(*sheet).writeStr(1,10,"Money Payable");
		(*sheet).writeStr(1,11,"Money Receivable");


		string pwd;
		cout<<"Enter the desired username:";
		cin>>uname;
		int row=(*sheet).lastRow();
		(*sheet).writeStr(row,0,uname.c_str());

		cout<<"Pease enter password:";
		cin>>pwd;
		(*sheet).writeStr(row,1,pwd.c_str());
		
		cout<<"Please enter your name:";
		cin.ignore();
		char n[100];
		cin.getline(n,100);
		string c_name(n);
		// getline(cin,name);
		(*sheet).writeStr(row,2,c_name.c_str());
		name=c_name;
		
		string c_no;
		cout<<"Please enter your contact number:";
		cin>>c_no;
		(*sheet).writeStr(row,3,c_no.c_str());
		contact=c_no;

		for(int i=4;i<=9;i++)	
			(*sheet).writeStr(row,i,"-");
		(*sheet).writeNum(row,10,0);
		(*sheet).writeNum(row,11,0);
		(*user_db).save("users_list.xlsx");
	
		cout<<"\nWelcome "<<(*sheet).readStr(row,2)<<"\nWhat can we do for you today?\n";

		(*user_db).release();
	}

	user customer;
	customer.uname=uname;
	customer.name=name;
	customer.contact=contact;
	customer.money_pay=0;
	customer.money_rec=0;
	customer.rented=false;
	customer.lended=false;

	customer.menu();
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

void user::menu()	{

	int choice;
	bool flag=true;
	while(flag)	{
		cout<<"\n1.Rent a bike\n2.Put up a bike for renting\n3.View profile\n4.Edit Profile\n0.Logout\n\n";
		cin>>choice;
		switch(choice)	{
			case 1:	this->rent();
					break;
			case 2:	this->lend();
					break;
			case 3:	this->status();
					break;
			case 4:	this->edit();
					break;
			case 0:	flag=false;
					break;
			default:cout<<"Incorrect input.Please enter a valid choice.\n";
					break; 

		}

	}
}

void user::status()	{

	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==this->uname)
			break;

	double return_rent=(*sheet).readNum(row,6);
	double return_lend=(*sheet).readNum(row,9);
	int return_r[3],return_l[3];
	(*user_db).dateUnpack(return_rent,&return_r[0],&return_r[1],&return_r[2]);
	(*user_db).dateUnpack(return_lend,&return_l[0],&return_l[1],&return_l[2]);
	
	cout<<"\n---- Customer Profile ----\n\n";
	cout<<"\nName-"<<this->name;
	cout<<"\nUsername-"<<this->uname;
	cout<<"\nContact Number-"<<this->contact;
	cout<<"\n\n**CYCLES RENTED**\n";
	cout<<"\nRented from-"<<this->getName((*sheet).readStr(row,4));
	cout<<"\nContact Number-"<<(*sheet).readStr(row,5);
	cout<<"\nReturn on-"<<return_r[2]<<"/"<<return_r[1]<<"/"<<return_r[0];
	cout<<"\n\n**CYCLES PUT OUT ON RENT**\n";
	cout<<"\nRented to-"<<this->getName((*sheet).readStr(row,7));
	cout<<"\nContact Number-"<<(*sheet).readStr(row,8);
	cout<<"\nTo be returned on-"<<return_l[2]<<"/"<<return_l[1]<<"/"<<return_l[0];
	cout<<"\n\nMoney Payable-"<<this->money_pay<<"\n";
	cout<<"Money Receivable-"<<this->money_rec<<"\n\n";
	cout<<"--------------------\n\n";

	(*user_db).release();
	return;
}

void user::edit()	{

	libxl::Book* user_db=xlCreateXMLBook();
	(*user_db).load("users_list.xlsx");
	libxl::Sheet* sheet=(*user_db).getSheet(0);
	int row=(*sheet).firstRow();
	for(;row!=(*sheet).lastRow();row++)
		if((*sheet).readStr(row,0)==this->uname)
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
						this->contact=no;
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

void user::lend()	{

	libxl::Book* rental_db=xlCreateXMLBook();
	
	if(!system("test -e rental_list.xlsx"))	{

		(*rental_db).load("rental_list.xlsx");
		libxl::Sheet* sheet=(*rental_db).getSheet(0); //Sheet 1 contains the bikes available for renting
		libxl::Format* dformat=(*rental_db).addFormat();
		(*dformat).setNumFormat(libxl::NUMFORMAT_DATE);

		int row=(*sheet).lastRow();
		// user u;
		(*sheet).writeStr(row,6,this->uname.c_str());
		// (*sheet).writeStr(row,7,(u.getContact(uname)).c_str());

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
		int s,input;
		cin>>s;
		switch(s)	{
			case 1:	input=500;
					break;
			case 2:	input=800;
					break;
			case 3:	input=1000;
					break;
		}

		(*sheet).writeNum(row,2,input);
		
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
		string arr[]={"Brand","Geared","Price","Location","Details","Available till","Owner Username"};

		for(int i=0;i<=6;i++)
			(*sheet).writeStr(1,i,arr[i].c_str());

		int row=(*sheet).lastRow();

		// user u;
		(*sheet).writeStr(row,6,this->uname.c_str());
		// (*sheet).writeStr(row,7,(u.getContact(uname)).c_str());

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
		int s,input;
		cin>>s;
		switch(s)	{
			case 1:	input=500;
					break;
			case 2:	input=800;
					break;
			case 3:	input=1000;
					break;
		}

		(*sheet).writeNum(row,2,input);
		
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

void user::rent()	{
	
	// user u;
	if(!system("test -e rental_list.xlsx"))	{
		cout<<"Please choose the price filter [press '0' to skip]\n1.Rs.500/day only\n2.Rs.800/day only\n3.Rs.1000/day only\n4.<=Rs.800/day\n";
		int pr;
		cin>>pr;


		cout<<"Please choose the brand filter [press '0' to skip]\n";
		
		string* brands=new string[25];
		int l=this->getBrands(brands);
		for(int i=0;i<l;i++)
			cout<<i+1<<"."<<brands[i]<<endl;
		int br;
		cin>>br;

		cout<<"Please choose the geared bikes filter [press '0' to skip]\n1.Geared only\n2.Non-geared only\n"; 
		int gr;
		cin>>gr;

		cout<<"Please enter the number of days you want to rent-";
		int d;
		cin>>d;

		int filters[4]={pr,br,gr,d};
		this->runQuery(filters);

	}
	else
		cout<<"\nNo bikes available for rent currently!\n";

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
	
	// user u;
	libxl::Book* rental_db=xlCreateXMLBook();
	libxl::Book* query_db=xlCreateXMLBook();

	(*rental_db).load("rental_list.xlsx");
	
	libxl::Format* dformat=(*rental_db).addFormat();
	(*dformat).setNumFormat(libxl::NUMFORMAT_DATE);
	libxl::Format* dateformat=(*query_db).addFormat();
	(*dateformat).setNumFormat(libxl::NUMFORMAT_DATE);


	libxl::Sheet* r_sheet=(*rental_db).getSheet(0);
	libxl::Sheet* q_sheet=(*query_db).addSheet("Sheet1");

	for(int i=0;i<=6;i++)	
		(*q_sheet).writeStr(1,i,(*r_sheet).readStr(1,i));

	

	string brand_f="";

	//Brand filter
	if(filters[1]!=0)	{
		string* br_list=new string[25];
		getBrands(br_list);
		brand_f=br_list[(filters[1]-1)];
	}
	else
		brand_f="";

	//Geared filter
	string geared_f="";
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

	int* till_date=this->getDate(filters[3]);
	
	//Price filter
	int price_f[2]={0,0};
	switch(filters[0])	{
		case 1:	price_f[0]=500;
				price_f[1]=0;
				break;
		case 2:	price_f[0]=800;
				price_f[1]=0;
				break;
		case 3:	price_f[0]=1000;
				price_f[1]=0;
				break;
		case 4:	price_f[0]=500;
				price_f[1]=800;
				break;
		// case 0:	price_f[0]="";
		// 		price_f[1]="";
		// 		break;
		default:price_f[0]=0;
				price_f[1]=0;
				break;
	}

	
	cout<<"\nPrice-"<<price_f[0]<<" "<<price_f[1];
	cout<<"\ngeared-"<<geared_f;
	cout<<"\nbrand-"<<brand_f;
	cout<<"\ndate-"<<till_date[0]<<till_date[1]<<till_date[2]<<endl;

	bool match=true;
	// cout<<"FLAG INITIAL VALUE="<<match<<endl;
	int query_row=2;

	for(int i=2;i!=(*r_sheet).lastRow();i++)	{
		
		match=true;
		
		if(brand_f!="")	{
			// cout<<"brand_check\n";
			cout<<(*r_sheet).readStr(i,0)<<endl;
			if((*r_sheet).readStr(i,0)!=brand_f)	{
				// cout<<"Fuck\n";
				match=false;
			}
		}

		if(price_f[0]!=0)	{
			// cout<<"price_check\n";
			cout<<(*r_sheet).readNum(i,2)<<endl;
			if((*r_sheet).readNum(i,2)!=price_f[0] && (*r_sheet).readNum(i,2)!=price_f[1])	{
				// cout<<"Fuck2\n";
				match=false;
			}
		}

		if(geared_f!="")	{
			// cout<<"gear_check\n";
			cout<<(*r_sheet).readStr(i,1)<<endl;
			if((*r_sheet).readStr(i,1)!=geared_f)	{
				// cout<<"Fuck3\n";
				match=false;			
			}
		}

		int yr,mon,day;
		double read_date=(*r_sheet).readNum(i,5);
		(*rental_db).dateUnpack(read_date,&yr,&mon,&day);
		// cout<<"Avail_date="<<yr<<" "<<mon<<" "<<day<<endl;
		// cout<<"date_needed-"<<till_date[0]<<" "<<till_date[1]<<" "<<till_date[2]<<endl;

		if(yr!=0)	{
			// cout<<"date_check\n";

			if(yr>=till_date[0]){
				if(mon>till_date[1]){
					// continue;
				}
				else
					if(mon==till_date[1])	{
						if(day>=till_date[2]){
							// continue;
						}
						else	{
							// cout<<"Fuck_day\n";
							match=false;				
						}
					}
				else	{
					// cout<<"Fuck_month\n";
					match=false;
				}
			}
			else	{
				// cout<<"Fuck_yr\n";
				match=false;				
			}
		}

		// cout<<"for i="<<i<<endl;
		// cout<<"match="<<match<<endl<<endl;
		if(match==true)	{
			// cout<<"writing\n";
			for(int j=0;j<=6;j++)	{
				if(j!=5 && j!=2)	
					(*q_sheet).writeStr(query_row,j,(*r_sheet).readStr(i,j));
				
				else	{
					if(j==5)
						(*q_sheet).writeNum(query_row,j,(*r_sheet).readNum(i,j),dateformat);
					else
						(*q_sheet).writeNum(query_row,j,(*r_sheet).readNum(i,j));
				}
	


			// user x;
			string name=this->getName((*r_sheet).readStr(i,6));
			string cno=this->getContact((*r_sheet).readStr(i,6));
			(*q_sheet).writeStr(query_row,7,name.c_str());
			(*q_sheet).writeStr(query_row,8,cno.c_str());

			}
			query_row++;
		}
		// cout<<name<<cno;
		// cout<<"**\n";
	}
	// (*r_sheet).insertCol((*r_sheet).lastCol(),(*r_sheet).lastCol());
	// (*r_sheet).insertCol((*r_sheet).lastCol(),(*r_sheet).lastCol());

	(*q_sheet).writeStr(1,7,"Owner Name");
	(*q_sheet).writeStr(1,8,"Contact Number");
	// (*r_sheet).insertCol((*r_sheet).lastCol(),(*r_sheet).lastCol());
	int col=9;
	(*q_sheet).writeStr(1,9,"uniqueID");
	for(int i=2;i!=(*q_sheet).lastRow();i++)
		(*q_sheet).writeNum(i,col,i-1);

	(*query_db).save("queryResults.xlsx");
	(*query_db).release();
	(*rental_db).release();
	cout<<"\nA 'queryResults.xlsx' has been created.Please enter the 'uniqueID' of the bike you want to rent or enter '0' to discard query\n";
	int ch;
	cin>>ch;
	if(ch==0)
		return;
	else
		this->confirmRent(ch,till_date,filters[3]);

	return;

	//Copied from backup
	
}

void user::confirmRent(int ch,int* till_date,int days)	{

	// string uname,pass;
	// cout<<"Please enter username and password to confirm-\n";
	// cout<<"Username:";
	// cin>>uname;
	// cout<<"Password:";
	// cin>>pass;

	libxl::Book* rental=xlCreateXMLBook();
	libxl::Book* query=xlCreateXMLBook();

	(*rental).load("rental_list.xlsx");
	(*query).load("queryResults.xlsx");

	libxl::Format* dateformat=(*rental).addFormat();
	(*dateformat).setNumFormat(libxl::NUMFORMAT_DATE);

	libxl::Sheet* r_sheet=(*rental).getSheet(0);
	libxl::Sheet* l_sheet=(*rental).getSheet(1);

	libxl::Sheet* q_sheet=(*query).getSheet(0);

	string owner;
	int price;

	if(l_sheet!=NULL)	{

		int row=(*l_sheet).lastRow();

		for(int i=0;i<=6;i++)	{
			if(i!=5 && i!=2)
				(*l_sheet).writeStr(row,i,(*q_sheet).readStr(ch+1,i));
			else	{
				if(i==5)
					(*l_sheet).writeNum(row,i,(*q_sheet).readNum(ch+1,i),dateformat);
				else	{
					price=(*q_sheet).readNum(ch+1,i);
					(*l_sheet).writeNum(row,i,(*q_sheet).readNum(ch+1,i));
				}
			}

		}
		owner=(*l_sheet).readStr(row,6);
		(*l_sheet).writeStr(row,7,this->uname.c_str());
		(*l_sheet).writeNum(row,8,(*rental).datePack(till_date[0],till_date[1],till_date[2]),dateformat);

	}
	else	{
		//Create
		l_sheet=(*rental).addSheet("Sheet2");
		string head[]={"Brand","Geared","Price","Location","Details","Available till","Owner Username","Renter Username","Due Date"};
		for(int i=0;i<=8;i++)
			(*l_sheet).writeStr(1,i,head[i].c_str());

		int row=(*l_sheet).lastRow();

		for(int i=0;i<=6;i++)	{
			if(i!=5 && i!=2)
				(*l_sheet).writeStr(row,i,(*q_sheet).readStr(ch+1,i));
			else	{
				if(i==5)
					(*l_sheet).writeNum(row,i,(*q_sheet).readNum(ch+1,i),dateformat);
				else	{
					price=(*q_sheet).readNum(ch+1,i);
					(*l_sheet).writeNum(row,i,(*q_sheet).readNum(ch+1,i));
				}
			}

		}

		owner=(*l_sheet).readStr(row,6);	
		(*l_sheet).writeStr(row,7,this->uname.c_str());
		(*l_sheet).writeNum(row,8,(*rental).datePack(till_date[0],till_date[1],till_date[2]),dateformat);

	}
	//Delete the rented bike from 'to-rent' list
	int find_row;
	for(find_row=2;find_row<(*r_sheet).lastRow();find_row++)	{
		if((*r_sheet).readStr(find_row,6)==owner)
			break;
	}

	(*r_sheet).removeRow(find_row,find_row);

	//Update status in users' database
	libxl::Book* users_db=xlCreateXMLBook();
	(*users_db).load("users_list.xlsx");
	libxl::Sheet* u_sheet=(*users_db).getSheet(0);
	libxl::Format* d_format=(*users_db).addFormat();
	(*d_format).setNumFormat(libxl::NUMFORMAT_DATE);


	int money=price*days;
	// cout<<money'\n';
	for(int i=2;i<(*u_sheet).lastRow();i++)	{
		
		if((*u_sheet).readStr(i,0)==owner)	{
			(*u_sheet).writeStr(i,7,this->uname.c_str());
			(*u_sheet).writeStr(i,8,this->contact.c_str());
			int temp=(*u_sheet).readNum(i,11);
			(*u_sheet).writeNum(i,11,temp+money);
			(*u_sheet).writeNum(i,9,(*users_db).datePack(till_date[0],till_date[1],till_date[2]),d_format);
		}

		if((*u_sheet).readStr(i,0)==this->uname)	{
			(*u_sheet).writeStr(i,4,owner.c_str());
			(*u_sheet).writeStr(i,5,this->getContact(owner).c_str());
			int temp=(*u_sheet).readNum(i,10);
			(*u_sheet).writeNum(i,10,temp+money);
			(*u_sheet).writeNum(i,6,(*users_db).datePack(till_date[0],till_date[1],till_date[2]),d_format);
		}
	}

	this->money_pay+=money;
	this->rented=true;
	cout<<"Bike rented successfully!\nReturn date-"<<till_date[2]<<"/"<<till_date[1]<<"/"<<till_date[0]<<endl;
	cout<<"Location:"<<(*q_sheet).readStr(ch+1,3)<<endl;
	system("rm -f queryResults.xlsx");
	(*rental).save("rental_list.xlsx");
	(*users_db).save("users_list.xlsx");
	(*rental).release();
	(*query).release();
	(*users_db).release();

	return;
}


string user::getName(string uname)	{

	if(uname=="-")
		return "-";
	
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

	if(uname=="-")
		return "-";
	
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
