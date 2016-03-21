#include "load.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "bt_query.h"
using namespace std;
int loadrdf(const string rdf_file,const string database)
    {
const char *table = database.data();
    ifstream _fin(rdf_file.c_str());
    if(!_fin)
    {
    cerr<<"fair to open~~"<<rdf_file<<endl;
    exit(0);
    }
    TripleWithObjType* triple_array =new TripleWithObjType[RDFParser::TRIPLE_NUM_PER_GROUP];
    RDFParser _parser(_fin);
    ifstream _file;
    _file.open(database.c_str(),ios::in);
     if(!_file)
	{
	   	cout<<"database not exist...cteate now"<<endl;
		CommandCreate(table);
	}
      else
	{
		cout<<"database found. insert soon"<<endl;
		_file.close();
		CommandOpen(table);
	}
    while(true)
    {
     int parse_triple_num = 100;
    // _parser.parseFile(triple_array,parse_triple_num);
    if(parse_triple_num ==0)
    {break;}

    for(int i=0; i<parse_triple_num;i++)
        {
        // for subject
         //string _sub = triple_array[i].getSubject();
         int _sub=i;
         cout<<"SSSSSSSSSS～～～～～～～～～"<<_sub<<endl;
	 int data=200;
	// CommandInsert(table, (void *)_sub ,strlen(_sub), (void *)data, sizeof(data));
	//CommandInsert(table, &_sub ,strlen(_sub.c_str()), &data, sizeof(data));
     CommandInsert(table, &_sub ,sizeof(int), &data, sizeof(data));
data++;
         //For predicate
         //string _pre = triple_array[i].getPredicate();
        	int _pre = i+100;
         CommandInsert(table, &_pre ,sizeof(int), &data, sizeof(data));
         cout<<"PPPPPPPPPP～～～～～～～～～"<<_pre<<endl;
         data++;
         //For object
        int _obj =i+1000;
       // string _obj = triple_array[i].getObject();
	 CommandInsert(table, &_obj ,sizeof(int), &data, sizeof(data));
	cout<<"count: "<<i<<endl;
         cout<<"OOOOOOOOOOO～～～～～～～～～"<<_obj<<endl;
        }

break;
    }
    CommandClose(table);
    CommandOpen(table);
   // int qu=1096;
   // int* qx;
    //qx=&qu;
    //bt_query_t q(bt_query_t::bt_eq, new int(1096),NULL);
    int ww=1096;
    bt_query_t q(bt_query_t::bt_eq,new int(ww),NULL);

     CommandSelect(table,&q);
     CommandClose(table);
return 1;
    }
