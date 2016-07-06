
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <set>
#include <cassert>
#include "Query.h"
using namespace std;
class SPARQLParser;
class QueryGraph;
class integer;
class  datetime;
class decimal;




static bool binds(const SPARQLParser::PatternGroup& group,unsigned id)
   // Check if a variable is bound in a pattern group
{
   for (std::vector<SPARQLParser::Pattern>::const_iterator iter=group.patterns.begin(),limit=group.patterns.end();iter!=limit;++iter)
      if ((((*iter).subject.type==SPARQLParser::Element::Variable)&&((*iter).subject.id==id))||
          (((*iter).predicate.type==SPARQLParser::Element::Variable)&&((*iter).predicate.id==id))||
          (((*iter).object.type==SPARQLParser::Element::Variable)&&((*iter).object.id==id)))
         return true;
   // for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter=group.optional.begin(),limit=group.optional.end();iter!=limit;++iter)
   //    if (binds(*iter,id))
   //       return true;
   // for (std::vector<std::vector<SPARQLParser::PatternGroup> >::const_iterator iter=group.unions.begin(),limit=group.unions.end();iter!=limit;++iter)
   //    for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter2=(*iter).begin(),limit2=(*iter).end();iter2!=limit2;++iter2)
   //       if (binds(*iter2,id))
   //          return true;
   // for (std::vector<std::vector<SPARQLParser::PatternGroup> >::const_iterator iter=group.gjoins.begin(),limit=group.gjoins.end();iter!=limit;++iter)
   //    for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter2=(*iter).begin(),limit2=(*iter).end();iter2!=limit2;++iter2)
   //       if (binds(*iter2,id))
   //          return true;

   return false;
}


static bool lookup(const std::string& text,::Type::Type_ID type,unsigned subType,unsigned& id)
   // Perform a dictionary lookup
{
   // if (dict.lookup(text,type,subType,id))
   //    return true;
   // if ((diffIndex)&&(diffIndex->lookup(text,type,subType,id)))
   //    return true;
   // return false;
   string s=text;
               
cout<<"lookup: "<<s<<endl;

      string md5 = MD5(s).toString();
      char* p = new char[100];
      strcpy(p, md5.c_str());

      unsigned long sig = node_exist("node2id", p);

  if (sig==0)
  {
     return false;
  }
  else{

      return true;
  }
}

static bool encodeBinaryFilter(QueryGraph::Filter::Type type,const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::Filter& output)
   // Encode a binary filter element
{
   output.type=type;
   output.arg1=new QueryGraph::Filter();
   if (!encodeFilter(group,*input.arg1,*output.arg1))
      return false;
   if (input.arg2) {
      output.arg2=new QueryGraph::Filter();
      if (!encodeFilter(group,*input.arg2,*output.arg2))
         return false;
   }
   return true;
}
static bool encodeFilter(const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::SubQuery& output)
   // Encode an element for the query graph
{
   // Handle and separately to be more flexible
   if (input.type==SPARQLParser::Filter::And) {
      if (!encodeFilter(group,*input.arg1,output))
         return false;
      if (!encodeFilter(group,*input.arg2,output))
         return false;
      return true;
   }

   // Encode recursively
   output.filters.push_back(QueryGraph::Filter());
   return encodeFilter(group,input,output.filters.back());
}

