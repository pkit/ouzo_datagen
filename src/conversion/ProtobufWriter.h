/*
 * ProtobufWriter.h
 *
 *  Created on: Jul 5, 2011
 *      Author: nhsan
 */

#ifndef PROTOBUFWRITER_H_
#define PROTOBUFWRITER_H_
#include "Writer.h"
#include <google/protobuf/descriptor.h>

class ProtobufWriter
{
public:
	ProtobufWriter();
	virtual ~ProtobufWriter();

	void open(string path);
	void close();
	void write(Document* doc);
private:
	FILE* f;
};

#endif /* PROTOBUFWRITER_H_ */
