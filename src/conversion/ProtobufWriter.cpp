/*
 * ProtobufWriter.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: nhsan
 */

#include "ProtobufWriter.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE	(1024*16)

ProtobufWriter::ProtobufWriter()
{

}

ProtobufWriter::~ProtobufWriter()
{
	close();
}

void ProtobufWriter::open(string path)
{
	f = fopen(path.c_str(),"wb");
	if (!f)
	{
		perror("Can not open file for writing\n");
		exit(1);
	}
}

void ProtobufWriter::close()
{
	if (f) fclose(f);
}

void ProtobufWriter::write(Document* doc)
{
	static char* buf[BUF_SIZE];
	int len = doc->ByteSize();

	if (len>BUF_SIZE)
	{
		perror("Data is too large\n");
		exit(1);
	}

	doc->SerializePartialToArray(buf, len);
	fwrite(&len,4,1,f);
	fwrite(buf, len, 1, f);
}