static bool encodeFilter(const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::Filter& output)
   // Encode an element for the query graph
{
   switch (input.type) {
      case SPARQLParser::Filter::Or: return encodeBinaryFilter(QueryGraph::Filter::Or,group,input,output);
      case SPARQLParser::Filter::And: return encodeBinaryFilter(QueryGraph::Filter::And,group,input,output);
      case SPARQLParser::Filter::Equal: return encodeBinaryFilter(QueryGraph::Filter::Equal,group,input,output);
      case SPARQLParser::Filter::NotEqual: return encodeBinaryFilter(QueryGraph::Filter::NotEqual,group,input,output);
      case SPARQLParser::Filter::Less: return encodeBinaryFilter(QueryGraph::Filter::Less,group,input,output);
      case SPARQLParser::Filter::LessOrEqual: return encodeBinaryFilter(QueryGraph::Filter::LessOrEqual,group,input,output);
      case SPARQLParser::Filter::Greater: return encodeBinaryFilter(QueryGraph::Filter::Greater,group,input,output);
      case SPARQLParser::Filter::GreaterOrEqual: return encodeBinaryFilter(QueryGraph::Filter::GreaterOrEqual,group,input,output);
      // case SPARQLParser::Filter::Plus: return encodeBinaryFilter(QueryGraph::Filter::Plus,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Minus: return encodeBinaryFilter(QueryGraph::Filter::Minus,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Mul: return encodeBinaryFilter(QueryGraph::Filter::Mul,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Div: return encodeBinaryFilter(QueryGraph::Filter::Div,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Not: return encodeUnaryFilter(QueryGraph::Filter::Not,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::UnaryPlus: return encodeUnaryFilter(QueryGraph::Filter::UnaryPlus,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::UnaryMinus: return encodeUnaryFilter(QueryGraph::Filter::UnaryMinus,dict,diffIndex,group,input,output);
      case SPARQLParser::Filter::Literal: {
         SPARQLParser::Element e;
         e.type=SPARQLParser::Element::Literal;
         e.subType=static_cast<SPARQLParser::Element::SubType>(input.valueArg);
         e.subTypeValue=input.valueType;
         e.value=input.value;
         unsigned id; bool constant;
         if (encode(e,id,constant)) {
            output.type=QueryGraph::Filter::Literal;
            output.id=id;
            output.value=input.value;
            output.valueType=input.valueType;
         } else {
            output.type=QueryGraph::Filter::Literal;
            output.id=~0u;
            output.value=input.value;
            output.valueType=input.valueType;


         }
         } return true;
      case SPARQLParser::Filter::Variable:
         if (binds(group,input.valueArg)) {
            output.type=QueryGraph::Filter::Variable;
            output.id=input.valueArg;
         } else {
            output.type=QueryGraph::Filter::Null;
         }
         return true;
         default:
         cout<<"undefined type"<<endl;
      // case SPARQLParser::Filter::IRI: {
      //    SPARQLParser::Element e;
      //    e.type=SPARQLParser::Element::IRI;
      //    e.subType=static_cast<SPARQLParser::Element::SubType>(input.valueArg);
      //    e.subTypeValue=input.valueType;
      //    e.value=input.value;
      //    unsigned id; bool constant;
      //    if (encode(dict,diffIndex,e,id,constant)) {
      //       output.type=QueryGraph::Filter::IRI;
      //       output.id=id;
      //       output.value=input.value;
      //    } else {
      //       output.type=QueryGraph::Filter::IRI;
      //       output.id=~0u;
      //       output.value=input.value;
      //    }
      //    } return true;
      // case SPARQLParser::Filter::Function:
      //    if (input.arg1->value==tableFunctionId)
      //       throw SemanticAnalysis::SemanticException(std::string("<")+tableFunctionId+"> calls must be placed in seperate filter clauses");
      //    return encodeBinaryFilter(QueryGraph::Filter::Function,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::ArgumentList: return encodeBinaryFilter(QueryGraph::Filter::ArgumentList,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_str: return encodeUnaryFilter(QueryGraph::Filter::Builtin_str,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_lang: return encodeUnaryFilter(QueryGraph::Filter::Builtin_lang,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_langmatches: return encodeBinaryFilter(QueryGraph::Filter::Builtin_langmatches,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_datatype: return encodeUnaryFilter(QueryGraph::Filter::Builtin_datatype,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_bound: return encodeUnaryFilter(QueryGraph::Filter::Builtin_bound,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_sameterm: return encodeBinaryFilter(QueryGraph::Filter::Builtin_sameterm,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_isiri: return encodeUnaryFilter(QueryGraph::Filter::Builtin_isiri,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_isblank: return encodeUnaryFilter(QueryGraph::Filter::Builtin_isblank,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_isliteral: return encodeUnaryFilter(QueryGraph::Filter::Builtin_isliteral,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_regex: return encodeTernaryFilter(QueryGraph::Filter::Builtin_regex,dict,diffIndex,group,input,output);
      // case SPARQLParser::Filter::Builtin_in: return encodeBinaryFilter(QueryGraph::Filter::Builtin_in,dict,diffIndex,group,input,output);
   }
   return false; // XXX cannot happen
}

