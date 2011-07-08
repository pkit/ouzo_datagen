/*
 * DocumentsBuilder.cpp
 *
 *  Created on: May 26, 2011
 *      Author: nhsan
 */

#include "DocumentsBuilder.h"
#include "rand_utils.h"
#include <sstream>
#include <iostream>
#include <stdio.h>

DocumentsBuilder::DocumentsBuilder()
{
	initrand();
	for (int i = 0; i < AGENT_COUNT; i++)
	{
		int len = randint(100);
		char* agent = (char*) malloc(len);
		agents[i] = randstr(agent, len);
	}

	for (int i = 0; i < PRODUCT_COUNT; i++)
	{
		int len = randint(20, 200);
		char* name = (char*) malloc(len);
		product_names[i] = randstr(name, len);
	}
	static char* domains[8] = { "com", "net", "org", "us", "biz", "vn", "info", "edu" };

	for (int i = 0; i < URL_COUNT; i++)
	{
		if (rand() < (RAND_MAX / 10000)) //0.01%
		{
			urls[i] = "http://www.ouzo.com/abcdef";
			//printf(urls[i]);
		}
		else
		{
			int len = randint(4, 20);
			char* name = (char*) malloc(len);

			randstr(name, len, RAND_STR_LOWER_CASE_LETTERS);
			int dom = randint(8);
			char* url = (char*) malloc(len + strlen(domains[dom]) + strlen("http://") + 1);
			sprintf(url, "http://%s.%s", name, domains[dom]);
			free(name);
			urls[i] = url;
		}
		//printf("%s\n", url);
	}

	for (int i = 0; i < VARIABLE_COUNT; i++)
	{
		int len = randint(2, 8);
		char* name = (char*) malloc(len);

		randstr(name, len, RAND_STR_LOWER_CASE_LETTERS);
		variables[i] = name;
		//printf("%s\n", url);
	}

	for (int i = 0; i < 100000; i++)
	{
		int len = randint(10, 25);
		char* name = (char*) malloc(len);

		randstr(name, len, RAND_STR_LOWER_CASE_LETTERS);
		new_country[i] = name;
		//printf("%s\n", url);
	}

}

DocumentsBuilder::~DocumentsBuilder()
{
	for (int i = 0; i < AGENT_COUNT; i++)
	{
		free(agents[i]);
	}

	for (int i = 0; i < PRODUCT_COUNT; i++)
	{
		free(product_names[i]);
	}
}

void DocumentsBuilder::fillSale(Document_Sales* sale)
{
	int producid = randint(PRODUCT_COUNT); ////TODO: change to normal distribution
	sale->set_productid(producid);
	sale->set_productname(product_names[producid]);
	sale->set_sale(randint(SALE_COUNT)); //TODO: change to normal distribution
}

Document* DocumentsBuilder::buildDocument()
{
	Document* doc = new Document();
	fillDocument(doc);
	return doc;
}

void DocumentsBuilder::fillRequest(Document_Requests* request)
{
	static int REQUEST_ID = 1;
	int request_inc = randint(2, 5);
	REQUEST_ID += request_inc;
	request->set_requestid(REQUEST_ID);

	struct tm tm1;//TODO: move to class
	struct tm tm2;
	strptime("2010-1-1 00:00:00", "%Y-%m-%d %H:%M:%S", &tm1);
	strptime("2011-1-1 00:00:00", "%Y-%m-%d %H:%M:%S", &tm2);

	time_t t1 = mktime(&tm1);
	time_t t2 = mktime(&tm2);
	int t_distance = t2 - t1;

	int t = randint(t_distance);

	request->set_requesttime(t1 + t);
	int resp = randint(2, 20);
	request->set_responsetime(t1 + t + resp);

	int url_id = randint(URL_COUNT);
	request->set_url(urls[url_id]);

	stringstream ss;

	int var_count = randint(5, 10);
	for (int i = 0; i < var_count; i++)
	{
		int var_indx = randint(VARIABLE_COUNT);
		int val = randint(100);
		ss << variables[var_indx] << "=" << val << "&";
	}

	request->set_requestdata(ss.str());
}

void DocumentsBuilder::fillName(Document_Name* name)
{
	int language_count = randint(2, 10);

	if (rand() < NAME_URL_RATIO)
	{
		int url_id = randint(URL_COUNT);
		name->set_url(urls[url_id]);
		language_count = randint(10);
	}

	for (int i = 0; i < language_count; i++)
	{
		Document_Name_Language* language = name->add_language();
		int code_id = randint(LANCODE_COUNT);
		language->set_code(LANG_CODE[code_id]);

		if (rand() < COUNTRY_RATIO)
		{
			int country_id = randint(COUNTRY_COUNT);
			language->set_country(COUNTRY_NAME[country_id]);
		}
	}
}

