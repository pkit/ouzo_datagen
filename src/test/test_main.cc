/*
 * test_main.cc
 *
 *  Created on: Jul 18, 2011
 *      Author: nhsan
 */
#include <iostream>
#include "schema/epaper.pb.h"
using namespace std;
#include "conversion/DremelWriter.h"
#include "conversion/DremelReader.h"
#include "builder/DocumentsBuilder.h"
#include "conversion/ProtobufReader.h"
#include "conversion/ProtobufWriter.h"
#include "gtest/gtest.h"

#define N_TIMES		1000

TEST(dremel_read_write,one_time)
{
	string path="data";
	DocumentsBuilder builder;
	Document* doc = builder.buildDocument();

	DremelWriter* writer = new DremelWriter();

	writer->open(path);
	writer->write(doc);
	writer->close();

	DremelReader* reader = new DremelReader();

	reader->open(path);
	Document* doc1 = reader->read();
	reader->close();
	ASSERT_TRUE(DocumentsBuilder::compare(doc, doc1));
	delete doc;
	delete doc1;
}

TEST(protobuf_read_write, one_time)
{
	string path="data";
	DocumentsBuilder builder;
	Document* doc = builder.buildDocument();

	ASSERT_TRUE(doc!=NULL);

	ProtobufWriter* writer = new ProtobufWriter();

	writer->open(path + "/document.pb");
	writer->write(doc);
	writer->close();

	ProtobufReader* reader = new ProtobufReader();

	reader->open(path + "/document.pb");
	Document* doc1 = reader->read();
	reader->close();
	ASSERT_TRUE(doc1!=NULL);

	ASSERT_TRUE(DocumentsBuilder::compare(doc, doc1));
	delete doc;
	delete doc1;
}

TEST(protobuf_read_write,n_times)
{
	string path="data";
	DocumentsBuilder builder;
	Document* doc = builder.buildDocument();

	ProtobufWriter* writer = new ProtobufWriter();

	writer->open(path + "/document.pb");
	for (int i = 0; i < N_TIMES; i++)
	{
		writer->write(doc);
	}

	writer->close();

	ProtobufReader* reader = new ProtobufReader();
	reader->open(path + "/document.pb");
	Document* doc1 = reader->read();

	while (doc1 != NULL)
	{
		ASSERT_TRUE(DocumentsBuilder::compare(doc, doc1));
		delete doc1;
		doc1 = reader->read();
	}
	reader->close();
	delete doc;
}

TEST(dremel_read_write,n_times)
{
	string path="data";
	DocumentsBuilder builder;
	Document* doc = builder.buildDocument();

	DremelWriter* writer = new DremelWriter();

	writer->open(path);
	for (int i = 0; i < N_TIMES; i++)
	{
		writer->write(doc);
	}

	writer->close();

	DremelReader* reader = new DremelReader();
	reader->open(path);
	Document* doc1 = reader->read();
	while (doc1 != NULL)
	{
		ASSERT_TRUE(DocumentsBuilder::compare(doc, doc1));
		delete doc1;
		doc1 = reader->read();
	}

	reader->close();
	delete doc;
}

TEST(dremel_protobuf_cross_test,n_times)
{
	string path="data";
	DocumentsBuilder builder;
	DremelWriter* writer = new DremelWriter();
	ProtobufWriter* writer2 = new ProtobufWriter();

	writer->open(path);
	writer2->open(path + "/document.pb");
	for (int i = 0; i < N_TIMES; i++)
	{
		Document* doc = builder.buildDocument();
		writer->write(doc);
		writer2->write(doc);
		delete doc;
	}
	writer->close();
	writer2->close();

	DremelReader* reader = new DremelReader();
	ProtobufReader* reader2 = new ProtobufReader();

	reader->open(path);
	reader2->open(path + "/document.pb");
	Document* doc = reader->read();
	Document* doc2 = reader2->read();
	while ((doc != NULL) && (doc2 != NULL))
	{
		ASSERT_TRUE(DocumentsBuilder::compare(doc, doc2));
		delete doc;
		delete doc2;
		doc = reader->read();
		doc2 = reader2->read();
	}
	reader2->close();
	reader->close();
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