static void encodeTableFunction(const SPARQLParser::PatternGroup& /*group*/,const SPARQLParser::Filter& input,QueryGraph::SubQuery& output)
   // Produce a table function call
{
   // Collect all arguments
   std::vector<SPARQLParser::Filter*> args;
   for (SPARQLParser::Filter* iter=input.arg2;iter;iter=iter->arg2)
      args.push_back(iter->arg1);

   // Check the call
   if ((args.size()<2)||(args[0]->type!=SPARQLParser::Filter::Literal)||(args[1]->type!=SPARQLParser::Filter::Literal))
    cout<<"malformed table function call"<<endl;
   unsigned inputArgs=std::atoi(args[1]->value.c_str());
   if ((inputArgs+2)>=args.size())
      cout<<"too few arguments to table function"<<endl;
   for (unsigned index=0;index<inputArgs;index++)
      if ((args[2+index]->type!=SPARQLParser::Filter::Literal)&&(args[2+index]->type!=SPARQLParser::Filter::IRI)&&(args[2+index]->type!=SPARQLParser::Filter::Variable))
         cout<<"table function arguments must be literals or variables"<<endl;
   for (unsigned index=2+inputArgs;index<args.size();index++)
      if (args[index]->type!=SPARQLParser::Filter::Variable)
         cout<<"table function output must consist of variables"<<endl;

   // Translate it
   output.tableFunctions.resize(output.tableFunctions.size()+1);
   QueryGraph::TableFunction& func=output.tableFunctions.back();
   func.name=args[0]->value;
   func.input.resize(inputArgs);
   func.output.resize(args.size()-2-inputArgs);
   for (unsigned index=0;index<inputArgs;index++) {
      if (args[index+2]->type==SPARQLParser::Filter::Variable) {
         func.input[index].id=args[index+2]->valueArg;
      } else {
         func.input[index].id=~0u;
         if (args[index+2]->type==SPARQLParser::Filter::IRI)
            func.input[index].value="<"+args[index+2]->value+">"; else
            func.input[index].value="\""+args[index+2]->value+"\"";
      }
   }
  for (unsigned index=2+inputArgs;index<args.size();index++)
      func.output[index-2-inputArgs]=args[index]->valueArg;
}
static bool encode(const SPARQLParser::Element& element,unsigned& id,bool& constant)
   // Encode an element for the query graph
{
   switch (element.type) {
      case SPARQLParser::Element::Variable:
         id=element.id;
         constant=false;
         return true;
      case SPARQLParser::Element::Literal:
          if (element.subType==SPARQLParser::Element::CustomType) {
            Type::Type_ID type; 
            unsigned subType=0;
            if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#string") {
               type=Type::Type_String;
            } else if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#integer") {
               type=Type::Type_Integer;
            } else if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#decimal") {
               type=Type::Type_Decimal;
            } else if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#double") {
               type=Type::Type_Double;
            } else if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#boolean") {
               type=Type::Type_Boolean;
            } else if (element.subTypeValue=="http://www.w3.org/2001/XMLSchema#date") {
               type=Type::Type_Date;
            } else {
               type=Type::Type_CustomType;
            }
            if (lookup(element.value,type,subType,id)) {
               constant=true;
               return true;
            } else 
            return false;
         } else {
            return false;
         }
      case SPARQLParser::Element::IRI:
         if (lookup(element.value,Type::Type_URI,0,id)) {

            constant=true;
            return true;
         } else 
         return false;
   }
   return false;
}

