#include "header.h"

void db_access::createDB(string sheet)	{

	this->book=xlCreateXMLBook();
	this->sheet1=(*(this->book)).addSheet(sheet.c_str());
	this->format=(*(this->book)).addFormat();
	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

	return;
}


void db_access::loadDB(string db_name,int sheet)	{

	this->book=xlCreateXMLBook();
	(*(this->book)).load(db_name.c_str());
	this->sheet1=(*(this->book)).getSheet(sheet);
	this->format=(*(this->book)).addFormat();
	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

	return;
}


void db_access::loadDB(string db_name,int sheet1,int sheet2)	{

	this->book=xlCreateXMLBook();
	(*(this->book)).load(db_name.c_str());
	this->sheet1=(*(this->book)).getSheet(sheet1);
	this->sheet2=(*(this->book)).getSheet(sheet2);
	this->format=(*(this->book)).addFormat();
	(*(this->format)).setNumFormat(libxl::NUMFORMAT_DATE);

	return;
}

void db_access::saveDB(string db_name)	{

	(*(this->book)).save(db_name.c_str());
	(*(this->book)).release();

	return;
}

void db_access::clearMem()	{
	
	(*(this->book)).release();

	return;
}

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

			db_access users;
			users.loadDB("users_list.xlsx",0);

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
				for(;row!=(*(users.sheet1)).lastRow();row++)	{
					if(uname==(*(users.sheet1)).readStr(row,0) && pwd==(*(users.sheet1)).readStr(row,1))	{
						flag=true;
						name=(*(users.sheet1)).readStr(row,2);
						contact=(*(users.sheet1)).readStr(row,3);
						money_p=(*(users.sheet1)).readNum(row,10);
						money_r=(*(users.sheet1)).readNum(row,11);
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
				user customer;
				customer.uname=uname;
				customer.name=name;
				customer.contact=contact;
				customer.money_pay=money_p;
				customer.money_rec=money_r;
				string r=(*(users.sheet1)).readStr(row,4);
				string l=(*(users.sheet1)).readStr(row,7);
				if(r=="-")	{
					customer.rented=false;
				}
				else	{
					customer.rented=true;
				}
				if(l=="-")	{
					customer.lended=false;
				}
				else	{
					customer.lended=true;
				}
				
				users.saveDB("users_list.xlsx");		
				customer.menu();
			}
		
	}
	else
		cout<<"Users' database not found!\n";
	
	return;
}

