#ifndef LOAD
#define LOAD
#include "Triple.h"
#include "TurtleParser.h"
#include "RDFParser.h"
#include "../operate/operate.h"
//#include "operate.h"

using namespace std;
//void encode_RDF(const string rdf_file);
int loadrdf(const string rdf_file,const string database);
#endif