static bool transformSubquery(const SPARQLParser::PatternGroup& group,QueryGraph::SubQuery& output)
   // Transform a subquery
{  
   // Encode all patterns
   for (std::vector<SPARQLParser::Pattern>::const_iterator iter=group.patterns.begin(),limit=group.patterns.end();iter!=limit;++iter) {
      // Encode the entries
      QueryGraph::Node node;
      if ((!encode((*iter).subject,node.subject,node.constSubject))||
          (!encode((*iter).predicate,node.predicate,node.constPredicate))||
          (!encode((*iter).object,node.object,node.constObject))) {
         // A constant could not be resolved. This will produce an empty result
         return false;
      }
      output.nodes.push_back(node);
   }

   // Encode the filter conditions
   for (std::vector<SPARQLParser::Filter>::const_iterator iter=group.filters.begin(),limit=group.filters.end();iter!=limit;++iter) {
      if (!encodeFilter(group,*iter,output)) {
         // The filter variable is not bound. This will produce an empty result
         return false;
      }
                 

   }

   // // Encode all optional parts
   // for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter=group.optional.begin(),limit=group.optional.end();iter!=limit;++iter) {
   //    QueryGraph::SubQuery subQuery;
   //    if (!transformSubquery(dict,diffIndex,*iter,subQuery)) {
   //       // Known to produce an empty result, skip it
   //       continue;
   //    }
   //    output.optional.push_back(subQuery);
   // }

   // // Encode all union parts
   // for (std::vector<std::vector<SPARQLParser::PatternGroup> >::const_iterator iter=group.unions.begin(),limit=group.unions.end();iter!=limit;++iter) {
   //    std::vector<QueryGraph::SubQuery> unionParts;
   //    for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter2=(*iter).begin(),limit2=(*iter).end();iter2!=limit2;++iter2) {
   //       QueryGraph::SubQuery subQuery;
   //       if (!transformSubquery(dict,diffIndex,*iter2,subQuery)) {
   //          // Known to produce an empty result, skip it
   //          continue;
   //       }
   //       unionParts.push_back(subQuery);
   //    }
   //    // Empty union?
   //    if (unionParts.empty())
   //       return false;
   //    output.unions.push_back(unionParts);
   // }

   // // Encode all gjoin parts - Hancel & Giuseppe
   // for (std::vector<std::vector<SPARQLParser::PatternGroup> >::const_iterator iter=group.gjoins.begin(),limit=group.gjoins.end();iter!=limit;++iter) {
   //    std::vector<QueryGraph::SubQuery> gjoinParts;
   //    for (std::vector<SPARQLParser::PatternGroup>::const_iterator iter2=(*iter).begin(),limit2=(*iter).end();iter2!=limit2;++iter2) {
   //       QueryGraph::SubQuery subQuery;
   //       if (!transformSubquery(dict,diffIndex,*iter2,subQuery)) {
   //          // Known to produce an empty result, skip it
   //          continue;
   //       }
   //       gjoinParts.push_back(subQuery);
   //    }
   //    // Empty gjoin?
   //    if (gjoinParts.empty())
   //       return false;
   //    output.gjoins.push_back(gjoinParts);
   // }

   return true;
}