void authenticate::new_user()	{

	string uname,name,contact;
	if(!system("test -e users_list.xlsx"))	{

			db_access users;
			users.loadDB("users_list.xlsx",0);

			string pwd;
			bool flag=true;
			while(flag)	{
				cout<<"Enter the desired username:";
				cin>>uname;
				if(!chk_uname(uname))	{
					int row=(*(users.sheet1)).lastRow();
					(*(users.sheet1)).writeStr(row,0,uname.c_str());
					
					cout<<"Pease enter password:";
					cin>>pwd;
					(*(users.sheet1)).writeStr(row,1,pwd.c_str());
					

					cout<<"Please enter your name:";
					cin.ignore();
					char n[100];
					cin.getline(n,100);
					string c_name(n);
					(*(users.sheet1)).writeStr(row,2,c_name.c_str());
					name=c_name;

					string c_no;
					cout<<"Please enter your contact number:";
					cin>>c_no;
					(*(users.sheet1)).writeStr(row,3,c_no.c_str());
					contact=c_no;
					
					for(int i=4;i<=9;i++)	
						(*(users.sheet1)).writeStr(row,i,"-");
					(*(users.sheet1)).writeNum(row,10,0);
					(*(users.sheet1)).writeNum(row,11,0);

					cout<<"\nWelcome "<<(*(users.sheet1)).readStr(row,2)<<"\nWhat can we do for you today?\n";

					flag=false;
				}
				else
					cout<<"Sorry,the username already exists.Please choose another\n";
			
			}
		
			users.saveDB("users_list.xlsx");
		}
	else	{
		//Create new database
		db_access users;
		users.createDB("Sheet1");

		(*(users.sheet1)).writeStr(1,0,"Username");
		(*(users.sheet1)).writeStr(1,1,"Password");
		(*(users.sheet1)).writeStr(1,2,"Name");
		(*(users.sheet1)).writeStr(1,3,"Contact Number");
		(*(users.sheet1)).writeStr(1,4,"Rented from");
		(*(users.sheet1)).writeStr(1,5,"Contact Number ");
		(*(users.sheet1)).writeStr(1,6,"Due Date");
		(*(users.sheet1)).writeStr(1,7,"Rented to");
		(*(users.sheet1)).writeStr(1,8,"Contact Number");
		(*(users.sheet1)).writeStr(1,9,"Due Date");
		(*(users.sheet1)).writeStr(1,10,"Money Payable");
		(*(users.sheet1)).writeStr(1,11,"Money Receivable");


		string pwd;
		cout<<"Enter the desired username:";
		cin>>uname;
		int row=(*(users.sheet1)).lastRow();
		(*(users.sheet1)).writeStr(row,0,uname.c_str());

		cout<<"Pease enter password:";
		cin>>pwd;
		(*(users.sheet1)).writeStr(row,1,pwd.c_str());
		
		cout<<"Please enter your name:";
		cin.ignore();
		char n[100];
		cin.getline(n,100);
		string c_name(n);
		(*(users.sheet1)).writeStr(row,2,c_name.c_str());
		name=c_name;
		
		string c_no;
		cout<<"Please enter your contact number:";
		cin>>c_no;
		(*(users.sheet1)).writeStr(row,3,c_no.c_str());
		contact=c_no;

		for(int i=4;i<=9;i++)	
			(*(users.sheet1)).writeStr(row,i,"-");
		(*(users.sheet1)).writeNum(row,10,0);
		(*(users.sheet1)).writeNum(row,11,0);
		
		users.saveDB("users_list.xlsx");

		cout<<"\nWelcome "<<c_name<<"\nWhat can we do for you today?\n";

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

	db_access users;
	users.loadDB("users_list.xlsx",0);	

	int row=2,column=0;
	for(;row!=(*(users.sheet1)).lastRow();row++)	{
		if((*(users.sheet1)).readStr(row,0)==uname)	{
			status=true;
			break;
		}
	}

	users.saveDB("users_list.xlsx");

	return status;
}

void user::menu()	{

	int choice;
	bool flag=true;

	while(flag)	{
		cout<<"\n1.Rent a bike\n2.Put up a bike for renting\n3.View profile\n4.Edit Profile\n5.Mark bike as 'returned'\n6.Remove bike listing\n0.Logout\n\n";
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
			case 5:	this->returnBike();
					break;
			case 6:	this->removeAd();
					break;		
			case 0:	flag=false;
					break;
			default:cout<<"Incorrect input.Please enter a valid choice.\n";
					break; 

		}

	}
}

