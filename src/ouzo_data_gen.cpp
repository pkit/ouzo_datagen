//============================================================================
// Name        : ouzo_data_gen.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "schema/epaper.pb.h"
using namespace std;
#include "conversion/DremelWriter.h"
#include "conversion/DremelReader.h"
#include "builder/DocumentsBuilder.h"
#include "conversion/SqliteWriter.h"
#include "conversion/ProtobufReader.h"
#include "conversion/ProtobufWriter.h"

Document* build_simple_paper_doc()
{
	Document* doc = new Document();
	doc->set_docid(10);
	Document_Links* links = doc->mutable_links();
	links->add_forward(20);
	links->add_forward(40);
	links->add_forward(60);

	Document_Name* name = doc->add_name();
	Document_Name_Language* lang = name->add_language();
	lang->set_code("en-us");
	lang->set_country("us");
	lang = name->add_language();
	lang->set_code("en");
	name->set_url("http://A");
	name = doc->add_name();
	name->set_url("http://B");
	name = doc->add_name();
	lang = name->add_language();
	lang->set_code("en-gb");
	lang->set_country("gb");
	return doc;
}

bool test_read_write(Document* doc, string path)
{
	string s = DocumentsBuilder::toString(doc);
	DremelWriter* writer = new DremelWriter();

	writer->open(path);
	writer->write(doc);
	writer->close();

	//cout <<DocumentsBuilder::toString(doc)<<"\n";

	DremelReader* reader = new DremelReader();

	reader->open(path);
	Document* doc1 = reader->read();

	reader->close();
	//cout <<DocumentsBuilder::toString(doc1)<<"\n";

	if (DocumentsBuilder::compare(doc, doc1)) return true;
	else return false;
}

bool test_read_write_protobuf(Document* doc, string path)
{
	//string s = DocumentsBuilder::toString(doc);
	ProtobufWriter* writer = new ProtobufWriter();

	writer->open(path + "/document.pb");
	writer->write(doc);
	writer->close();

	//cout << DocumentsBuilder::toString(doc) << "\n";

	ProtobufReader* reader = new ProtobufReader();

	reader->open(path + "/document.pb");
	Document* doc1 = reader->read();
	reader->close();
	//cout << DocumentsBuilder::toString(doc1) << "\n";

	if (DocumentsBuilder::compare(doc, doc1)) return true;
	else return false;
}

bool test_read_write_n_times_protobuf(Document* doc, int n, string path)
{
	ProtobufWriter* writer = new ProtobufWriter();

	writer->open(path + "/document.pb");
	for (int i = 0; i < n; i++)
	{
		writer->write(doc);
	}

	writer->close();

	ProtobufReader* reader = new ProtobufReader();
	bool equal = true;
	reader->open(path + "/document.pb");
	Document* doc1 = reader->read();

	while (doc1 != NULL)
	{
		if (!DocumentsBuilder::compare(doc, doc1))
		{
			equal = false;
			break;
		}
		doc1 = reader->read();
	}

	reader->close();
	return equal;
}

bool test_read_write_n_times(Document* doc, int n, string path)
{
	DremelWriter* writer = new DremelWriter();

	writer->open(path);
	for (int i = 0; i < n; i++)
	{
		writer->write(doc);
	}

	writer->close();

	DremelReader* reader = new DremelReader();
	bool equal = true;
	reader->open(path);
	Document* doc1 = reader->read();
	while (doc1 != NULL)
	{
		if (!DocumentsBuilder::compare(doc, doc1))
		{
			equal = false;
			break;
		}
		doc1 = reader->read();
	}

	reader->close();
	return equal;
}

void test_sqlite(int n)
{
	DocumentsBuilder builder;
	SqliteWriter* writer = new SqliteWriter();
	cout << "GENERATING...\n";

	writer->open("../dremel");
	for (int i = 0; i < n; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		delete doc;
	}
	writer->close();
	cout << "DONE\n";
}

void generate_data_set(int n, string path)
{
	DocumentsBuilder builder;
	DremelWriter* writer = new DremelWriter();
	cout << "GENERATING...\n";

	writer->open(path);
	for (int i = 0; i < n; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		//cout<<DocumentsBuilder::toString(doc)<<"\n";
		//cout<<doc->requests_size()<<"\n";
		delete doc;
	}
	writer->close();
	cout << "DONE\n";
}