void transform(const SPARQLParser& input, QueryGraph& output)
// Perform the transformation
{
  // CommandOpen(nodetable);
 // CommandOpen("node2id");
  output.clear();

  if (!transformSubquery(input.getPatterns(), output.getQuery())) {
    //    // A constant could not be resolved. This will produce an empty result
    output.markAsKnownEmpty();
    return;
  }

  // Compute the edges
  output.constructEdges();

  // Add the projection entry
  for (SPARQLParser::projection_iterator iter = input.projectionBegin(),
                                         limit = input.projectionEnd();
       iter != limit; ++iter)
    output.addProjection(*iter);

  // Set the duplicate handling
  switch (input.getProjectionModifier()) {
    case SPARQLParser::Modifier_None:
      output.setDuplicateHandling(QueryGraph::AllDuplicates);
      break;
    case SPARQLParser::Modifier_Distinct:
      output.setDuplicateHandling(QueryGraph::NoDuplicates);
      break;
    case SPARQLParser::Modifier_Reduced:
      output.setDuplicateHandling(QueryGraph::ReducedDuplicates);
      break;
    case SPARQLParser::Modifier_Count:
      output.setDuplicateHandling(QueryGraph::CountDuplicates);
      break;
    case SPARQLParser::Modifier_Duplicates:
      output.setDuplicateHandling(QueryGraph::ShowDuplicates);
      break;
  }

  // Order by clause
  for (SPARQLParser::order_iterator iter = input.orderBegin(),
                                    limit = input.orderEnd();
       iter != limit; ++iter) {
    QueryGraph::Order o;
    if (~(*iter).id) {
      if (!binds(input.getPatterns(), (*iter).id)) continue;
      o.id = (*iter).id;
    } else {
      o.id = ~0u;
    }
    o.descending = (*iter).descending;
    output.addOrder(o);
  }
  SPARQLParser::PatternGroup group;
  // SPARQLParser::Pattern pat(Element subject,Element predicate,Element
  // object);
  group = input.getPatterns();
  for (std::vector<SPARQLParser::Pattern>::const_iterator
           iter = group.patterns.begin(),
           limit = group.patterns.end();
       iter != limit; ++iter)
    cout << "  subject  " << (*iter).subject.id
         << " type:" << (*iter).subject.type << endl;
  for (std::vector<SPARQLParser::Pattern>::const_iterator
           iter = group.patterns.begin(),
           limit = group.patterns.end();
       iter != limit; ++iter)
    cout << "  predicate  " << (*iter).predicate.value
         << " type:" << (*iter).predicate.type <<" id:"<<(*iter).predicate.id<<endl<< "  object  "
         << (*iter).object.value << " sbt" << (*iter).object.subTypeValue << " ."
         << (*iter).object.id << " type:" << (*iter).object.type
         << " object type:" << (*iter).object.type<<" subTypeValue:"<< (*iter).object.subTypeValue<< endl;

  for (std::vector<SPARQLParser::Filter>::const_iterator
           iter = group.filters.begin(),
           limit = group.filters.end();
       iter != limit; ++iter)
    cout << "  filters type  " << (*iter).type << " | " << (*iter).arg1->value
         << " type " << (*iter).arg1->type << " " << (*iter).arg2->value << " valueArg:"<<(*iter).arg2->valueArg<<" valueType:"<<(*iter).arg2->valueType
         << (*iter).arg2->type << endl;
  for (SPARQLParser::projection_iterator iter = input.projectionBegin(),
                                         limit = input.projectionEnd();
       iter != limit; ++iter)
    cout << "projection " << (*iter) << endl;
  output.setLimit(input.getLimit());
  QueryGraph::SubQuery Query;

  Query = output.getQuery();
  for (std::vector<QueryGraph::Node>::const_iterator
           iter = Query.nodes.begin(),
           limit = Query.nodes.end();
       iter != limit; ++iter){
    cout << "  subject  " << (*iter).subject
         << " const:" << (*iter).constSubject << endl;

    cout << "  object  " << (*iter).object
         << " const:" << (*iter).constObject << endl;}


 for (std::vector<QueryGraph::Filter>::const_iterator
           iter = Query.filters.begin(),
           limit = Query.filters.end();
       iter != limit; ++iter)
    cout << "  filters  " << (*iter).type <<" v "<<(*iter).value
         << " arg1:" << (*iter).arg1->id<<" type "<< (*iter).arg1->type<< " arg2:" << (*iter).arg2->value<<" type "<< (*iter).arg2->type<< " valueType "<< (*iter).arg2->valueType<< endl;









  //   std::vector<SPARQLParser::Filter>::const_iterator filter_iter=group.filters.begin();
  //  (*filter_iter).type=input.

  //   if (group.filters.size()==0)
  //   {
  //      return 0;
  //   }
  //   else{

  //        switch(group.filters.type) 
  //        case Or:
  //        break;
  //        case Equal:
  //        break;
  //        case NotEqual:
  //        break;
  //        case Less:
  //        break;
  //        case LessOrEqual:
  //        break;
  //        case Greater:
  //        break;
  //        case GreaterOrEqual:
  //        break;

  //   }




}

bool isinlinenode(unsigned long id) {
  bitset<64> result(id);
  if (result[63] == 1) {
    return true;
  } else
    return false;
}


void decodeinline(unsigned long id,  vector<string>& v){

  bitset<64> b(id);
  string result="";
  string str_id=b.to_string();
  bitset<7> type(str_id,1,7);
  unsigned   long t=type.to_ulong();//0:date 1:integer 2:time 3:deciml
if (t==0) {
  bitset<13> y(str_id,8,13);
  bitset<4> m(str_id,21,4);
  bitset<5> d(str_id,25,5);
  result=to_string(y.to_ulong())+"-"+to_string(m.to_ulong())+"-"+to_string(d.to_ulong());
}
else if(t==1){

    bitset<56> i(str_id,8,56);
    result=to_string(i.to_ulong());

}
else if(t==2){

}else if(t==3){
   bitset<32> sca(str_id,8,32);
   string temp=sca.to_string();
float f=GetFloat32(temp);

}
else{
  cout<<"undefined type"<<endl;
  exit(0);

}

v.push_back(result);


}


