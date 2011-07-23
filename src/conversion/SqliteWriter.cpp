/*
 * SqliteWriter.cpp
 *
 *  Created on: Jun 16, 2011
 *      Author: nhsan
 */

#include "SqliteWriter.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

SqliteWriter::SqliteWriter()
{

}

SqliteWriter::~SqliteWriter()
{
	close();
}

void SqliteWriter::open(string path)
{
	remove((path + "/document.sqlite").c_str());
	db = new Database((path + "/document.sqlite").c_str());
	Query q(*db);

	q.execute("CREATE TABLE Document(docid int, SessionID int, UserID int, UserGroup int, clientIP int, country char(100), agent char(100));");
	q.execute("CREATE TABLE Links_Backward(docid int, Backward int);");
	q.execute("CREATE TABLE Links_Forward(docid int, Forward int);");
	q.execute("CREATE TABLE Name(docid int, Url char(256),nameid INTEGER PRIMARY KEY AUTOINCREMENT);");
	q.execute("CREATE TABLE Name_Language(nameid int, Code char(50), Country char(100));");
	q.execute("CREATE TABLE Sales(docid int, productId int, productName char(100), sale int);");
	q.execute("CREATE TABLE Requests(docid int, RequestID int, RequestTime int, ResponseTime int, URL char(256), RequestData char(200));");
}

void SqliteWriter::close()
{
	if (db) delete db;
}

void SqliteWriter::write(Document* doc)
{
	Query q(*db);
	stringstream ins;
	stringstream values;
	ins << "INSERT INTO Document(docid,SessionID, UserID, UserGroup, clientIP";
	values << " VALUES(";
	values << doc->docid();
	values << "," << doc->sessionid();
	values << "," << doc->userid();
	values << "," << doc->usergroup();
	values << "," << doc->clientip();

	if (doc->has_country())
	{
		ins << ", country";
		values << ",'" << doc->country() << "'";
	}

	if (doc->has_agent())
	{
		ins << ", agent";
		values << ",'" << doc->agent() << "'";
	}

	ins << ")";
	values << ");";

	string sql = ins.str() + values.str();
	q.execute(sql);

	if (doc->has_links())
	{
		Document_Links* links = doc->mutable_links();
		for (int i = 0; i < links->backward_size(); i++)
		{
			stringstream bwd;
			bwd << "INSERT INTO Links_Backward(docid, Backward) VALUES(" << doc->docid() << "," << links->backward(i) << ");";
			q.execute(bwd.str());
		}

		for (int i = 0; i < links->forward_size(); i++)
		{
			stringstream fwd;
			fwd << "INSERT INTO Links_Forward(docid, Forward) VALUES(" << doc->docid() << "," << links->forward(i) << ");";
			q.execute(fwd.str());
		}
	}

	for (int i = 0; i < doc->name_size(); i++)
	{
		Document_Name* name = doc->mutable_name(i);

		if (name->has_url())
		{
			stringstream _name;
			//cout<<name->url()<<"\n";
			_name << "INSERT INTO Name(docid, Url) VALUES(" << doc->docid() << ",'" << name->url() << "');";
			q.execute(_name.str());
		}
		else
		{
			stringstream _name;
			_name << "INSERT INTO Name(docid) VALUES(" << doc->docid() << ");";
			q.execute(_name.str());

		}

		int64_t name_id = q.insert_id();
		for (int j = 0; j < name->language_size(); j++)
		{
			Document_Name_Language* language = name->mutable_language(j);

			if (language->has_country())
			{
				stringstream lang;
				lang << "INSERT INTO Name_Language(nameid, Code, Country) VALUES(" << name_id << ",'" << language->code() << "','" << language->country() << "');";
				q.execute(lang.str());
			}
			else
			{
				stringstream lang;
				lang << "INSERT INTO Name_Language(nameid, Code) VALUES(" << name_id << ",'" << language->code() << "');";
				q.execute(lang.str());
			}
		}
	}

	if (doc->has_sales())
	{
		Document_Sales* sale = doc->mutable_sales();
		stringstream _sale;
		_sale << "INSERT INTO Sales(docid, productId, productName, sale) VALUES(" << doc->docid() << "," << sale->productid() << ",'" << sale->productname() << "'," << sale->sale() << ");";
		q.execute(_sale.str());

	}

	for (int i = 0; i < doc->requests_size(); i++)
	{
		Document_Requests* request= doc->mutable_requests(i);
		stringstream req;
		req << "INSERT INTO Requests(docid, RequestID, RequestTime, ResponseTime, URL,RequestData) VALUES(" << doc->docid() << "," << request->requestid() << "," << request->requesttime() << "," << request->responsetime()<< ",'" << request->url() << "','" << request->requestdata()<< "');";
		q.execute(req.str());

	}
}
