/*
 * DocumentsBuilder.h
 *
 *  Created on: May 26, 2011
 *      Author: nhsan
 */

#ifndef DOCUMENTSBUILDER_H_
#define DOCUMENTSBUILDER_H_

#include "constants.h"
#include "../schema/epaper.pb.h"
using namespace std;

class DocumentsBuilder
{
public:
	DocumentsBuilder();
	virtual ~DocumentsBuilder();
	Document* buildDocument();
	void fillDocument(Document* doc);
	static string toString(Document* doc);
	static bool compare(Document* d1, Document* d2);
private:
	char* agents[AGENT_COUNT];
	char* product_names[PRODUCT_COUNT];
	char* urls[URL_COUNT];
	char* variables[URL_COUNT];
	char* new_country[100000];

	void fillSale(Document_Sales* sale);
	void fillRequest(Document_Requests* request);
	void fillName(Document_Name* name);
};

#endif /* DOCUMENTSBUILDER_H_ */
