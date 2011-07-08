/*
 * DremelWriter.cpp
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#include "DremelWriter.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

DremelWriter::DremelWriter()
{

}

DremelWriter::~DremelWriter()
{

}

void DremelWriter::open(string path)
{
	if (access(path.c_str(), F_OK)!=0)
	{
		printf("Folder %s is not exist or no permission access\n", path.c_str());
		exit(1);
	}
	const Descriptor* desc = Document::descriptor();
	open_files_for(desc, path);
}

void DremelWriter::close()
{
	const Descriptor* desc = Document::descriptor();
	close_files_for(desc);
}

void DremelWriter::write(Document* doc)
{
	write_long(Document::kDocIdFieldNumber, doc->docid(), 0, 0);

	if (!doc->has_links()) //links is null
	{
		write_null(Document_Links::kBackwardFieldNumber, 0, 0);
		write_null(Document_Links::kForwardFieldNumber, 0, 0);
	}
	else
	{
		Document_Links* links = doc->mutable_links();
		if (links->backward_size() == 0)
		{
			write_null(Document_Links::kBackwardFieldNumber, 0, 1);
		}
		else
		{
			write_long(Document_Links::kBackwardFieldNumber, links->backward(0), 0, 2);
			for (int i = 1; i < links->backward_size(); i++)
			{
				write_long(Document_Links::kBackwardFieldNumber, links->backward(i), 1, 2);
			}
		}

		if (links->forward_size() == 0)
		{
			write_null(Document_Links::kForwardFieldNumber, 0, 1);
		}
		else
		{
			write_long(Document_Links::kForwardFieldNumber, links->forward(0), 0, 2);
			for (int i = 1; i < links->forward_size(); i++)
			{
				write_long(Document_Links::kForwardFieldNumber, links->forward(i), 1, 2);
			}
		}
	}

	int name_count = doc->name_size();
	if (name_count == 0)
	{
		write_null(Document_Name_Language::kCodeFieldNumber, 0, 0);
		write_null(Document_Name_Language::kCountryFieldNumber, 0, 0);
		write_null(Document_Name::kUrlFieldNumber, 0, 0);
	}
	else
	{
		char rep = 0;

		for (int i = 0; i < name_count; i++)
		{
			if (i > 0) rep = 1;

			Document_Name* name = doc->mutable_name(i);
			if (!name->has_url())
			{
				write_null(Document_Name::kUrlFieldNumber, rep, 1);
			}
			else
			{
				write_string(Document_Name::kUrlFieldNumber, name->url(), rep, 2);
			}

			if (name->language_size() == 0)
			{
				write_null(Document_Name_Language::kCodeFieldNumber, rep, 1);
				write_null(Document_Name_Language::kCountryFieldNumber, rep, 1);
			}
			else
			{
				for (int j = 0; j < name->language_size(); j++)
				{
					if (j > 0) rep = 2;
					Document_Name_Language* language = name->mutable_language(j);
					if (!language->has_country())
					{
						write_null(Document_Name_Language::kCountryFieldNumber, rep, 2);
					}
					else
					{
						write_string(Document_Name_Language::kCountryFieldNumber, language->country(), rep, 3);
					}

					write_string(Document_Name_Language::kCodeFieldNumber, language->code(), rep, 2);
				}
			}
		}
	}

	/* write data for extended part to files*/

	write_long(Document::kSessionIDFieldNumber, doc->sessionid(), 0, 0);
	write_long(Document::kUserIDFieldNumber, doc->userid(), 0, 0);
	write_long(Document::kUserGroupFieldNumber, doc->usergroup(), 0, 0);
	write_int(Document::kClientIPFieldNumber, doc->clientip(), 0, 0);

	if (!doc->has_country()) write_null(Document::kCountryFieldNumber, 0, 0); //BUG
	//write_null(Document::kCountryFieldNumber, 0, -1); //BUG
	else write_string(Document::kCountryFieldNumber, doc->country(), 0, 1);

	if (!doc->has_agent()) write_null(Document::kAgentFieldNumber, 0, 0); //BUG
	//write_null(Document::kAgentFieldNumber, 0, -1); //BUG
	else write_string(Document::kAgentFieldNumber, doc->agent(), 0, 1);

	if (!doc->has_sales())
	{
		write_null(Document_Sales::kProductIdFieldNumber, 0, 0);
		write_null(Document_Sales::kProductNameFieldNumber, 0, 0);
		write_null(Document_Sales::kSaleFieldNumber, 0, 0);
	}
	else
	{
		Document_Sales* sale = doc->mutable_sales();
		write_int(Document_Sales::kProductIdFieldNumber, sale->productid(), 0, 1);
		write_string(Document_Sales::kProductNameFieldNumber, sale->productname(), 0, 1);
		write_int(Document_Sales::kSaleFieldNumber, sale->sale(), 0, 1);
	}

	int request_count = doc->requests_size();

	if (request_count == 0)
	{
		write_null(Document_Requests::kRequestIDFieldNumber, 0, 0);
		write_null(Document_Requests::kRequestTimeFieldNumber, 0, 0);
		write_null(Document_Requests::kResponseTimeFieldNumber, 0, 0);
		write_null(Document_Requests::kURLFieldNumber, 0, 0);
		write_null(Document_Requests::kRequestDataFieldNumber, 0, 0);
	}
	else
	{
		char rep = 0;
		for (int i = 0; i < request_count; i++)
		{
			if (i > 0) rep = 1;
			Document_Requests* request = doc->mutable_requests(i);
			write_long(Document_Requests::kRequestIDFieldNumber, request->requestid(), rep, 1);
			write_long(Document_Requests::kRequestTimeFieldNumber, request->requesttime(), rep, 1);
			write_long(Document_Requests::kResponseTimeFieldNumber, request->responsetime(), rep, 1);
			write_string(Document_Requests::kURLFieldNumber, request->url(), rep, 1);
			write_string(Document_Requests::kRequestDataFieldNumber, request->requestdata(), rep, 1);
		}
	}
}

