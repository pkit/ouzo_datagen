/*
 * DremelReader.cpp
 *
 *  Created on: May 30, 2011
 *      Author: nhsan
 */

#include "DremelReader.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

DremelReader::DremelReader()
{
}

DremelReader::~DremelReader()
{
}

void DremelReader::open(string path)
{
	const Descriptor* desc = Document::descriptor();
	open_files_for(desc, path);
}

void DremelReader::close()
{
	const Descriptor* desc = Document::descriptor();
	close_files_for(desc);
}

Document* DremelReader::read()
{
	Document* doc = new Document();

	char rep, def;

	/* docid */
	if (!take(Document::kDocIdFieldNumber, rep, def)) return NULL;
	int64_t docid = read_long(Document::kDocIdFieldNumber);
	doc->set_docid(docid);

	/* links.backward */
	take(Document_Links::kBackwardFieldNumber, rep, def);
	if (def == 2) //not null
	{
		int64_t backward = read_long(Document_Links::kBackwardFieldNumber);
		Document_Links* links = doc->mutable_links();
		links->add_backward(backward);
	}

	if (head(Document_Links::kBackwardFieldNumber, rep, def))
	{
		while (rep == 1)
		{
			take(Document_Links::kBackwardFieldNumber, rep, def);
			if (def == 2) //not null
			{
				int64_t backward = read_long(Document_Links::kBackwardFieldNumber);
				Document_Links* links = doc->mutable_links();
				links->add_backward(backward);
			}
			if (!head(Document_Links::kBackwardFieldNumber, rep, def)) break;
		}
	}

	/* links.forward */
	take(Document_Links::kForwardFieldNumber, rep, def);

	if (def == 2) //not null
	{
		int64_t forward = read_long(Document_Links::kForwardFieldNumber);
		Document_Links* links = doc->mutable_links();
		links->add_forward(forward);
	}

	if (head(Document_Links::kForwardFieldNumber, rep, def))
	{
		while (rep == 1)
		{
			take(Document_Links::kForwardFieldNumber, rep, def);
			if (def == 2) //not null
			{
				int64_t forward = read_long(Document_Links::kForwardFieldNumber);
				Document_Links* links = doc->mutable_links();
				links->add_forward(forward);
			}
			if (!head(Document_Links::kForwardFieldNumber, rep, def)) break;
		}
	}

	int name_ind = 0;
	int language_ind = 0;
	bool finished = false;

	/* name.language.code */
	while (!finished)
	{

		take(Document_Name_Language::kCodeFieldNumber, rep, def);
		if (def == 2) //not null
		{
			string code = read_string(Document_Name_Language::kCodeFieldNumber);
			if (name_ind == doc->name_size()) doc->add_name();
			Document_Name* name = doc->mutable_name(name_ind);

			if (language_ind == name->language_size()) name->add_language();
			Document_Name_Language* language = name->mutable_language(language_ind);
			language->set_code(code);
		}

		take(Document_Name_Language::kCountryFieldNumber, rep, def);
		if (def == 3) //not null
		{
			string country = read_string(Document_Name_Language::kCountryFieldNumber);
			if (name_ind == doc->name_size()) doc->add_name();
			Document_Name* name = doc->mutable_name(name_ind);

			if (language_ind == name->language_size()) name->add_language();
			Document_Name_Language* language = name->mutable_language(language_ind);
			language->set_country(country);
		}

		bool has_more_country = head(Document_Name_Language::kCountryFieldNumber, rep, def);

		if (rep == 2 && has_more_country)
		{
			language_ind++;
		}
		else //0 and 1
		{
			take(Document_Name::kUrlFieldNumber, rep, def);
			if (def == 2) //not null
			{
				string url = read_string(Document_Name::kUrlFieldNumber);
				if (name_ind == doc->name_size()) doc->add_name();
				Document_Name* name = doc->mutable_name(name_ind);
				name->set_url(url);
			}

			if (!head(Document_Name::kUrlFieldNumber, rep, def))
			{
				finished = true;
			}
			else
			{
				if (rep == 0)
				{
					finished = true;
				}
				else
				{
					name_ind++;
					language_ind = 0;
				}
			}
		}
	}

	/*TODO: implement FSM for extended part*/
	/* sessionid */
	take(Document::kSessionIDFieldNumber, rep, def);
	int64_t sessionid = read_long(Document::kSessionIDFieldNumber);
	doc->set_sessionid(sessionid);

	take(Document::kUserIDFieldNumber, rep, def);
	int64_t userid = read_long(Document::kUserIDFieldNumber);
	doc->set_userid(userid);

	take(Document::kUserGroupFieldNumber, rep, def);
	int64_t usergroup = read_long(Document::kUserGroupFieldNumber);
	doc->set_usergroup(usergroup);

	take(Document::kClientIPFieldNumber, rep, def);
	int32_t clientip = read_int(Document::kClientIPFieldNumber);
	doc->set_clientip(clientip);

	take(Document::kCountryFieldNumber, rep, def);
	if (def == 1)//not null
	{
		string country = read_string(Document::kCountryFieldNumber);
		doc->set_country(country);
	}

	take(Document::kAgentFieldNumber, rep, def);
	if (def == 1)//not null
	{
		string agent = read_string(Document::kAgentFieldNumber);
		doc->set_agent(agent);
	}

	take(Document_Sales::kProductIdFieldNumber, rep, def);
	if (def == 1)
	{
		Document_Sales* sale = doc->mutable_sales();
		int32_t productid = read_int(Document_Sales::kProductIdFieldNumber);
		sale->set_productid(productid);
	}

	take(Document_Sales::kProductNameFieldNumber, rep, def);
	if (def == 1)
	{
		Document_Sales* sale = doc->mutable_sales();
		string productname = read_string(Document_Sales::kProductNameFieldNumber);
		sale->set_productname(productname);
	}

	take(Document_Sales::kSaleFieldNumber, rep, def);
	if (def == 1)
	{
		Document_Sales* sale = doc->mutable_sales();
		int32_t money = read_int(Document_Sales::kSaleFieldNumber);
		sale->set_sale(money);
	}

	finished = false;
	Document_Requests* request;
	while (!finished)
	{
		take(Document_Requests::kRequestIDFieldNumber, rep, def);
		if (def == 1)
		{
			request = doc->add_requests();
			int64_t requestid = read_long(Document_Requests::kRequestIDFieldNumber);
			request->set_requestid(requestid);
		}

		take(Document_Requests::kRequestTimeFieldNumber, rep, def);
		if (def == 1)
		{
			int64_t requesttime = read_long(Document_Requests::kRequestTimeFieldNumber);
			request->set_requesttime(requesttime);
		}

		take(Document_Requests::kResponseTimeFieldNumber, rep, def);
		if (def == 1)
		{
			int64_t responsetime = read_long(Document_Requests::kResponseTimeFieldNumber);
			request->set_responsetime(responsetime);
		}

		take(Document_Requests::kURLFieldNumber, rep, def);
		if (def == 1)
		{
			string url = read_string(Document_Requests::kURLFieldNumber);
			request->set_url(url);
		}

		take(Document_Requests::kRequestDataFieldNumber, rep, def);
		if (def == 1)
		{
			string requestdata = read_string(Document_Requests::kRequestDataFieldNumber);
			request->set_requestdata(requestdata);
		}

		if (!head(Document_Requests::kRequestDataFieldNumber, rep, def))
		{
			finished = true;
		}
		else
		{
			if (rep == 0) finished = true;
		}
	}

	return doc;
}

