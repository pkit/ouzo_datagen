/*
 * ProtobufReader.h
 *
 *  Created on: Jul 5, 2011
 *      Author: nhsan
 */

#ifndef PROTOBUFREADER_H_
#define PROTOBUFREADER_H_

#include "Reader.h"
#include <google/protobuf/descriptor.h>
using namespace google::protobuf;

class ProtobufReader: public Reader
{
public:
	ProtobufReader();
	virtual ~ProtobufReader();

	void open(string path);
	void close();
	Document* read();

private:
	FILE* f;
	int file_size;
};

#endif /* PROTOBUFREADER_H_ */