//---------------------------------------------------------------------------
 void run( const SPARQLParser& spinput,  QueryGraph& qginput){
//void run( const SPARQLParser::PatternGroup& group,QueryGraph::SubQuery& graph){

  SPARQLParser::PatternGroup  group;
  group = spinput.getPatterns();
  QueryGraph::SubQuery  graph;
  graph = qginput.getQuery();

  string str_re="";




  std::vector<SPARQLParser::Pattern>::const_iterator pattern_iter=group.patterns.begin();
  
  string var="0000000000000000";
    unsigned long s_id,p_id,o_id;
    string zero="0",hex_oid;

    QueryGraph::Filter filter;

    string s,p,o;
    char* squery =new char[100];

    if ((*pattern_iter).subject.type==0)
    {
         s=var;
    }
    else {

         s=(*pattern_iter).subject.value;
       string md5 = MD5(s).toString();
      strcpy(squery, md5.c_str());
      unsigned long sig = node_exist("node2id", squery);

         if (sig==0)
         {
            qginput.markAsKnownEmpty();
            cout<<"s node is not exist"<<endl;
         }
         else{
            s_id=sig;

    }
 }
    //delete [] squery;

    char* pquery =new char[100];

     if ((*pattern_iter).predicate.type==0)
    {
         p=var;
    }
    else {
         p=(*pattern_iter).predicate.value;
          string md5 = MD5(p).toString();
      strcpy(pquery, md5.c_str());
      unsigned long sig = node_exist("node2id", pquery);
         if (sig==0)
         {
            qginput.markAsKnownEmpty();
            cout<<" node not exist"<<endl;
         }
         else{
         p_id=sig;
      }
    }
//delete [] pquery;
    char* oquery =new char[100];


     if ((*pattern_iter).object.type==0)
    {

         o=var;
    }else if ((*pattern_iter).object.subTypeValue=="http://www.w3.org/2001/XMLSchema#integer")
    {
       bitset<64> r;
       r[63] = 1;
       r[56] = 1;
       string temp = "";



       integer incode_int;
       o=(*pattern_iter).object.value;
       incode_int.setint(o);
       for (int i = 55, j = 0; i >= 0; i--, j++) {
       temp = incode_int.b_integer[j];
       r[i] = atoi(temp.c_str());
     }
       o_id= r.to_ulong();
       str_re= r.to_string();
       //o_id=atoi(integer.c_str())

       hex_oid=int_to_hex(incode_int.b_integer);

    }
      else if ((*pattern_iter).object.subTypeValue=="http://www.w3.org/2001/XMLSchema#decimal")
    {
      decimal dec;
       bitset<64> r;
       r[63] = 1;
       o=(*pattern_iter).object.value;
      string temp = "";

     dec.set(o);
     r[56] = 1;
     r[57] = 1;
     for (int i = 55, j = 0; i >= 0; i--, j++) {
       temp = dec.b_decimal[j];
       r[i] = atoi(temp.c_str());
     }
     o_id= r.to_ulong();
     str_re= r.to_string();
     cout<<"incode result:"<<r.to_ulong()<<"  "<<str_re<<endl;    }
    else if ((*pattern_iter).object.subTypeValue=="http://www.w3.org/2001/XMLSchema#date")
    {
      bitset<64> r;
      r[63] = 1;
      string temp = "";
      datetime date;

      o=(*pattern_iter).object.value;
      vector<string> vt;
      split(o, vt, "-");

      date.setyear(vt[0]);
      date.setmonth(vt[1]);
      date.setday(vt[2]);
      cout << "year:" << date.b_year << endl;
      cout << "month:" << date.b_month << endl;
      cout << "day:" << date.b_day << endl;

     for (int i = 55, j = 0; i > 42; i--, j++) {
       temp = date.b_year[j];
       cout << temp;

       r[i] = atoi(temp.c_str());
     }

     for (int i = 42, j = 0; i > 38; i--, j++) {
       temp = date.b_month[j];
       r[i] = atoi(temp.c_str());
     }
     for (int i = 38, j = 0; i > 33; i--, j++) {
       temp = date.b_day[j];
       r[i] = atoi(temp.c_str());
     }
     o_id=r.to_ulong();
     str_re= r.to_string();    
  }
    else if ((*pattern_iter).object.subTypeValue=="http://www.w3.org/2001/XMLSchema#time")
    {
       /* code */
    }
      else{

         o=(*pattern_iter).object.value;
         string md5 = MD5(o).toString();
      strcpy(oquery, md5.c_str());
      unsigned long sig = node_exist("node2id", oquery);

          if (sig==0)
         {
         //   qginput.markAsKnownEmpty();
            cout<<" node not exist"<<endl;
         }
         else{
         o_id=sig;
      }
    }
//delete  [] oquery;
cout<<"s type "<<(*pattern_iter).subject.type<<" p type:"<<(*pattern_iter).predicate.type<<" o type:"<<(*pattern_iter).object.type<<endl;
string hex_s, hex_p, hex_o, hex_sp, hex_po, hex_os;

if ((*pattern_iter).subject.type==0){

   hex_p = int_to_hex(p_id);
   hex_o = int_to_hex(o_id);
   hex_po = hex_p + hex_o;
   cout<<"hPid:"<<hex_p<<" hOid:"<<hex_o<<" key:"<<hex_po<<endl;

   char* _key= new char[100];
    strcpy(_key,hex_po.c_str());
         vector<result_item> result;
          bt_query_t q(bt_query_t::bt_eq, _key, NULL);
          Select("POS",&q, result);
          cout<<"result:"<<endl;
         for (std::vector<result_item>::const_iterator
           iter = result.begin(),
           limit = result.end();
       iter != limit; ++iter)
            {
            unsigned long sid = (*iter).value;


            string s_r = Id2node(sid);

               cout<<s_r<<endl;

            }



}else if ((*pattern_iter).predicate.type==0)
{
   
   hex_s = int_to_hex(s_id);
   hex_o = int_to_hex(o_id);
   hex_os = hex_o + hex_s;

   char* _key= new char[100];
    strcpy(_key,hex_os.c_str());
         vector<result_item> result;
          bt_query_t q(bt_query_t::bt_eq, _key, NULL);
          Select("OSP",&q, result);
          cout<<"result:"<<endl;
         for (std::vector<result_item>::const_iterator
           iter = result.begin(),
           limit = result.end();
       iter != limit; ++iter)
            {
            unsigned long pid = (*iter).value;


            string s_r = Id2node(pid);

               cout<<s_r<<endl;

            }
}
else{
   hex_s = int_to_hex(s_id);
   hex_p = int_to_hex(p_id);
   hex_sp = hex_s + hex_p;

   char* _key= new char[100];
    strcpy(_key,hex_sp.c_str());
         vector<result_item> result;
                  vector<string> r;

          bt_query_t q(bt_query_t::bt_eq, _key, NULL);
          Select("SPO",&q, result);
          cout<<"result:"<<endl;
         for (std::vector<result_item>::const_iterator
           iter = result.begin(),
           limit = result.end();
       iter != limit; ++iter)
            {
            unsigned long oid = (*iter).value;
            cout<<"iod:"<<oid<<endl;
            string s_r;
             if (isinlinenode(oid)) {

                     decodeinline(oid,r);
                     s_r=r[0];
             }else{

                 s_r = Id2node(oid);
                  }
                   cout<<s_r<<endl;
            }
}
}



