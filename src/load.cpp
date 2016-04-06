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
    // exit(rdf_file);
    ifstream _fin(rdf_file.c_str());
    if(!_fin)
    {
    cerr<<"fair to open~~"<<rdf_file<<endl;
    exit(0);
    }
 //   CommandCreateNodefile(nodetable);
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
        ifstream nodes;
        nodes.open("nodes",ios::in);
     if(!nodes)
    {
        cout<<"nodetable not exist...cteate now"<<endl;
        CommandCreateNodefile(nodetable);
    }
      else
    {
        cout<<"nodetable found. insert soon"<<endl;
        nodes.close();
        CommandOpen(nodetable);
    }
     TripleWithObjType* triple_array =new TripleWithObjType[RDFParser::TRIPLE_NUM_PER_GROUP];
     RDFParser _parser(_fin);
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
        const char *sub=_sub.c_str();
           char *subhs =(char *)_subh.c_str();
         long long subid=add2Node(sub);
            cout<<"sub :"<<_sub<<"  sub id: "<<subid<<"   MD5:"<<subhs<<endl;
         CommandInsert(table, subhs ,strlen(subhs), &subid, sizeof(subid));

         //For predicate
          string _pre = triple_array[i].getPredicate();
          string _preh=MD5(_pre).toString();
          char *pre = (char *)_pre.c_str();
          const char *prehs = _preh.c_str();
          long long preid=add2Node(pre);
          // cout<<"pre id: "<<preid<<endl;
          cout<<"pre: "<<pre<<"  pre id: "<<preid<<"   MD5:"<<_preh<<endl;
          CommandInsert(table, const_cast<char*>(prehs) ,strlen(prehs), &preid, sizeof(preid));
       
         //For object
            string _obj = triple_array[i].getObject();
            string _objh=MD5(_obj).toString();
           const  char *obj = _obj.c_str();
           const  char *objhs = _objh.c_str();
         //   long long objid=add2Node(obj);
             //cout<<"obj id: "<<objid<<endl;
          //  CommandInsert(table, const_cast<char*>(objhs) ,strlen(objhs), &objid, sizeof(objid));
          //   cout<<"obj:"<<_obj<<" obj id: "<<"   MD5:"<<_objh<<endl;
        }

break;
    }
    CommandClose(table);
    CommandOpen(table);
   // int qu=1096;
   // int* qx;
    //qx=&qu;
   // bt_query_t q(bt_query_t::bt_eq, new int(1096),NULL);
    char* p =new char[32];
    char * a="478341015db373671537b490f1f32b88";
    strcpy(p,a);
    bt_query_t q(bt_query_t::bt_eq,p,NULL);


     CommandSelect(table,&q);
     CommandClose(table);
return 1;
    }

