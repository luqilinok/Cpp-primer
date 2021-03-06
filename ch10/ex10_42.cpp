#include<iostream>
#include<fstream>
#include<list>
#include<string>
#include<algorithm>

using namespace std;

inline void output_words(list<string> &words)
{
	for (auto iter=words.begin();iter!=words.end();iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;
}

void elimDups(list<string> &words)
{
	output_words(words);

	words.sort();  //使用其成员函数版本的算法，排序  
	output_words(words);

	words.unique(words);  //使用其成员函数版本的算法，删除重复的元素
	output_words(words);
}

int main(int arcg, char *argv[])
{
	ifstream in(argv[1]);
	if (!in)
	{
		cout << "打开输入文件失败！" << endl;
		return -1;
	}

	list<string> words;
	string word;
	while (in>>word)
	{
		words.push_back(word);
	}

	elimDups(words);
	system("pause");
	return 0;
}
