/*
 * Writer.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#ifndef WRITER_H_
#define WRITER_H_
#include "../schema/epaper.pb.h"
class Writer
{
public:
	virtual void open()=0;
	virtual void close()=0;
	virtual void write(Document* doc)=0;
};

#endif /* WRITER_H_ */
