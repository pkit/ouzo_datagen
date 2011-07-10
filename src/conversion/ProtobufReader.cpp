/*
 * ProtobufReader.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: nhsan
 */

#include "ProtobufReader.h"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define BUF_SIZE	(1024*16)

ProtobufReader::ProtobufReader()
{
}

ProtobufReader::~ProtobufReader()
{
	close();
}

void ProtobufReader::open(string path)
{
	f = fopen(path.c_str(), "rb");
	if (!f)
	{
		perror("Can not open file for reading\n");
		exit(1);
	}

	struct stat buf;
	fstat(fileno(f), &buf);
	file_size = buf.st_size;

}

void ProtobufReader::close()
{
	if (f) fclose(f);
}

Document* ProtobufReader::read()
{
	int len = 0;
	static char buf[BUF_SIZE];


	if (ftell(f) == file_size)
	{
		return NULL;
	}

	fread(&len, 4, 1, f);

	if (len > BUF_SIZE)
	{
		perror("Data is too large\n");
		exit(1);
	}
	fread(buf, len, 1, f);

	Document* doc = new Document();

	doc->ParseFromArray(buf,len);

	return doc;
}
