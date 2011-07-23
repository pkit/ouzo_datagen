/*
 * SqliteWriter.h
 *
 *  Created on: Jun 16, 2011
 *      Author: nhsan
 */

#ifndef SQLITEWRITER_H_
#define SQLITEWRITER_H_
#include "Writer.h"
#include "sqlite/Database.h"
#include "sqlite/Query.h"

class SqliteWriter: public Writer
{
public:
	SqliteWriter();
	virtual ~SqliteWriter();

	void open(string path);
	void close();
	void write(Document* doc);

private:
	Database* db;
};

#endif /* SQLITEWRITER_H_ */
