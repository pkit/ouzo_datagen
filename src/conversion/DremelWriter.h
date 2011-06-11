/*
 * DremelWriter.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

/*
 *	Write data in dremel format
 *
 */

#ifndef DREMELWRITER_H_
#define DREMELWRITER_H_

#include "Writer.h"
#include <google/protobuf/descriptor.h>
using namespace google::protobuf;

#define MAX_COL		26
class DremelWriter: public Writer
{
public:
	DremelWriter();
	virtual ~DremelWriter();

	void open();
	void close();
	void write(Document* doc);
private:
	FILE* levels[MAX_COL];
	FILE* datas[MAX_COL];

	void open_files_for(const Descriptor* desc);
	void close_files_for(const Descriptor* desc);

	void write_null(int fno, char rep, char def);
	void write_int(int fno, int32_t val, char rep, char def);
	void write_long(int fno, int64_t val, char rep, char def);
	void write_string(int fno, char* str,int len, char rep, char def);
	void write_string(int fno, string str, char rep, char def);
};

#endif /* DREMELWRITER_H_ */
