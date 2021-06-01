#include <iostream>
#include<fstream>
#include<math.h>
#include"head.h"
#include<iomanip>
using namespace std;
static string FILENAME = "doc/brdm0990.txt";
static double GM = 3.986005e+14;
static double we = 7.292115e-5;
int main() {
	//��ʼ��
	fstream file;
	string str = "";
	gpsdat mdata;
	string name = "G01"; int year = 2021, month = 4, day = 9, hour = 4, min = 0; double sec = 0.148;

	//��ȡ����
	file.open(FILENAME, ios::in);
	for (int i = 0; i < 26; i++) {getline(file, str);}
	for (int i = 0;!file.eof(); i++) {
		cout << "���ڲ��ҵ�" << i+1 << "������\n";
		file >> mdata;
		if (mdata.name==name&& mdata.time[0] == year && mdata.time[1] == month && mdata.time[2] == day && mdata.time[3] == hour)
		{
			cout << "���ҵ���Ӧ��Ԫ����\n";
			break;
		}
		system("cls");
	}
	file.close();
	//mdata.ini();
	double temp, * a = mdata.a, * time = mdata.time;
	double n0, n, M, E, f, uu, delu, delr, deli, u, r, i, x, y, L, X, Y, Z;

	//���������ļ�
	file.open("report.txt", ios::out);


	//����gps��ʱ
	double number;
	double JD = 1721013.5 + 367 * time[0] - int(1.75 * (time[0] + int((time[1] + 9.0) / 12.0))) + time[2] + (time[3]+time[5]/3600.0) / 24.0 + int(275.0 * time[1] / 9.0);
	temp = int((JD- 2444244.5) / 7.0);
	double aaa = (JD - 2444244.5 - temp * 7.0) * 86400.0, bbb = a[11];
	number = (JD - 2444244.5 - temp * 7.0) * 86400.0 - a[11];

	if (number > 302400) {
		number -= 604800;
	}
	else if (number < -302400) {
		number += 604800;
	}
	//cout << number << endl;
	//�ο�ʱ��ƽ�����ٶ�n0
	 n0 = sqrt(GM) / (a[10] * a[10] * a[10]);

	//�۲�ʱ��ƽ�����ٶ�n
	 n = n0 + a[5];

	//�۲�˲�����ǵ�ƽ�����M
	 M = a[6] + n * number;

	//����ƫ�����E,��ʼ������ȷ����ֵtemp
	/*temp = 1; for (double temp1 = 0;;temp=temp1) {
		temp1 = temp - (M+ a[8] * sin(temp) - temp) / (a[8] * cos(temp) - 1);
		if (abs(temp1 - temp) < 0.00001) { E = temp1; break; }
	}*/
	 E = M;double E0 = 0, sigma = 10e-8;
	E = M + a[8] * sin(E);
	while (abs(E - E0) > sigma)
	{
		E0 = E;
		E = M + a[8] * sin(E0);
	}

	//����������f
	 f = atan2(sqrt(1 - a[8] * a[8]) * sin(E), cos(E) - a[8]);

	//�����Ǿ�u'
	 uu = a[17] + f;

	//�㶯������
	 delu, delr, deli;
	delu = a[7] * cos(2 * uu) + a[9] * sin(2 * uu);
	delr = a[16] * cos(2 * uu) + a[4] * sin(2 * uu);
	deli = a[12] * cos(2 * uu) + a[14] * sin(2 * uu);

	//��u',r',i0����
	 u = uu + delu;
	 r = a[10] * a[10] * (1 - a[8] * cos(E)) + delr;
	 i = a[15] + deli + a[19] * number;

	//�����ڹ��������ϵ�е�λ��
	 x = r * cos(u);
	 y = r * sin(u);

	//�۲�˲��������ľ���
	 L = a[13] + (a[18]-we)*(a[11]+number)-a[18]*a[11];

	//�ع�����ϵ�µ�����
	 X = x * cos(L) - y * cos(i) * sin(L);
	 Y = x * sin(L) + y * cos(i)*cos(L);
	 Z = y * sin(i);

	cout <<"����������report.txt�ļ�����鿴\n";
	
	file <<setprecision(15)<<"��������\n"
		<<"�۲�˲�䣺" << year << "/" << month << "/" << day << "   " << hour << ":" << min << ":" << sec << endl
		<< "�۲�ʱ��ƽ�����ٶ�n��" << n<< endl << "�۲�˲��ƽ�����M��" << M << endl
		<< "����ƫ�����E��" << E << endl << "������f��" << f << endl
		<< "�����Ǿ�u'��" << uu << endl << "�㶯������deltu��" << delu << endl
		<< "          deltr��" << delr << endl << "          delti��" << deli << endl
		<< "�����ڹ�����ϵ�λ�ã���" << x << " , " << y << ")" << endl
		<< "�۲�˲��������ľ���L��" << L << endl
		<< "�ع�����ϵ�µ����꣺(" << X << " , " << Y << " , " << Z << ")\n";
	file.close();
	system("pause");
	return 0;
}