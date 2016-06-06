#ifndef LOAD
#define LOAD
#include <stdlib.h> 
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include "Triple.h"
#include "TurtleParser.h"
#include "RDFParser.h"
#include "../operate/operate.h"
//#include "md5.h"
//#include "operate.h"

using namespace std;
//void encode_RDF(const string rdf_file);
int loadrdf(const string rdf_file,const string database);
unsigned long node_exist(  const char *table,char *node_md5);
unsigned long inlinenode(char nodetype,vector<string> v);
int split(const string& str, vector<string>& ret_, string sep);
void  InsertIndex(unsigned long s,unsigned long p,unsigned long o);
float GetFloat32( std::string Binary );
int Binary2Hex( std::string Binary );



 class  datetime
{

public:
	 datetime(){};
	~ datetime(){};


void setyear(string year){
	 _year = atoi(year.c_str());
	int temp =_year;
	b_year="";
	while(temp!=0)
	{

	string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_year=b_year+S;
		temp=temp/2;


	}
	reverse(b_year.begin(),b_year.end());
	if(b_year.size()<13)
	{
		for (int i =13-b_year.size() ; i >0; i--)
		{
			b_year=zero+b_year;
		}
	}


}


void setmonth(string month){
 _month = atoi(month.c_str());
	int temp =_month;
	b_month="";
	while(temp!=0)
	{

	string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_month=b_month+S;
		temp=temp/2;


	}
	reverse(b_month.begin(),b_month.end());
		if(b_month.size()<4)
	{
		for (int i =4-b_month.size() ; i >0; i--)
		{
			b_month=zero+b_month;
			
		}


	}

}
void setday(string day){
 _day = atoi(day.c_str());
int temp=_day;
b_day="";

	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_day=b_day+S;
		temp=temp/2;

	}
	reverse(b_day.begin(),b_day.end());
	if(b_day.size()<5)
	{
		for (int i =5-b_day.size() ; i >0; i--)
		{
			b_day=zero+b_day;
			
		}
	}
}
void setyhour(string hour){
 _hour = atoi(hour.c_str());
int temp=_hour;
b_hour="";

	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_hour=b_hour+S;
		temp=temp/2;

	}
	reverse(b_hour.begin(),b_hour.end());
	if(b_hour.size()<5)
	{
		for (int i =5-b_hour.size() ; i >0; i--)
		{
			b_hour=zero+b_hour;
			
		}
	}

}
void setmin(string min){
 _min = atoi(min.c_str());
int temp=_min;
b_min="";

	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_min=b_min+S;
		temp=temp/2;

	}
	reverse(b_min.begin(),b_min.end());
	if(b_min.size()<6)
	{
		for (int i =6-b_min.size() ; i >0; i--)
		{
			b_min=zero+b_min;
			
		}
	}

}
void setsec(string sec){
 _sec = atoi(sec.c_str());
int temp=_sec;
b_sec="";

	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_sec=b_sec+S;
		temp=temp/2;

	}
	reverse(b_sec.begin(),b_sec.end());
		if(b_sec.size()<6)
	{
		for (int i =6-b_sec.size() ; i >0; i--)
		{
			b_sec=zero+b_sec;
			
		}
	}
}

void setmsec(string msec){
 _msec = atoi(msec.c_str());
int temp=_msec;
b_msec="";

	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_msec=b_msec+S;
		temp=temp/2;
	}
	reverse(b_msec.begin(),b_msec.end());
	if(b_msec.size()<10)
	{
		for (int i =10-b_msec.size() ; i >0; i--)
		{
			b_msec=zero+b_msec;
			
		}
	}

}


int _year;
int _month;
int _day;
string b_year;
string b_month;
string b_day;
int _hour;
int _min;
int _sec;
int _msec;
string b_hour;
string b_min;
string b_sec;
string b_msec;
string zero="0";

	
};

class integer
{
public:
	integer(){};
	~integer(){};


void setint(string integer){
	_integer = atoi(integer.c_str());
	int temp=_integer;
	b_integer="";
	while(temp!=0)
	{
		string S = static_cast<ostringstream*>( &(ostringstream() << (temp%2)) )->str();
		b_integer=b_integer+S;
		temp=temp/2;
	}
	reverse(b_integer.begin(),b_integer.end());

	if(b_integer.size()<56)
	{
		for (int i =56-b_integer.size() ; i >0; i--)
		{
			b_integer=zero+b_integer;
			
		}
	}

 }

 int _integer;
 string b_integer;
 string zero="0";
	
};
class decimal
{
public:
	decimal(){};
	~decimal(){};

void set(string str_decimal){

 f=stof(str_decimal);
    bitset<sizeof f*8> a(*(long unsigned int*)(&f));
   b_decimal=a.to_string();

	if(56>b_decimal.size())
	{
		for (int i =56-b_decimal.size() ; i >0; i--)
		{
			b_decimal=b_decimal+zero;

		}
	}
 }




 float f;
 int decimalvalue;
 string b_decimal;
 string zero="0";

};


template< typename T >
string int_to_hex( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)*2)<< std::hex << i;
         string result=stream.str();
  return result;
}
template <class T>
 std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}
#endif
