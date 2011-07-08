/*
 * Reader.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#ifndef READER_H_
#define READER_H_
#include <string>
#include "../schema/epaper.pb.h"
using namespace std;

class Reader
{
public:
	virtual void open(string path)=0;
	virtual void close()=0;
	virtual Document* read()=0;
};

#endif /* READER_H_ */