static string readInput(istream& in)
   // Read a stream into a string
{
   string result;
   while (true) {
      string s;
      getline(in,s);
      result+=s;
      if (!in.good())
         break;
      result+='\n';
   }
   return result;
}

static void runQuery(const string& query,bool explain)
   // Evaluate a query
{

     const char nodetable[] = "nodes";
     CommandOpen(nodetable);
     CommandOpen("node2id");
          CommandOpen("SPO");

     CommandOpen("POS");

     CommandOpen("OSP");


   QueryGraph queryGraph;
   {
      // Parse the query
      SPARQLLexer lexer(query);
      SPARQLParser parser(lexer);
      try {
         parser.parse();
      } catch (const SPARQLParser::ParserException& e) {
         cerr << "parse error: " << e.message << endl;
                  return;
      }

  
         transform(parser,queryGraph);
    
      if (queryGraph.knownEmpty()) {
         if (explain)
            cerr << "static analysis determined that the query result will be empty" << endl; else
            cout << "<empty result>" << endl;
         return;
      }
      run(parser,queryGraph);
   }



}


int main(int argc,char* argv[])
{


   // Check the arguments
//    if ((argc!=1)&&(argc!=2)) {
// cout<<"arguments num wrong"<<endl;
//       return 1;
//    }

   // Open the database
  //  Database db;
  //  if (!db.open(argv[1],true)) {
  //     cerr << "unable to open database " << argv[1] << endl;
  //     return 1;
  //  }

   //Execute a single query?
    cout<<"arguments "<<argv[0]<<" "<<argv[1]<<endl;


      ifstream in(argv[1]);
      if (!in.is_open()) {
         cerr << "unable to open " << argv[1] << endl;
         return 1;
      }
      string query=readInput(in);

         runQuery(query,false);

         cout.flush();
   return 0;
}