void generate_data_set2(int n, string path)
{
	DocumentsBuilder builder;
	DremelWriter* writer = new DremelWriter();
	SqliteWriter* writer2 = new SqliteWriter();
	cout << "GENERATING...\n";

	writer->open(path);
	writer2->open(path);
	for (int i = 0; i < n; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		writer2->write(doc);
		//cout<<doc->requests_size()<<"\n";
		delete doc;
	}
	writer->close();
	writer2->close();
	cout << "DONE\n";
}

void test_protobuf()
{
	char buffer[1024];
	clock_t s, e;
	s = clock();
	for (int i = 0; i < 1000000; i++)
	{
		Document* doc = build_simple_paper_doc();
		doc->SerializeToArray(buffer, 1024);
		delete doc;
	}
	e = clock();
	cout << "Time:" << (double) (e - s) / (CLOCKS_PER_SEC / 1000) << "\n";
}

bool dremel_protobuf_cross_test(string path, int n)
{
	DocumentsBuilder builder;
	DremelWriter* writer = new DremelWriter();
	ProtobufWriter* writer2 = new ProtobufWriter();

	writer->open(path);
	writer2->open(path + "/document.pb");
	for (int i = 0; i < n; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		writer2->write(doc);
		//cout<<DocumentsBuilder::toString(doc)<<"\n";
		//cout<<i<<"\n";
		delete doc;
	}
	writer->close();
	writer2->close();

	DremelReader* reader = new DremelReader();
	ProtobufReader* reader2 = new ProtobufReader();
	bool equal = true;

	reader->open(path);
	reader2->open(path + "/document.pb");
	Document* doc = reader->read();
	Document* doc2 = reader2->read();
	int c = 0;
	while ((doc != NULL) && (doc2 != NULL))
	{
		if (!DocumentsBuilder::compare(doc, doc2))
		{
			equal = false;
			//cout<<c;
			//cout<<DocumentsBuilder::toString(doc)<<"\n";
			//cout<<DocumentsBuilder::toString(doc2)<<"\n";
			break;
		}
		c++;
		delete doc;
		delete doc2;
		doc = reader->read();
		doc2 = reader2->read();
	}

	reader2->close();
	reader->close();
	return equal;
}

#define N_TIMES		1000
int main(int argc, char* argv[])
{
	//	/*NULL QUERY*/
	//	if (argc != 3)
	//	{
	//		cout << "Wrong arguments, please provide number of record for first argument, and destination path for second argument\n";
	//		return 0;
	//	}
	//	int count = atoi(argv[1]);
	//	string path = string(argv[2]);
	//
	//	DocumentsBuilder builder;
	//	Document* doc = builder.buildDocument();
	//	string s = DocumentsBuilder::toString(doc);
	//	//cout << s << "\n";
	//
	//	/*protobuf format*/
	//	cout << "Test read/write in protobuf format....";
	//	if (test_read_write_protobuf(doc, path)) cout << "EQUAL\n";
	//	else cout << "NOT EQUAL\n";
	//	cout << "Test read/write protobuf (n times)....";
	//	if (test_read_write_n_times_protobuf(doc, count, path)) cout << "EQUAL\n";
	//	else cout << "NOT EQUAL\n";
	//
	//	/*dremel format*/
	//	cout << "Test read/write in dremel format....";
	//	if (test_read_write(doc, path)) cout << "EQUAL\n";
	//	else cout << "NOT EQUAL\n";
	//	cout << "Test read/write in dremel format(n times)....";
	//	if (test_read_write_n_times(doc, count, path)) cout << "EQUAL\n";
	//	else cout << "NOT EQUAL\n";
	//
	//	cout << "Test cross read/write between dremel and protobuf format....";
	//
	//	if (dremel_protobuf_cross_test(path, count))
	//	{
	//		cout << "EQUAL\n";
	//	}
	//	else
	//	{
	//		cout << "NOT EQUAL\n";
	//	}

	/*FOR GENERATE DATA*/
	if (argc != 3)
	{
		cout << "Wrong arguments, please provide number of record for first argument, and destination path for second argument\n";
		return 0;
	}
	int count = atoi(argv[1]);
	string path = string(argv[2]);
	generate_data_set(count, path);

	/*FOR GENERATE DATA IN DREMEL AND SQLITE FORMAT*/
	//generate_data_set2(20,"../dremel");

	//test_protobuf();
	//generate_data_set(1000,"../dremel");

	return 0;
}
