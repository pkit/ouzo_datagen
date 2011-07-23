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

int main(int argc, char* argv[])
{
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
