#ifndef CP5_ex12_27_h
#define CP5_ex12_27_h

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;
class QueryResult;
class TextQuery {
public:
	using LineNo = vector<string>::size_type;
	TextQuery(std::ifstream &);
	QueryResult query(const string&) const;
private:
	shared_ptr<vector<string>> input;    
	std::map<string, shared_ptr<std::set<LineNo>>> result;
};

TextQuery::TextQuery(std::ifstream &ifs) : input(new vector<string>)
{
	LineNo lineNo{ 0 };
	for (string line; std::getline(ifs, line); ++lineNo) {
		input->push_back(line);
		std::istringstream line_stream(line);
		for (string text, word; line_stream >> text; word.clear()) {
			std::remove_copy_if(text.begin(), text.end(), std::back_inserter(word), ispunct);
			auto &nos = result[word];
			if (!nos) nos.reset(new std::set<LineNo>);
			nos->insert(lineNo);
		}
	}
}

class QueryResult {
public:
	friend std::ostream& print(std::ostream &, const QueryResult&);
public:
	QueryResult(const string &s, shared_ptr<std::set<TextQuery::LineNo>> set, shared_ptr<vector<string>> v) : word(s), nos(set), input(v) { }
private:
	string word;
	shared_ptr<std::set<TextQuery::LineNo>> nos;
	shared_ptr<vector<string>> input;
};

QueryResult TextQuery::query(const string& str) const
{
	static shared_ptr<std::set<LineNo>> nodata(new std::set<LineNo>);
	auto found = result.find(str);
	if (found == result.end()) return QueryResult(str, nodata, input);
	else return QueryResult(str, found->second, input);
}

std::ostream& print(std::ostream &out, const QueryResult& qr)
{
	out << qr.word << " occurs " << qr.nos->size() << (qr.nos->size() > 1 ? " times" : " time") << std::endl;
	for (auto i : *qr.nos)
		out << "\t(line " << i + 1 << ") " << qr.input->at(i) << std::endl;
	return out;
}

#endif
