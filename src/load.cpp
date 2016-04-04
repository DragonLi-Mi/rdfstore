#include "load.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "bt_query.h"
#include "md5.h"
using namespace std;
static char nodetable[] = "nodes";
int loadrdf(const string rdf_file,const string database)
    {
const char *table = database.data();
    ifstream _fin(rdf_file.c_str());
    if(!_fin)
    {
    cerr<<"fair to open~~"<<rdf_file<<endl;
    exit(0);
    }
    CommandCreateNodefile(nodetable);
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
     int parse_triple_num = 0;
     _parser.parseFile(triple_array,parse_triple_num);
    if(parse_triple_num ==0)
    {break;}

    for(int i=0; i<parse_triple_num;i++)
        {
        // for subject
         string _sub = triple_array[i].getSubject();
         string _subh=MD5(_sub).toString();
      
         const  char *sub = _subh.c_str();
         long long subid=add2Node(sub);
         cout<<"sub id: "<<subid<<endl;
            cout<<"SSSSSSSSSS～～～～～～～～～"<<_sub<<"   MD5:"<<_subh<<endl;
         CommandInsert(table, const_cast<char*>(sub) ,strlen(sub), &subid, sizeof(subid));

         //For predicate
          string _pre = triple_array[i].getPredicate();
          string _preh=MD5(_pre).toString();
          const char *pre = _preh.c_str();
          long long preid=add2Node(pre);
           cout<<"pre id: "<<preid<<endl;
     //     cout<<"PPPPPPPPPP～～～～～～～～～"<<pre<<"   MD5:"<<_preh<<endl;
          CommandInsert(table, const_cast<char*>(pre) ,strlen(pre), &preid, sizeof(preid));
       
         //For object
            string _obj = triple_array[i].getObject();
            string _objh=MD5(_obj).toString();
           const  char *obj = _objh.c_str();
            long long objid=add2Node(obj);
             cout<<"obj id: "<<objid<<endl;
            CommandInsert(table, const_cast<char*>(obj) ,strlen(obj), &objid, sizeof(objid));
           //  cout<<"OOOOOOOOOOO～～～～～～～～～"<<obj<<"   MD5:"<<_objh<<endl;
        }

break;
    }
    CommandClose(table);
    CommandOpen(table);
   // int qu=1096;
   // int* qx;
    //qx=&qu;
    //bt_query_t q(bt_query_t::bt_eq, new int(1096),NULL);
 
    char ww[]="801ee50a2a70654ecf6273f9754bee70";
    bt_query_t q(bt_query_t::bt_eq,new char*(ww),NULL);

     CommandSelect(table,&q);
     CommandClose(table);
return 1;
    }