void user::status()	{

	db_access users;
	users.loadDB("users_list.xlsx",0);

	int row=(*(users.sheet1)).firstRow();
	for(;row!=(*(users.sheet1)).lastRow();row++)
		if((*(users.sheet1)).readStr(row,0)==this->uname)
			break;

	double return_rent=(*(users.sheet1)).readNum(row,6);
	double return_lend=(*(users.sheet1)).readNum(row,9);
	int return_r[3],return_l[3];
	(*(users.book)).dateUnpack(return_rent,&return_r[0],&return_r[1],&return_r[2]);
	(*(users.book)).dateUnpack(return_lend,&return_l[0],&return_l[1],&return_l[2]);
	
	cout<<"\n---- Customer Profile ----\n\n";
	cout<<"\nName-"<<this->name;
	cout<<"\nUsername-"<<this->uname;
	cout<<"\nContact Number-"<<this->contact;
	cout<<"\n\n**CYCLES RENTED**\n";
	cout<<"\nRented from-"<<this->getName((*(users.sheet1)).readStr(row,4));
	cout<<"\nContact Number-"<<(*(users.sheet1)).readStr(row,5);
	cout<<"\nReturn on-"<<return_r[2]<<"/"<<return_r[1]<<"/"<<return_r[0];
	cout<<"\n\n**CYCLES PUT OUT ON RENT**\n";
	cout<<"\nRented to-"<<this->getName((*(users.sheet1)).readStr(row,7));
	cout<<"\nContact Number-"<<(*(users.sheet1)).readStr(row,8);
	cout<<"\nTo be returned on-"<<return_l[2]<<"/"<<return_l[1]<<"/"<<return_l[0];
	cout<<"\n\nMoney Payable-"<<this->money_pay<<"\n";
	cout<<"Money Receivable-"<<this->money_rec<<"\n\n";
	cout<<"--------------------\n\n";

	users.saveDB("users_list.xlsx");
	return;
}

void user::edit()	{

	db_access users;
	users.loadDB("users_list.xlsx",0);

	int row=(*(users.sheet1)).firstRow();
	for(;row!=(*(users.sheet1)).lastRow();row++)
		if((*(users.sheet1)).readStr(row,0)==this->uname)
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
						(*(users.sheet1)).writeStr(row,3,no.c_str());
						cout<<"\nContact number updated successfully!\n";
						break;
					}

			case 2:	{
						string pwd;
						cout<<"Please enter your new password:";
						cin>>pwd;
						(*(users.sheet1)).writeStr(row,1,pwd.c_str());
						cout<<"Password changed successfully!\n";
						break;
					}
						
			case 3: flag=false;
					break;
			default:cout<<"Invalid choice.Please enter a valid choice\n";
					break;
		}

	}
	users.saveDB("users_list.xlsx");
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

	
	if(!system("test -e rental_list.xlsx"))	{

		db_access rental;
		rental.loadDB("rental_list.xlsx",0);

		int row=(*(rental.sheet1)).lastRow();
		
		(*(rental.sheet1)).writeStr(row,6,this->uname.c_str());

		string  inp;
		cout<<"Please fill out the necessary details-\n";
		
		cout<<"\nBrand-";
		cin>>inp;
		for(int i=0;i<inp.length();i++)
			inp[i]=tolower(inp[i]);

		(*(rental.sheet1)).writeStr(row,0,inp.c_str());
		
		cout<<"\nGeared [y/n]-";
		cin>>inp;
		if(inp=="y")
			inp="Yes";
		else
			inp="No";
		(*(rental.sheet1)).writeStr(row,1,inp.c_str());
		
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

		(*(rental.sheet1)).writeNum(row,2,input);
		
		cout<<"\nPlease enter your location-";
		cin.ignore();
		char str[100];
		cin.getline(str,100); 	
		(*(rental.sheet1)).writeStr(row,3,str);
		
		cout<<"\nPlease enter the date till you want to list the bike for rent in DD MM YYYY format\n[If you want to list it indefinitely/till deletion, please enter '00 00 0000']\n";
		int d,m,y;
		cin>>d>>m>>y;
		(*(rental.sheet1)).writeNum(row,5,(*(rental.book)).datePack(y,m,d),rental.format);
		
		cout<<"\nPlease enter additional details (if any)\n[Press 'Enter' to skip/end comment]\n";
		char com[1000];
		cin.ignore();
		cin.getline(com,1000);	
		(*(rental.sheet1)).writeStr(row,4,com);

		cout<<"\nList the bike for rental? [y/n]-";
		cin>>inp;
		if(inp=="y")	{
			rental.saveDB("rental_list.xlsx");
			cout<<"\nBike listed up for rent successfully!\n\n";
		}
		else	{
			rental.clearMem();
			cout<<"\nBike listing request discarded!\n\n";
		}

	}
	else	{
		//Create new
		db_access rental;
		rental.createDB("Sheet1");

		string arr[]={"Brand","Geared","Price","Location","Details","Available till","Owner Username"};

		for(int i=0;i<=6;i++)
			(*(rental.sheet1)).writeStr(1,i,arr[i].c_str());

		int row=(*(rental.sheet1)).lastRow();

		(*(rental.sheet1)).writeStr(row,6,this->uname.c_str());

		string  inp;
		cout<<"Please fill out the necessary details-\n";
		
		cout<<"\nBrand-";
		cin>>inp;
		for(int i=0;i<inp.length();i++)
			inp[i]=tolower(inp[i]);

		(*(rental.sheet1)).writeStr(row,0,inp.c_str());
		
		cout<<"\nGeared [y/n]-";
		cin>>inp;
		if(inp=="y")
			inp="Yes";
		else
			inp="No";
		(*(rental.sheet1)).writeStr(row,1,inp.c_str());
		
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

		(*(rental.sheet1)).writeNum(row,2,input);
		
		cout<<"\nPlease enter your location-";
		cin.ignore();
		char str[100];
		cin.getline(str,100);		
		(*(rental.sheet1)).writeStr(row,3,str);
		
		cout<<"\nPlease enter the date till you want to list the bike for rent in DD MM YYYY format\n[If you want to list it indefinitely/till deletion, please enter '00 00 0000']\n";
		int d,m,y;
		cin>>d>>m>>y;
		(*(rental.sheet1)).writeNum(row,5,(*(rental.book)).datePack(y,m,d),rental.format);
		
		cout<<"\nPlease enter additional details (if any)\n[Press 'Enter' to skip/end comment]\n";
		char com[1000];
		cin.ignore();
		cin.getline(com,1000);
		(*(rental.sheet1)).writeStr(row,4,com);

		cout<<"\nList the bike for rental? [y/n]-";
		cin>>inp;
		if(inp=="y")	{
			rental.saveDB("rental_list.xlsx");
			cout<<"\nBike put up for rent successfully!\n\n";
		}
		else	{
			rental.clearMem();
			cout<<"\nBike listing request discarded!\n\n";
		}

	}

