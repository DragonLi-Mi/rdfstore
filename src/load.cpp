#include "load.h"
#include <stdlib.h>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bt_query.h"
#include "md5.h"

using namespace std;
static char nodetable[] = "nodes";

int loadrdf(const string rdf_file, const string database) {
  const char* table = database.data();

  // exit(rdf_file);
  ifstream _fin(rdf_file.c_str());
  if (!_fin) {
    cerr << "fail to open~~" << rdf_file << endl;
    exit(0);
  }
  //   CommandCreateNodefile(nodetable);
  ifstream _file;
  _file.open(database.c_str(), ios::in);
  if (!_file) {
    cout << "database not exist...create now" << endl;
    CommandCreate(table);
    CommandCreate("SPO");
    CommandCreate("POS");
    CommandCreate("OSP");
  } else {
    cout << "database found. insert soon" << endl;
    _file.close();
    CommandOpen(table);
    CommandOpen("SPO");
    CommandOpen("POS");
    CommandOpen("OSP");
  }
  ifstream nodes;
  nodes.open("nodes", ios::in);
  if (!nodes) {
    cout << "nodefile not exist...cteate now" << endl;
    CommandCreateNodefile(nodetable);
  } else {
    cout << "nodefile found. insert soon" << endl;
    CommandOpen(nodetable);
  }
  TripleWithObjType* triple_array =
      new TripleWithObjType[RDFParser::TRIPLE_NUM_PER_GROUP];
  RDFParser _parser(_fin);
  while (true) {
    int parse_triple_num = 0;

    _parser.parseFile(triple_array, parse_triple_num);
    if (parse_triple_num == 0) {
      break;
    }

    for (int i = 0; i < parse_triple_num; i++) {
      // for subject

      string _sub = triple_array[i].getSubject();
      string _subh = MD5(_sub).toString();
      char* sub = (char*)_sub.c_str();
      char* inserts = new char[100];
      strcpy(inserts, _subh.c_str());
      unsigned long subid = node_exist(table, inserts);

      if (subid == 0) {
        subid = add2Node(sub);
        cout << "sub :" << _sub << "  sub id: " << subid << "   MD5:" << inserts
             << endl;
        CommandInsert(table, inserts, strlen(inserts) + 1, &subid,
                      sizeof(subid));
      } else {
        cout << "node exist! " << subid << endl;
      }
      delete[] inserts;

      //  For predicate
      string _pre = triple_array[i].getPredicate();
      string _preh = MD5(_pre).toString();
      char* pre = (char*)_pre.c_str();
      char* insertp = new char[100];
      strcpy(insertp, _preh.c_str());
      unsigned long preid = node_exist(table, insertp);

      if (preid == 0) {
        preid = add2Node(pre);

        cout << "pre: " << pre << "  pre id: " << preid << "   MD5:" << _preh
             << endl;
        CommandInsert(table, insertp, strlen(insertp) + 1, &preid,
                      sizeof(preid));

      } else
        cout << "node exist " << preid << endl;
      delete[] insertp;

      // For object
      string _obj = triple_array[i].getObject();
      char _objtype = triple_array[i].getObjectType();
      char* inserto = new char[100];
      unsigned long objid = 0;
      char* c_obj = (char*)_obj.c_str();
      datetime date;
      vector<string> vt;
      switch (_objtype) {
        case 'd':  // date
          _obj = _obj.substr(1, _obj.size() - 2);
          split(_obj, vt, "-");
          objid = inlinenode(_objtype, vt);
          cout << "objid:" << objid << endl;

          break;
        case 't':  // time
          cout << "Time node decode" << endl;
          // objid=inlinenode(c_obj,_objtype);
          break;
        case 'i':  // intger
          _obj = _obj.substr(1, _obj.size() - 2);

          vt.push_back(_obj);
          objid = inlinenode(_objtype, vt);

          break;
        case 'b':  // boolean
          cout << "boolean node decode" << endl;

          break;
        case 'u':  // double
          //   objid=inlinenode(c_obj,_objtype);
          cout << "double node decode" << endl;

          break;
           case 'l'://deciml
         objid=inlinenode(_objtype, vt);
             break;
        default:
          break;
      }
      if (objid == 0) {
        string _objh = MD5(_obj).toString();
        char* obj = (char*)_obj.c_str();
        strcpy(inserto, _objh.c_str());
        objid = node_exist(table, inserto);

        if (objid == 0) {
          objid = add2Node(obj);
          cout << "obj:" << _obj << " obj id: "<<objid
               << "   MD5:" << _objh << endl;

          CommandInsert(table, inserto, strlen(inserto) + 1, &objid,
                        sizeof(objid));

        } else
          cout << "node exist " << objid << endl;

        delete[] inserto;
      }

      InsertIndex(subid, preid, objid);
    }
    break;
  }

  CommandClose(table);
  CommandClose("SPO");
  CommandClose("POS");
  CommandClose("OSP");

  CommandOpen(table);
    CommandOpen("POS");


  //  // bt_query_t q(bt_query_t::bt_eq, new int(1096),NULL);
  char*   p= new char[32];
    char*   po= new char[64];

  char* a = "bfc739e9608b571700b38d74b7ece17c";
  char* b = "000000000000004e0000000000000080";
  strcpy(p, a);
  strcpy(po, b);


  vector<result_item> result,result2;
  bt_query_t q(bt_query_t::bt_eq, p, NULL);
    bt_query_t qo(bt_query_t::bt_eq, po, NULL);

  //
  string as = Id2node(979);
  cout << "ID to node: " << as << endl;

  // CommandSelect(table,&q);
  Select(table, &q, result);
    Select("POS", &qo, result2);

  cout << "key!!!!!!!!!!!!!!!!!!!!!:        " << result[0].key << endl;
  cout << "id!!!!!!!!!!!!!!!!!!!!!:        " << result[0].value << endl;
  cout << "2key!!!!!!!!!!!!!!!!!!!!!:        " << result2[0].key << endl;
  cout << "2id!!!!!!!!!!!!!!!!!!!!!:        " << result2[0].value << endl;








  CommandClose(table);
  return 1;
}

