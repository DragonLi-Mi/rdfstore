#include <string>
#include <bitset>
#include "QueryGraph.h"
#include "SPARQLLexer.h"
#include "SPARQLParser.h"
#include "../operate/operate.h"
#include "bt_query.h"
#include "load.h"
#include "md5.h"

using namespace std;

static bool encodeFilter(const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::Filter& output);
static bool encode(const SPARQLParser::Element& element,unsigned& id,bool& constant);
static bool lookup(const std::string& text,::Type::Type_ID type,unsigned subType,unsigned& id);
static bool encodeBinaryFilter(QueryGraph::Filter::Type type,const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::Filter& output);
static bool encodeFilter(const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::SubQuery& output);
static bool encodeFilter(const SPARQLParser::PatternGroup& group,const SPARQLParser::Filter& input,QueryGraph::Filter& output);
static void encodeTableFunction(const SPARQLParser::PatternGroup& /*group*/,const SPARQLParser::Filter& input,QueryGraph::SubQuery& output);
static bool encode(const SPARQLParser::Element& element,unsigned& id,bool& constant);
static bool transformSubquery(const SPARQLParser::PatternGroup& group,QueryGraph::SubQuery& output);
static string readInput(istream& in);
static void runQuery(const string& query,bool explain);
void transform(const SPARQLParser& input, QueryGraph& output);
void run( const SPARQLParser& spinput,  QueryGraph& qginput);
bool isinlinenode(unsigned long id);
void decodeinline(unsigned long id,  vector<string>& v);




