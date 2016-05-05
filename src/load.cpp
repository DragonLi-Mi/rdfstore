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
      long queryid;
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
	   	cout<<"database not exist...create now"<<endl;
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
         char *sub=(char *)_sub.c_str();
      //  char subhs[33]={0};
   // strcpy(subhs,_subh.c_str());
    //  char* subhs=new char[33];
         char* insert=new char[100];

       strcpy(insert,_subh.c_str());
        // insert[32]='\0';
         // cout<<"test"<<insert<<endl;
        //insert=subhs;
       cout<<endl;   cout<<endl;   cout<<endl;


    //  char *subhs =(char *)_subh.c_str();

       //  cout<<"subh="<<insert<<" subhs:"<<sizeof(insert)<<"  "<<strlen(insert)<<endl;
        if (!node_exist(table,insert))
        {

         long long subid=add2Node(sub);
        
           cout<<"sub :"<<_sub<<"  sub id: "<<subid<<"   MD5:"<<insert<<endl;
            CommandInsert(table,insert,strlen(insert)+1, &subid, sizeof(subid));
        }
        else
            cout<<"node exist"<<endl;

      //  delete [] subhs;
        delete [] insert;


         //CommandInsert(table, subhs ,strlen(subhs), &subid, sizeof(subid));

         //For predicate
        //   string _pre = triple_array[i].getPredicate();
        //   string _preh=MD5(_pre).toString();
        //    char *pre = _pre.c_str();
        //   char *prehs = (char *)_preh.c_str();

        //     if (!node_exist(table,prehs))
        // {
        //   long long preid=add2Node(pre);   
        
        //   cout<<"pre: "<<pre<<"  pre id: "<<preid<<"   MD5:"<<_preh<<endl;
        //   CommandInsert(table, prehs ,sizeof(prehs), &preid, sizeof(preid));
        // }
        // else
        //     cout<<"node exist"<<endl;
        
          

         //For object
        //     string _obj = triple_array[i].getObject();
        //     char _objtype =triple_array[i].getObjectType();
        //     string _objh=MD5(_obj).toString();
        //      char *obj = _obj.c_str();
        //     char *objhs = _objh.c_str();
        //    if (!node_exist(table,prehs))
        // {
        //   long long objid=add2Node(obj);   
        //   CommandInsert(table, objhs ,sizeof(objhs), &objid, sizeof(objid));
        // }
        // else
        //     cout<<"node exist"<<endl;
          //  CommandInsert(table, const_cast<char*>(objhs) ,strlen(objhs), &objid, sizeof(objid));
          //   cout<<"obj:"<<_obj<<" obj id: "<<"   MD5:"<<_objh<<endl;
        }

break;
    }
    CommandClose(table);
    CommandOpen(table);

  //  // bt_query_t q(bt_query_t::bt_eq, new int(1096),NULL);
     char* p =new char[32];
     char * a="4e9fa1d4ad8dfbe8da7ad01f268d55b0";
     strcpy(p,a);

 vector<result_item> result;
    bt_query_t q(bt_query_t::bt_eq,p,NULL);
   //
     string as=Id2node(979);
     cout<<"ID to node: "<<as<<endl;

    CommandSelect(table,&q);
    //Select(table,&q, result);
    // cout<<"get!!!!!!!!!!!!!!!!!!!!!:        "<<result[0].key<<endl;
     CommandClose(table);
return 1;
    }
int node_exist(  const char *table,char *node_md5){
        vector<result_item> result;
        char* p =new char[33];
        strcpy(p,node_md5);
       
        bt_query_t q(bt_query_t::bt_eq,p,NULL);
     
        Select(table,&q, result);
  
    if (result.empty())
    {
        return 0;
    }
    else
    {
        return 1;
    }

}