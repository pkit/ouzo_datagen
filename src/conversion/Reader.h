/*
 * Reader.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#ifndef READER_H_
#define READER_H_
#include "../schema/epaper.pb.h"

class Reader
{
public:
	virtual void open()=0;
	virtual void close()=0;
	virtual Document* read()=0;
};

#endif /* READER_H_ */
