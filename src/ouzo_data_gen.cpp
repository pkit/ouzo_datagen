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

bool test_read_write(Document* doc)
{
	string s = DocumentsBuilder::toString(doc);
	DremelWriter* writer = new DremelWriter();

	writer->open();
	writer->write(doc);
	writer->close();

	//cout <<DocumentsBuilder::toString(doc)<<"\n";

	DremelReader* reader = new DremelReader();

	reader->open();
	Document* doc1 = reader->read();
	reader->close();
	//cout <<DocumentsBuilder::toString(doc1)<<"\n";

	if (DocumentsBuilder::compare(doc, doc1)) return true;
	else return false;
}

bool test_read_write_n_times(Document* doc, int n)
{
	DremelWriter* writer = new DremelWriter();

	writer->open();
	for (int i = 0; i < n; i++)
	{
		writer->write(doc);
	}

	writer->close();

	DremelReader* reader = new DremelReader();
	bool equal = true;
	reader->open();
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

void generate_data_set(int n)
{
	DocumentsBuilder builder;
	DremelWriter* writer = new DremelWriter();
	cout << "GENERATING...\n";

	writer->open();
	for (int i = 0; i < n; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		//cout<<doc->requests_size()<<"\n";
		delete doc;
	}
	writer->close();
	cout << "DONE\n";
}

int main()
{
	/*FOR TEST DATA GENERATION*/
//	DocumentsBuilder builder;
//	Document* doc = builder.buildDocument();
//	string s = DocumentsBuilder::toString(doc);
//	cout << s << "\n";
//
//	if (test_read_write(doc)) cout << "EQUAL\n";
//	else cout << "NOT EQUAL\n";
//
//	if (test_read_write_n_times(doc, 10)) cout << "EQUAL\n";
//	else cout << "NOT EQUAL\n";
//	cout << "DONE\n";

	/*FOR GENERATE DATA*/
	generate_data_set(1000000);
	return 0;
}