void DocumentsBuilder::fillDocument(Document* doc)
{
	/* paper schema */
	static int64_t DOCID = 1;
	int docid_inc = randint(2, 4);
	DOCID += docid_inc;
	doc->set_docid(DOCID);

	if (rand() < LINKS_RATIO)
	{
		Document_Links* links = doc->mutable_links();
		int bwd_count = randint(10);
		int fwd_count = randint(10);

		while (bwd_count+fwd_count==0)
		{
			bwd_count = randint(10);
			fwd_count = randint(10);
		}

		for (int i = 0; i < bwd_count; i++)
		{
			links->add_backward(rand());
		}
		for (int i = 0; i < fwd_count; i++)
		{
			links->add_forward(rand());
		}
	}
	int name_count = randint(10);

	for (int i = 0; i < name_count; i++)
	{
		Document_Name* name = doc->add_name();
		fillName(name);
	}

	/* extension */
	static int64_t SESSION_ID = 1;
	int session_inc = randint(2, 5);
	SESSION_ID += session_inc;
	doc->set_sessionid(SESSION_ID);
	doc->set_userid(randint(UID_COUNT));
	doc->set_usergroup(randint(10));
	//doc->set_usergroup(randint(10000));
	doc->set_clientip(randint(IP_COUNT));

	if (rand() < COUNTRY_RATIO)
	{
		doc->set_country(COUNTRY_NAME[randint(COUNTRY_COUNT)]);
		//doc->set_country(new_country[randint(100000)]);
	}

	if (rand() < AGENT_RATIO)
	{
		doc->set_agent(agents[randint(AGENT_COUNT)]);
	}

	if (rand() < SALE_RATIO)
	{
		Document_Sales* sale = doc->mutable_sales();
		fillSale(sale);
	}

	int request_count = randint(10);

	for (int i = 0; i < request_count; i++)
	{
		Document_Requests* request = doc->add_requests();
		fillRequest(request);
	}

}

string DocumentsBuilder::toString(Document* doc)
{
	stringstream stream;
	stream << "Document {\n";
	stream << "\tdocid = " << doc->docid() << ";\n";
	if (doc->has_links())
	{
		stream << "\tlinks {\n";
		Document_Links* links = doc->mutable_links();
		for (int i = 0; i < links->backward_size(); i++)
		{
			stream << "\t\tbackward = " << links->backward(i) << ";\n";
		}
		for (int i = 0; i < links->forward_size(); i++)
		{
			stream << "\t\tforward = " << links->forward(i) << ";\n";
		}
		stream << "\t}\n";

	}

	for (int i = 0; i < doc->name_size(); i++)
	{
		stream << "\tname {\n";
		Document_Name* name = doc->mutable_name(i);

		for (int j = 0; j < name->language_size(); j++)
		{
			stream << "\t\tlanguage {\n";
			Document_Name_Language* language = name->mutable_language(j);
			stream << "\t\t\tcode = " << language->code() << ";\n";
			if (language->has_country()) stream << "\t\t\tcountry = " << language->country() << ";\n";
			stream << "\t\t}\n";
		}

		if (name->has_url()) stream << "\t\turl = " << name->url() << ";\n";
		stream << "\t}\n";
	}

	/* for extended part*/
	stream << "\tSessionId = " << doc->sessionid() << ";\n";
	stream << "\tUserId = " << doc->userid() << ";\n";
	stream << "\tUserGroup = " << doc->usergroup() << ";\n";
	stream << "\tClientIp = " << doc->clientip() << ";\n";

	if (doc->has_country()) stream << "\tCountry = " << doc->country() << ";\n";
	if (doc->has_agent()) stream << "\tAgent = " << doc->agent() << ";\n";

	if (doc->has_sales())
	{
		stream << "\tsale {\n";
		Document_Sales* sale = doc->mutable_sales();
		stream << "\t\tProductId = " << sale->productid() << ";\n";
		stream << "\t\tProductName = " << sale->productname() << ";\n";
		stream << "\t\tsale = " << sale->sale() << ";\n";
		stream << "\t}\n";
	}

	for (int i = 0; i < doc->requests_size(); i++)
	{
		stream << "\trequest {\n";
		Document_Requests* request = doc->mutable_requests(i);
		stream << "\t\tRequestId = " << request->requestid() << ";\n";
		stream << "\t\tRequestTime = " << request->requesttime() << ";\n";
		stream << "\t\tResponseTime = " << request->responsetime() << ";\n";
		stream << "\t\tUrl = " << request->url() << ";\n";
		stream << "\t\tRequestData = " << request->requestdata() << ";\n";
		stream << "\t}\n";
	}
	stream << "}\n";
	return stream.str();

}

bool DocumentsBuilder::compare(Document* d1, Document* d2)
{
	if (d1 == d2) return true;
	string s1 = d1->SerializeAsString();
	string s2 = d2->SerializeAsString();

	if (s1 == s2) return true;
	return false;
}
