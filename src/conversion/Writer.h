/*
 * Writer.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#ifndef WRITER_H_
#define WRITER_H_
#include <string>
#include "../schema/epaper.pb.h"
using namespace std;
class Writer
{
public:
	virtual void open(string path)=0;
	virtual void close()=0;
	virtual void write(Document* doc)=0;
};

#endif /* WRITER_H_ */
