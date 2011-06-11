/*
 * country.h
 *
 *  Created on: May 25, 2011
 *      Author: nhsan
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define GROUP_COUNT		10
#define UID_COUNT		50
#define IP_COUNT		70
#define AGENT_COUNT		15
#define PRODUCT_COUNT	100
#define SALE_COUNT		1000
#define URL_COUNT		500

#define UGROUP_BASE				1
#define UID_BASE				12345
#define CLIENTIP_BASE			54321

#define SALE_RATIO				((RAND_MAX/100)*80)	//3%
#define COUNTRY_RATIO			((RAND_MAX/100)*80)	//80%
#define AGENT_RATIO				((RAND_MAX/100)*55)	//55%
#define LINKS_RATIO				((RAND_MAX/100)*75)	//75%
#define NAME_URL_RATIO			((RAND_MAX/100)*50)

#define COUNTRY_COUNT	128
#define LANGUAGE_COUNT	130
#define LANCODE_COUNT	123

static char* COUNTRY_NAME[COUNTRY_COUNT] = { "Afghanistan", "Albania", "Algeria", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", "Bahrain", "Bangladesh", "Belarus", "Belgium", "Belize", "Bolivarian Republic of Venezuela", "Bolivia", "Bosnia and Herzegovina", "Brazil", "Brunei Darussalam", "Bulgaria", "Cambodia", "Canada", "Caribbean", "Chile", "Colombia", "Costa Rica", "Croatia", "Czech Republic", "Denmark", "Dominican Republic", "Ecuador", "Egypt", "El Salvador", "Estonia", "Ethiopia", "Faroe Islands", "Finland", "France", "Georgia", "Germany", "Greece", "Greenland", "Guatemala", "Honduras", "Hong Kong S.A.R.", "Hungary", "Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Islamic Republic of Pakistan", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Korea", "Kuwait", "Kyrgyzstan", "Lao P.D.R.", "Latvia", "Lebanon", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Macao S.A.R.", "Macedonia (FYROM)", "Malaysia", "Maldives", "Malta", "Mexico", "Mongolia",
		"Montenegro", "Morocco", "Nepal", "Netherlands", "New Zealand", "Nicaragua", "Nigeria", "Norway", "Oman", "Panama", "Paraguay", "People's Republic of China", "Peru", "Philippines", "Poland", "Portugal", "Principality of Monaco", "Puerto Rico", "Qatar", "Republic of the Philippines", "Romania", "Russia", "Rwanda", "Saudi Arabia", "Senegal", "Serbia", "Serbia and Montenegro (Former)", "Singapore", "Slovakia", "Slovenia", "South Africa", "Spain", "Sri Lanka", "Sweden", "Switzerland", "Syria", "Taiwan", "Tajikistan", "Thailand", "Trinidad and Tobago", "Tunisia", "Turkey", "Turkmenistan", "U.A.E.", "Ukraine", "United Kingdom", "United States", "Uruguay", "Uzbekistan", "Vietnam", "Yemen", "Zimbabwe" };

static char* LANGUAGE[LANGUAGE_COUNT] = { "Dari", "Pashto", "Albanian", "Arabic", "Tamazight (Latin)", "Spanish", "Armenian", "English", "German", "Azeri (Cyrillic)", "Azeri (Latin)", "Bengali", "Belarusian", "Dutch", "French", "Quechua", "Bosnian (Cyrillic)", "Bosnian (Latin)", "Croatian", "Serbian (Cyrillic)", "Serbian (Latin)", "Portuguese", "Malay", "Bulgarian", "Khmer", "Inuktitut (Latin)", "Inuktitut (Syllabics)", "Mohawk", "Mapudungun", "Czech", "Danish", "Estonian", "Amharic", "Faroese", "Finnish", "Sami (Inari)", "Sami (Northern)", "Sami (Skolt)", "Swedish", "Alsatian", "Breton", "Corsican", "Occitan", "Georgian", "Lower Sorbian", "Upper Sorbian", "Greek", "Greenlandic", "K'iche", "Chinese (Traditional) Legacy", "Hungarian", "Icelandic", "Assamese", "Gujarati", "Hindi", "Kannada", "Konkani", "Malayalam", "Marathi", "Oriya", "Punjabi", "Sanskrit", "Tamil", "Telugu", "Indonesian", "Persian", "Irish", "Urdu", "Hebrew", "Italian", "Japanese", "Kazakh", "Kiswahili", "Korean", "Kyrgyz", "Lao", "Latvian",
		"Lithuanian", "Luxembourgish", "Macedonian (FYROM)", "Divehi", "Maltese", "Mongolian (Cyrillic)", "Nepali", "Frisian", "Maori", "Hausa (Latin)", "Igbo", "Yoruba", "Norwegian (Bokm�l)", "Norwegian (Nynorsk)", "Sami (Lule)", "Sami (Southern)", "Chinese (Simplified) Legacy", "Mongolian (Traditional Mongolian)", "Tibetan", "Uyghur", "Yi", "Filipino", "Polish", "Romanian", "Bashkir", "Russian", "Tatar", "Yakut", "Kinyarwanda", "Wolof", "Slovak", "Slovenian", "Afrikaans", "isiXhosa", "isiZulu", "Sesotho sa Leboa", "Setswana", "Basque", "Catalan", "Galician", "Sinhala", "Romansh", "Syriac", "Tajik (Cyrillic)", "Thai", "Turkish", "Turkmen", "Ukrainian", "Scottish Gaelic", "Welsh", "Uzbek (Cyrillic)", "Uzbek (Latin)", "Vietnamese" };

static char* LANG_CODE[LANCODE_COUNT] = { "prs", "ps", "sq", "ar", "tzm", "es", "hy", "en", "de", "az", "bn", "be", "nl", "fr", "quz", "bs", "hr", "sr", "pt", "ms", "bg", "km", "iu", "moh", "arn", "cs", "da", "et", "am", "fo", "fi", "smn", "se", "sms", "sv", "gsw", "br", "co", "oc", "ka", "dsb", "hsb", "el", "kl", "qut", "zh", "hu", "is", "as", "gu", "hi", "kn", "kok", "ml", "mr", "or", "pa", "sa", "ta", "te", "id", "fa", "ga", "ur", "he", "it", "ja", "kk", "sw", "ko", "ky", "lo", "lv", "lt", "lb", "mk", "dv", "mt", "mn", "ne", "fy", "mi", "ha", "ig", "yo", "nb", "nn", "smj", "sma", "bo", "ug", "ii", "fil", "pl", "ro", "ba", "ru", "tt", "sah", "rw", "wo", "sk", "sl", "af", "xh", "zu", "nso", "tn", "eu", "ca", "gl", "si", "rm", "syr", "tg", "th", "tr", "tk", "uk", "gd", "cy", "uz", "vi" };


#endif /* CONSTANTS_H_ */