unsigned long node_exist(const char* table, char* node_md5) {
  vector<result_item> result;
  char* p = new char[33];

  strcpy(p, node_md5);

  bt_query_t q(bt_query_t::bt_eq, p, NULL);

  Select(table, &q, result);
  unsigned long id;
  if (result.empty()) {
    return 0;
  } else {
    id = result[0].value;
    // cout<<"find"<<id<<endl;

    return id;
  }
}

unsigned long inlinenode(char nodetype, vector<string> v) {
  // type:  0:date 1:integer 2:time 3:deciml

  bitset<64> result;
  result[63] = 1;
  datetime date;
  integer int_;
  string temp = "";

  switch (nodetype) {
    case 'd':  // date
      date.setyear(v[0]);
      date.setmonth(v[1]);
      date.setday(v[2]);
      cout << "year:" << date.b_year << endl;
      for (int i = 55, j = 0; i > 42; i--, j++) {
        temp = date.b_year[j];
        cout << temp;

        result[i] = atoi(temp.c_str());
      }
      cout << endl;
      for (int i = 42, j = 0; i > 38; i--, j++) {
        temp = date.b_month[j];
        result[i] = atoi(temp.c_str());
      }
      for (int i = 38, j = 0; i > 33; i--, j++) {
        temp = date.b_day[j];
        result[i] = atoi(temp.c_str());
      }

      return result.to_ulong();

      break;
    case 't':  // t`ime

      break;
    case 'i':  // intger
      int_.setint(v[0]);
      result[56] = 1;
      for (int i = 55, j = 0; i >= 0; i--, j++) {
        temp = int_.b_integer[j];
        result[i] = atoi(temp.c_str());
      }
      return result.to_ulong();

      break;
    case 'b':  // boolean
      break;
    case 'u':  // double
      break;
    case 'l':  // deciml



      break;
    default:
      break;
  }

  return 0;
}

