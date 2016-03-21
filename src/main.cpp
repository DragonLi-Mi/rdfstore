#include <iostream>
#include <string>
#include <time.h>
#include "load.h"
using namespace std;
int main(int argc, char * argv[])
{
time_t timep;
time(&timep);
cout<<"load.."<<endl;
cout<<"argc: "<<argc<<"store: "<<argv[1]<<" RDF data: "<<argv[2]<<endl;
string dbpath = string(argv[1]);
string sourcepath = string (argv[2]);
int i=loadrdf(sourcepath,dbpath);
if(i==1)
cout<<"success!"<<endl;
else
cout<<"flase~"<<endl;
}