void DremelReader::open_files_for(const Descriptor* desc, string path)
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
			string name = path + "/"+field->full_name();

			string level_name = name + ".level";
			string data_name = name + ".dremel";
			levels[number] = fopen(level_name.c_str(), "rb");
			datas[number] = fopen(data_name.c_str(), "rb");

			struct stat buf;
			fstat(fileno(levels[number]), &buf);
			level_len[number] = buf.st_size;
		}
	}
}

bool DremelReader::take(int fno, char& rep, char& def)
{
	if (ftell(levels[fno]) == level_len[fno])
	{
		return false;
	}
	char tmp[2];
	fread(tmp, 2, 1, levels[fno]);
	rep = tmp[0];
	def = tmp[1];
	return true;
}

bool DremelReader::head(int fno, char& rep, char& def)
{
	if (ftell(levels[fno]) == level_len[fno])
	{
		rep = -1;
		def = -1;
		return false;
	}

	char tmp[2];
	fread(tmp, 2, 1, levels[fno]);
	rep = tmp[0];
	def = tmp[1];
	fseek(levels[fno], -2, SEEK_CUR);
	return true;
}

int32_t DremelReader::read_int(int fno)
{
	int32_t ret;
	fread(&ret, 4, 1, datas[fno]);
	return ret;
}

int64_t DremelReader::read_long(int fno)
{
	int64_t ret;
	fread(&ret, 8, 1, datas[fno]);
	return ret;
}

string DremelReader::read_string(int fno)
{
	int32_t len;
	fread(&len, 4, 1, datas[fno]);
	size_t hashcode;
	fread(&hashcode, sizeof(size_t), 1, datas[fno]);
	len++;
	int padding_count = len % 4;
	char* str = new char[len+padding_count];

	fread(str, len+padding_count, 1, datas[fno]);
	string ret(str, len-1);
	delete str;
	return ret;
}

void DremelReader::close_files_for(const Descriptor* desc)
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

			fclose(levels[number]);
			fclose(datas[number]);
		}
	}

}
