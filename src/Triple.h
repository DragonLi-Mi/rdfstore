/*
 * Triple.h
 *
 */

#ifndef TRIPLE_H_
#define TRIPLE_H_
#include<iostream>
#include<string.h>
using namespace std;

class Triple{
public:
	string subject;
	string predicate;
	string object;

	Triple();

	/* _line responds to a triple
	 * It's OK if _line ends up with '.' or not
	 * tuples separated by '\t'
	 *  */
	Triple(string _line);
	Triple(const string& _s, const string& _p, const string& _o);
	Triple(const Triple& _triple);
	Triple& operator=(const Triple& _triple);

	void setSubject(const string& _s);
	void setPredicate(const string& _p);
	void setObject(const string& _o);

	const string& getSubject()const;
	const string& getPredicate()const;
	const string& getObject()const;

	const string toString()const;
};

class TripleWithObjType : public Triple
{
public:
    //char object_type;
    char object_type;

    static const char ENTITY = 'e';
    static const char LITERA = 'l';
    static const char DATE = 'd';
    static const char TIME = 't';
    static const char INTEGER = 'i';
    static const char _BOOLEAN = 'b';
    static const char DOUBLE = 'u';
    static const char DECIMAL = 'l';

    TripleWithObjType();
  //  TripleWithObjType(const string& _s, const string& _p, const string& _o, const char& _o_type = ' ');
    TripleWithObjType(const string& _s, const string& _p, const string& _o, const char& _o_type = ' ');

    TripleWithObjType(const TripleWithObjType& _triple_with_objtype);

    TripleWithObjType& operator=(const TripleWithObjType& _triple_with_objtype);

    void setObjType(const char &_o_type);
    const char getObjectType()const;

    bool isObjEntity()const;
    bool isObjLiteral()const;

    const string toString()const;
};

#endif /* TRIPLE_H_ */
