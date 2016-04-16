#ifndef LOAD
#define LOAD
#include "Triple.h"
#include "TurtleParser.h"
#include "RDFParser.h"
#include "../operate/operate.h"
//#include "md5.h"
//#include "operate.h"

using namespace std;
//void encode_RDF(const string rdf_file);
int loadrdf(const string rdf_file,const string database);
int node_exist(  const char *table,char *node_md5);
// int exist(const string filename);
#endif