return;
}

void user::rent()	{
	
	if(!system("test -e rental_list.xlsx"))	{
		
		this->updateDB();

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

	db_access rental;
	rental.loadDB("rental_list.xlsx",0);

	int row=(*(rental.sheet1)).lastRow();
	string brand_data[row-2];//Since row data starts from row index 2 and continues upto 'row'-1
	for(int i=2;i<row;i++)
		brand_data[i-2]=(*(rental.sheet1)).readStr(i,0);

	rental.saveDB("rental_list.xlsx");

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

	db_access rental,query;
	rental.loadDB("rental_list.xlsx",0);
	query.createDB("Sheet1");

	for(int i=0;i<=6;i++)	
		(*(query.sheet1)).writeStr(1,i,(*(rental.sheet1)).readStr(1,i));

	

	string brand_f="";

	//Brand filter
	if(filters[1]!=0)	{
		string* br_list=new string[25];
		this->getBrands(br_list);
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
		default:price_f[0]=0;
				price_f[1]=0;
				break;
	}

	bool match=true;
	int query_row=2;

	for(int i=2;i!=(*(rental.sheet1)).lastRow();i++)	{
		
		match=true;
		
		if(brand_f!="")	{
			if((*(rental.sheet1)).readStr(i,0)!=brand_f)	{
				match=false;
			}
		}

		if(price_f[0]!=0)	{
			if((*(rental.sheet1)).readNum(i,2)!=price_f[0] && (*(rental.sheet1)).readNum(i,2)!=price_f[1])	{
				match=false;
			}
		}

		if(geared_f!="")	{
			if((*(rental.sheet1)).readStr(i,1)!=geared_f)	{
				match=false;			
			}
		}

		int yr,mon,day;
		double read_date=(*(rental.sheet1)).readNum(i,5);
		(*(rental.book)).dateUnpack(read_date,&yr,&mon,&day);

		if(yr!=0)	{

			if(yr>=till_date[0]){
				if(mon>till_date[1]){
				}
				else
					if(mon==till_date[1])	{
						if(day>=till_date[2]){
						}
						else	{
							match=false;				
						}
					}
				else	{
					match=false;
				}
			}
			else	{
				match=false;				
			}
		}

		if(match==true)	{
			for(int j=0;j<=6;j++)	{
				if(j!=5 && j!=2)	
					(*(query.sheet1)).writeStr(query_row,j,(*(rental.sheet1)).readStr(i,j));
				
				else	{
					if(j==5)
						(*(query.sheet1)).writeNum(query_row,j,(*(rental.sheet1)).readNum(i,j),query.format);
					if(j==2)
						(*(query.sheet1)).writeNum(query_row,j,(*(rental.sheet1)).readNum(i,j));
				}
	


			string name=this->getName((*(rental.sheet1)).readStr(i,6));
			string cno=this->getContact((*(rental.sheet1)).readStr(i,6));
			(*(query.sheet1)).writeStr(query_row,7,name.c_str());
			(*(query.sheet1)).writeStr(query_row,8,cno.c_str());

			}
			query_row++;
		}
	}

	(*(query.sheet1)).writeStr(1,7,"Owner Name");
	(*(query.sheet1)).writeStr(1,8,"Contact Number");
	int col=9;
	(*(query.sheet1)).writeStr(1,9,"uniqueID");
	for(int i=2;i!=(*(query.sheet1)).lastRow();i++)
		(*(query.sheet1)).writeNum(i,col,i-1);

	query.saveDB("queryResults.xlsx");
	rental.saveDB("rental_list.xlsx");
	cout<<"\nA 'queryResults.xlsx' has been created.Please enter the 'uniqueID' of the bike you want to rent or enter '0' to discard query\n";
	int choice;
	cin>>choice;
	if(choice==0)	{
		system("rm -f queryResults.xlsx");
		cout<<"\nQuery discarded.\n";
		return;
	}
	else
		this->confirmRent(choice,till_date,filters[3]);

	return;	
}

void user::confirmRent(int ch,int* till_date,int days)	{

	db_access rental,query;
	rental.loadDB("rental_list.xlsx",0,1);
	query.loadDB("queryResults.xlsx",0);

	string owner;
	int price;

	if(rental.sheet2!=NULL)	{

		int row=(*(rental.sheet2)).lastRow();

		for(int i=0;i<=6;i++)	{
			if(i!=5 && i!=2)
				(*(rental.sheet2)).writeStr(row,i,(*(query.sheet1)).readStr(ch+1,i));
			else	{
				if(i==5)
					(*(rental.sheet2)).writeNum(row,i,(*(query.sheet1)).readNum(ch+1,i),rental.format);
				else	{
					price=(*(query.sheet1)).readNum(ch+1,i);
					(*(rental.sheet2)).writeNum(row,i,(*(query.sheet1)).readNum(ch+1,i));
				}
			}

		}
		owner=(*(rental.sheet2)).readStr(row,6);
		(*(rental.sheet2)).writeStr(row,7,this->uname.c_str());
		(*(rental.sheet2)).writeNum(row,8,(*(rental.book)).datePack(till_date[0],till_date[1],till_date[2]),rental.format);

	}
	else	{
		//Create
		(rental.sheet2)=(*(rental.book)).addSheet("Sheet2");
		string head[]={"Brand","Geared","Price","Location","Details","Available till","Owner Username","Renter Username","Due Date"};
		for(int i=0;i<=8;i++)
			(*(rental.sheet2)).writeStr(1,i,head[i].c_str());

		int row=(*(rental.sheet2)).lastRow();

		for(int i=0;i<=6;i++)	{
			if(i!=5 && i!=2)
				(*(rental.sheet2)).writeStr(row,i,(*(query.sheet1)).readStr(ch+1,i));
			else	{
				if(i==5)
					(*(rental.sheet2)).writeNum(row,i,(*(query.sheet1)).readNum(ch+1,i),rental.format);
				else	{
					price=(*(query.sheet1)).readNum(ch+1,i);
					(*(rental.sheet2)).writeNum(row,i,(*(query.sheet1)).readNum(ch+1,i));
				}
			}

		}

		owner=(*(rental.sheet2)).readStr(row,6);	
		(*(rental.sheet2)).writeStr(row,7,this->uname.c_str());
		(*(rental.sheet2)).writeNum(row,8,(*(rental.book)).datePack(till_date[0],till_date[1],till_date[2]),rental.format);
	}

	//Delete the rented bike from 'to-rent' list
	int find_row;
	for(find_row=2;find_row<(*(rental.sheet1)).lastRow();find_row++)	{
		if((*(rental.sheet1)).readStr(find_row,6)==owner)
			break;
	}

	(*(rental.sheet1)).removeRow(find_row,find_row);

	rental.saveDB("rental_list.xlsx");

	//Update status in users' database
	db_access users;
	users.loadDB("users_list.xlsx",0);

	int money=price*days;
	for(int i=2;i<(*(users.sheet1)).lastRow();i++)	{
		
		string uname=(*(users.sheet1)).readStr(i,0);
		if(uname==owner)	{
			(*(users.sheet1)).writeStr(i,7,this->uname.c_str());
			(*(users.sheet1)).writeStr(i,8,this->contact.c_str());
			int temp=(*(users.sheet1)).readNum(i,11);
			(*(users.sheet1)).writeNum(i,11,temp+money);
			(*(users.sheet1)).writeNum(i,9,(*(users.book)).datePack(till_date[0],till_date[1],till_date[2]),users.format);
		}

		if(uname==this->uname)	{
			(*(users.sheet1)).writeStr(i,4,owner.c_str());
			(*(users.sheet1)).writeStr(i,5,this->getContact(owner).c_str());
			int temp=(*(users.sheet1)).readNum(i,10);
			(*(users.sheet1)).writeNum(i,10,temp+money);
			(*(users.sheet1)).writeNum(i,6,(*(users.book)).datePack(till_date[0],till_date[1],till_date[2]),users.format);
		}
	}

	this->money_pay+=money;
	this->rented=true;
	cout<<"Bike rented successfully!\nReturn date-"<<till_date[2]<<"/"<<till_date[1]<<"/"<<till_date[0]<<endl;
	cout<<"Location:"<<(*(query.sheet1)).readStr(ch+1,3)<<endl;
	users.saveDB("users_list.xlsx");
	query.saveDB("queryResults.xlsx");

	system("rm -f queryResults.xlsx");

	return;
}

void user::updateDB()	{

	db_access rental;
	rental.loadDB("rental_list.xlsx",0);

	int* date=this->getDate(0);
	double date_today=(*(rental.book)).datePack(date[0],date[1],date[2]);

	for(int i=2;i!=(*(rental.sheet1)).lastRow();i++)	{

		double av_date=(*(rental.sheet1)).readNum(i,5);
		if(av_date!=0)	{
			if(av_date<=date_today)	{
				(*(rental.sheet1)).removeRow(i,i);
				i--;
			}
		}
	}

	rental.saveDB("rental_list.xlsx");

	return;
}

void user::returnBike()	{

	if(this->lended==true)	{

		db_access rental;
		rental.loadDB("rental_list.xlsx",0,1);

		int lended_row=2;
		string owner=this->uname;
		string renter;
		
		for(;lended_row!=(*(rental.sheet2)).lastRow();lended_row++)	{

			if((*(rental.sheet2)).readStr(lended_row,6)==owner)	
				break;
			
		}

		renter=(*(rental.sheet2)).readStr(lended_row,7);

		int row=(*(rental.sheet1)).lastRow();

		for(int i=0;i<=6;i++)	{

			if(i!=2 && i!=5)	{

				(*(rental.sheet1)).writeStr(row,i,(*(rental.sheet2)).readStr(lended_row,i));

			}
			else	{
				if(i==2)
					(*(rental.sheet1)).writeNum(row,i,(*(rental.sheet2)).readNum(lended_row,i));
				if(i==5)
					(*(rental.sheet1)).writeNum(row,i,(*(rental.sheet2)).readNum(lended_row,i),rental.format);

			}
		}

		(*(rental.sheet2)).removeRow(lended_row,lended_row);

		rental.saveDB("rental_list.xlsx");

		db_access users;
		users.loadDB("users_list.xlsx",0);

		for(int i=2;i!=(*(users.sheet1)).lastRow();i++)	{

			if((*(users.sheet1)).readStr(i,0)==renter)	{

				(*(users.sheet1)).writeStr(i,4,"-");
				(*(users.sheet1)).writeStr(i,5,"-");
				(*(users.sheet1)).writeStr(i,6,"-");
				(*(users.sheet1)).writeNum(i,10,0);

			}

			if((*(users.sheet1)).readStr(i,0)==owner)	{

				(*(users.sheet1)).writeStr(i,7,"-");
				(*(users.sheet1)).writeStr(i,8,"-");
				(*(users.sheet1)).writeStr(i,9,"-");
				(*(users.sheet1)).writeNum(i,11,0);

			}
		}

	this->lended=false;
	this->money_rec=0;
	
	cout<<"\nAccount information updated successfully!\n";

	users.saveDB("users_list.xlsx");
	}

	else
		cout<<"\nNo bike rented out yet!\n";

	return;
}


string user::getName(string uname)	{

	if(uname=="-")
		return "-";
	
	db_access users;
	users.loadDB("users_list.xlsx",0);

	int row=(*(users.sheet1)).firstRow();
	for(;row!=(*(users.sheet1)).lastRow();row++)
		if((*(users.sheet1)).readStr(row,0)==uname)
			break;

	string name=(*(users.sheet1)).readStr(row,2);
	
	users.saveDB("users_list.xlsx");

	return name;	
}

string user::getContact(string uname)	{

	if(uname=="-")
		return "-";
	
	db_access users;
	users.loadDB("users_list.xlsx",0);

	int row=(*(users.sheet1)).firstRow();
	for(;row!=(*(users.sheet1)).lastRow();row++)
		if((*(users.sheet1)).readStr(row,0)==uname)
			break;

	string contact=(*(users.sheet1)).readStr(row,3);
	
	users.saveDB("users_list.xlsx");
		
	return contact;	
}

void user::removeAd()	{


	if(!system("test -e rental_list.xlsx"))	{

		db_access rental;
		rental.loadDB("rental_list.xlsx",0,1);

		bool found=false;

		for(int i=2;i!=(*(rental.sheet1)).lastRow();i++)	{

			string owner=(*(rental.sheet1)).readStr(i,6);
			if(owner==this->uname)	{

				(*(rental.sheet1)).removeRow(i,i);
				cout<<"\nBike listing removed successfully!\n";
				found=true;
				rental.saveDB("rental_list.xlsx");
				break;
			}
		}

		if(found==false)	{

			bool flag=false;
			if((rental.sheet2)!=NULL)	{

				for(int i=2;i!=(*(rental.sheet2)).lastRow();i++)	{

					string owner=(*(rental.sheet2)).readStr(i,6);
					if(owner==this->uname)	{

						cout<<"\nYour bike is currently rented out.Please wait for its return\n";
						flag=true;
						rental.saveDB("rental_list.xlsx");
						break;
					}
				}
			}

			if(flag==false)	{

				cout<<"\nYou do not have any bike listed currently.\n";
				rental.saveDB("rental_list.xlsx");
			}
		}

	}
	else
		cout<<"\nNo bike listed for rental.\n";

	return;
}