void DremelWriter::open_files_for(const Descriptor* desc, string path)
{
	int count = desc->field_count();

	for (int i = 0; i < count; i++)
	{
		const FieldDescriptor* field = desc->field(i);

		if (field->type() == FieldDescriptor::TYPE_GROUP)
		{
			const Descriptor* d = field->message_type();
			open_files_for(d, path);
		}
		else
		{
			int number = field->number();
			string name = path + "/" + field->full_name();
			string level_name = name + ".level";
			string data_name = name + ".dremel";
			levels[number] = fopen(level_name.c_str(), "wb");
			datas[number] = fopen(data_name.c_str(), "wb");
			if ((!levels[number]) || (!datas[number]))
			{
				perror("Can not open file for writing.\n");
				exit(1);
			}

		}

	}
}

void DremelWriter::close_files_for(const Descriptor* desc)
{
	int count = desc->field_count();

	for (int i = 0; i < count; i++)
	{
		const FieldDescriptor* field = desc->field(i);

		if (field->type() == FieldDescriptor::TYPE_GROUP)
		{
			const Descriptor* d = field->message_type();
			close_files_for(d);
		}
		else
		{
			int number = field->number();
			if (levels[number]) fclose(levels[number]);
			if (datas[number]) fclose(datas[number]);
		}
	}
}

void DremelWriter::write_null(int fno, char rep, char def)
{
	char level[2];
	level[0] = rep;
	level[1] = def;
	fwrite(level, 2, 1, levels[fno]);
	//cout << fno << "\t" << (int) rep << "\t" << (int) def << "\t" << "NULL" << "\n";
}

void DremelWriter::write_int(int fno, int32_t val, char rep, char def)
{
	char level[2];
	level[0] = rep;
	level[1] = def;
	fwrite(level, 2, 1, levels[fno]);
	fwrite(&val, 4, 1, datas[fno]);
}

void DremelWriter::write_long(int fno, int64_t val, char rep, char def)
{
	char level[2];
	level[0] = rep;
	level[1] = def;
	fwrite(level, 2, 1, levels[fno]);
	fwrite(&val, 8, 1, datas[fno]);

	//cout << fno << "\t" << (int) rep << "\t" << (int) def << "\t" << val << "\n";
}

static inline size_t hash_code(const char* s)
{
	size_t ret = *s;
	if (ret) for (++s; *s; ++s)
		ret = (ret << 5) - ret + *s;
	return ret;
}

void DremelWriter::write_string(int fno, char* str, int len, char rep, char def)
{
	char tmp[4];
	tmp[0] = rep;
	tmp[1] = def;
	fwrite(tmp, 2, 1, levels[fno]);
	fwrite(&len, 4, 1, datas[fno]);
	size_t hashcode = hash_code(str);
	fwrite(&hashcode, sizeof(size_t), 1, datas[fno]);

	if (len > 0)
	{
		fwrite(str, len, 1, datas[fno]);
	}

	len++;
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = 0;
	tmp[3] = 0;

	int padding_count = len % 4;
	padding_count++; //null terminal C string
	fwrite(tmp, padding_count, 1, datas[fno]); //one byte for c string end indicator, one for padding
	//cout << fno << "\t" << (int) rep << "\t" << (int) def << "\t" << str << "\n";
}

void DremelWriter::write_string(int fno, string str, char rep, char def)
{
	char tmp[4];
	tmp[0] = rep;
	tmp[1] = def;
	int len = str.length();
	fwrite(tmp, 2, 1, levels[fno]);
	fwrite(&len, 4, 1, datas[fno]);

	size_t hashcode = hash_code(str.c_str());
	fwrite(&hashcode, sizeof(size_t), 1, datas[fno]);

	if (len > 0)
	{
		fwrite(str.c_str(), len, 1, datas[fno]);
	}

	len++;
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = 0;
	tmp[3] = 0;

	int padding_count = len % 4;
	padding_count++; //null terminal C string
	fwrite(tmp, padding_count, 1, datas[fno]); //one byte for c string end indicator, one for padding

	//cout << fno << "\t" << (int) rep << "\t" << (int) def << "\t" << str << "\n";
}

