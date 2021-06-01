#pragma once
#include <regex>
#include<string>
#include<iostream>
using namespace std;
class gpsdat {
public:	
	string name;//卫星编号
	double a[31];//每个数据块有31个数值
	double time[6];//参考历元 年月日时分秒
	void ini();
	friend istream& operator>>(istream&, gpsdat&);
};
void gpsdat::ini() {
	time[0] += 2000.0;
}
istream& operator>>(istream& input, gpsdat& gps) {
	const  string pattern1 = "(\\d+(\\.\\d+)?)";regex r1(pattern1);
	const  string pattern2 = "(D)";regex r2(pattern2);
	const  string pattern3 = "(-?\\d\\.\\d{12}e[\\+|-]\\d{2})";regex r3(pattern3);
	const string pattern4 = "([GRSE]\\d{2})"; regex r4(pattern4);
	const  int count = 8;
	string str = "", strtem = "";
	for (int i = 0; i < count; i++) {
		getline(input, strtem);
		str += strtem;
	}
	smatch result; 
	string::const_iterator itbegin = str.begin();
	string::const_iterator itend = str.end();

	regex_search(itbegin, itend, result, r4); gps.name = result[0]; itbegin = result[0].second;
	for (int i = 0; i < 6 && regex_search(itbegin, itend, result, r1); ++i) {
		strtem = result[0];
		gps.time[i] = atof(strtem.c_str());
		itbegin = result[0].second;
	}
	for (int i = 0; i < 31 && regex_search(itbegin, itend, result, r3); ++i) {
		strtem = result[0];
		//strtem = regex_replace(strtem, r2, "e");
		gps.a[i]=atof(strtem.c_str());
		itbegin = result[0].second;
	}
	return input;
}