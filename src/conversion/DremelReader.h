/*
 * DremelReader.h
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#ifndef DREMELREADER_H_
#define DREMELREADER_H_
#include "Reader.h"
#include <google/protobuf/descriptor.h>
using namespace google::protobuf;

#define MAX_COL		26

typedef struct
{
	char rep;
	char def;
} level;

class DremelReader: public Reader
{
public:
	DremelReader();
	virtual ~DremelReader();
	void open();
	void close();
	Document* read();

private:
	FILE* levels[MAX_COL];
	FILE* datas[MAX_COL];
	int level_len[MAX_COL];

	void open_files_for(const Descriptor* desc);
	void close_files_for(const Descriptor* desc);

	bool take(int fno, char& rep, char& def);
	bool head(int fno, char& rep, char& def);
	int32_t read_int(int fno);
	int64_t read_long(int fno);
	string read_string(int fno);
};

#endif /* DREMELREADER_H_ */