int split(const string& str, vector<string>& ret_, string sep) {
  if (str.empty()) {
    return 0;
  }

  string tmp;
  string::size_type pos_begin = str.find_first_not_of(sep);
  string::size_type comma_pos = 0;

  while (pos_begin != string::npos) {
    comma_pos = str.find(sep, pos_begin);
    if (comma_pos != string::npos) {
      tmp = str.substr(pos_begin, comma_pos - pos_begin);
      pos_begin = comma_pos + sep.length();
    } else {
      tmp = str.substr(pos_begin);
      pos_begin = comma_pos;
    }

    if (!tmp.empty()) {
      ret_.push_back(tmp);
      tmp.clear();
    }
  }
  return 0;
}
void InsertIndex(unsigned long s, unsigned long p, unsigned long o) {
  unsigned long s_id, p_id, o_id;
  s_id = s;
  p_id = p;
  o_id = o;

  string hex_s, hex_p, hex_o, hex_sp, hex_po, hex_os;
  hex_s = int_to_hex(s);
  hex_p = int_to_hex(p);
  hex_o = int_to_hex(o);
  hex_sp = hex_s + hex_p;
  hex_po = hex_p + hex_o;
  hex_os = hex_o + hex_s;

  char* sp = new char[100];
  char* po = new char[100];
  char* os = new char[100];

  strcpy(sp, hex_sp.c_str());
  strcpy(po, hex_po.c_str());
  strcpy(os, hex_os.c_str());
  unsigned long sub = node_exist("SPO", sp);
  unsigned long pre = node_exist("POS", po);
  unsigned long obj = node_exist("OSP", os);
  cout <<" SPO" << " " << s_id<<" "<<p_id<<" "<<o_id << endl;
    cout <<"inserting POS" << " " << hex_s<<" "<<hex_p<<" "<<hex_o <<" key:"<<hex_po<< endl;



  if (sub == s) {
    cout << "SP exist and have a same O" << endl;
  } else if (sub != 0) {
    cout << "SP exist but there is  a different O" << endl;
    CommandInsert("SPO", sp, strlen(sp) + 1, &o_id, sizeof(o_id));
  } else {
    CommandInsert("SPO", sp, strlen(sp) + 1, &o_id, sizeof(o_id));
  }
  if (pre == p) {
    cout << "PO exist and have a same S" << endl;
  } else if (pre != 0) {
    cout << "PO exist but there is  a different S" << endl;
    CommandInsert("POS", po, strlen(po) + 1, &s_id, sizeof(s_id));
  } else {
    CommandInsert("POS", po, strlen(po) + 1, &s_id, sizeof(s_id));
  }
  if (obj == o) {
    cout << "OS exist and have a same P" << endl;
  } else if (obj != 0) {
    cout << "OS exist but there is  a different P" << endl;
    CommandInsert("OSP", os, strlen(os) + 1, &p_id, sizeof(p_id));
  } else {
    CommandInsert("OSP", os, strlen(os) + 1, &p_id, sizeof(p_id));
  }

  delete[] sp;
  delete[] po;
  delete[] os;
  cout << endl;
  cout << endl;
}

float GetFloat32( std::string Binary )
{
  int HexNumber = Binary2Hex( Binary );

  bool negative  = !!(HexNumber & 0x80000000);
  int  exponent  =   (HexNumber & 0x7f800000) >> 23;
  int sign = negative ? -1 : 1;

  // Subtract 127 from the exponent
  exponent -= 127;

  // Convert the mantissa into decimal using the
  // last 23 bits
  int power = -1;
  float total = 0.0;
  for ( int i = 0; i < 23; i++ )
  {
    int c = Binary[ i + 9 ] - '0';
    total += (float) c * (float) pow( 2.0, power );
    power--;
  }
  total += 1.0;

  float value = sign * (float) pow( 2.0, exponent ) * total;

  return value;
}

int Binary2Hex( std::string Binary )
{
  std::bitset<32> set(Binary);
  int hex = set.to_ulong();

  return hex;
}



// void  datetime::setyear(string year){
// int _year = atoi(year.c_str());
//     int temp=_year;

//     b_year="";

//     while(temp!=0)
//     {
//         b_year+=temp%2;
//         temp=temp/2;

//     }
// }
// void  datetime::setmonth(string month){
// int _month = atoi(month.c_str());
// int temp=_month;
// b_month="";

//     while(temp!=0)
//     {
//         b_month+=temp%2;
//         temp=temp/2;

//     }

// }
// void  datetime::setday(string day){
// int _day = atoi(day.c_str());
// int temp=_day;
// b_day="";

//     while(temp!=0)
//     {
//         b_day+=temp%2;
//         temp=temp/2;

//     }

// }
// void  datetime::setyhour(string hour){
// int _hour = atoi(hour.c_str());
// int temp=_hour;
// b_hour="";

//     while(temp!=0)
//     {
//         b_hour+=temp%2;
//         temp=temp/2;

//     }
// void  datetime::