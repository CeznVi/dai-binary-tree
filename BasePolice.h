#pragma once
#include <string>
#include <fstream> // для збереження та завантаження файлів
#include"BTree.h"
#include"List.h"
#include"Menu.h"

using namespace std;

class Protocol
{
	static int count;
	int id;
	string numTZ;
	string data;
	string text;
	int sum{};
	bool pay = false;

public:
	Protocol() { id = count++; }
	friend ostream& operator<<(ostream& out, const Protocol* p);
	friend istream& operator>>(istream& in, Protocol* p);

	//Гетери данних
	string getNumTZ() { return numTZ; }
	string getData() { return data; }
	string getText() { return text; }
	int getSum() { return sum; }
	bool getPay() { return pay; }
	//Сетери данних
	void setNumTZ(string num) { numTZ = num; }
	void setData(string d) { data = d; }
	void setText(string t) { text = t; }
	void setSum(int s) { sum = s; }
	void setPay(bool p) { pay = p; }

	void save();

};

int Protocol::count = 1;

ostream& operator<<(ostream& out, const Protocol* p)
{
	out << p->id << " " << p->numTZ << " " << p->data
		<< " " << p->text << " " << p->sum << " " << ((p->pay) ? "Сплачено" : "Несплачено") << endl;
	return out;
}

istream& operator>>(istream& in, Protocol* p)
{
	cout << "Номер транспортного засобу : "; getline(in, p->numTZ);
	cout << "Дата правопорушення : "; getline(in, p->data);
	cout << "Правопорушення : "; getline(in, p->text);
	cout << "Сумма штрафу : "; cin >> p->sum;
	in.ignore();
	return in;
}

void Protocol::save()
{
	std::ofstream fout;
	fout.open("Protocol_DB.txt", std::ios::app);

	if (!fout)
    {
		std::cerr << "Помилка збереження файлу...";
		exit(1);
    }

	fout << numTZ << "\n";
	fout << data << "\n";
	fout << text << "\n";
	fout << sum << "\n";
	fout << pay << "\n";
	fout.close();
}

class BasePolice
{
	BTree<string, List<Protocol*>> base;
	friend ostream& operator<<(ostream& out, const Protocol* p);

	void addProtocol();
	void printAll();
	void printTZ();
	void printDiap();
	void save();
	void load();
	void setPay();
public:
	void menu();
};

void BasePolice::addProtocol()
{
	system("cls");
	cout << "Додавання протоколу" << endl;
	cout << "---------------------" << endl;
	Protocol* prot = new Protocol;
	cin >> prot;
	List<Protocol*>* list = base.getValue(prot->getNumTZ());
	if (!list)
	{
		List<Protocol*> newList;
		newList.push_back(prot);
		base.rpush(prot->getNumTZ(), newList);
	}
	else
	{
		list->push_back(prot);
	}
}

void BasePolice::printAll()
{
	system("cls");
	base.print();
	system("pause");
}

void BasePolice::printTZ()
{
	string numberTZ;

	system("cls");
	cout << "Роздрукувати данні за номером" << '\n';
	cout << "---------------------" << '\n';
	cout << "Введіть номер транспортного засобу" << '\n';
	cin >> numberTZ;
	//base.print(numberTZ);


}

void BasePolice::printDiap()
{
}

void BasePolice::save()
{
	system("cls");
	cout << "Збереження бази данних у файл\n";

	//Відкрити поток та видалити файл, якщо він існує
	//вимушена міра, через механізм збереження данних
	ofstream fout;
	fout.open("Protocol_DB.txt", std::ios::trunc);
	fout.close();

	base.save();
	Sleep(1000);
}

void BasePolice::load()
{
	const int size = 80;

	system("cls");
	cout << "Завантаження бази данних із файлу\n";
	Sleep(500);

	ifstream fin;
	fin.open("Protocol_DB.txt", std::ios::in);

	if (!fin)
	{
		std::cerr << "Помилка читання файлу...";
		exit(1);
	}

	if (fin.is_open())
	{
		while (fin)
		{
			char* temp = new char[size];
			Protocol* prot = new Protocol;
			int num{};

			fin.getline(temp, size);
			
			//невеличкий "костиль" для запобігання додавання пустого протоколу
			if (strlen(temp) == 0)
				break;

			prot->setNumTZ(temp);

			fin.getline(temp, size);
			prot->setData(temp);

			fin.getline(temp, size);
			prot->setText(temp);

			fin.getline(temp, size);
			num = atoi(temp);
			prot->setSum(num);

			fin.getline(temp, size);
			num = atoi(temp);
			prot->setPay(num);
						
			delete[] temp;

			List<Protocol*>* list = base.getValue(prot->getNumTZ());
			if (!list)
			{
				List<Protocol*> newList;
				newList.push_back(prot);
				base.rpush(prot->getNumTZ(), newList);
			}
			else
				list->push_back(prot);
		}
	}
	//system("pause");
	
	fin.close();
}

void BasePolice::setPay()
{
}

void BasePolice::menu()
{
	load();
	while (true)
	{
		system("cls");
		int c = Menu::select_vertical({ "Додати протокол", 
										"Роздрукувати базу", 
										"Роздрукувати данні за номером",
										"Роздрукувати діапазон номерів",
										"Змінити статус оплати",
										"Вихід"}, 
										HorizontalAlignment::Center, 5);

		switch (c)
		{
		case 0: addProtocol(); break;
		case 1: printAll(); break;
		case 2: printTZ(); break;
		case 3: printDiap(); break;
		case 4: setPay(); break;
		case 5: save(); exit(0);
		default:
			break;
		}
	}
}