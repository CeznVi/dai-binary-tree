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

	string getNumTZ() { return numTZ; }
	string getData() { return data; }
	string getText() { return text; }
	int getSum() { return sum; }
	bool getPay() { return pay; }

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
	cout << "Num TZ: "; getline(in, p->numTZ);
	cout << "Data  : "; getline(in, p->data);
	cout << "Text  : "; getline(in, p->text);
	cout << "Summa : "; cin >> p->sum;
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

	fout << id << "\n";
	fout << numTZ << "\n";
	fout << data << "\n";
	fout << text << "\n";
	fout << sum << "\n";
	fout << pay << "\n\n";
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
	cout << "ADD PROTOCOL" << endl;
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
	save(); //видалити
	
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
	system("cls");
	cout << "Завантаження бази данних із файлу\n";
	Sleep(1000);



}

void BasePolice::setPay()
{
}

void BasePolice::menu()
{
	while (true)
	{
		system("cls");
		//int c = Menu::select_vertical({ "Додати протокол", 
		//								"Роздрукувати базу", 
		//								"Роздрукувати данні за номером",
		//								"Роздрукувати діапазон номерів",
		//								"Змінити статус оплати",
		//								"Вихід"}, 
		//								HorizontalAlignment::Center, 5);
		int c = Menu::select_vertical({ "Додати протокол",
								"Роздрукувати базу",
								"Роздрукувати данні за номером",
								"Роздрукувати діапазон номерів",
								"Змінити статус оплати",
								"Вихід", 
								"Завантажити базу з файлу"},
			HorizontalAlignment::Center, 5);
		switch (c)
		{
		case 0: addProtocol(); break;
		case 1: printAll(); break;
		case 2: printTZ(); break;
		case 3: printDiap(); break;
		case 4: setPay(); break;
		case 5: save(); exit(0);
		case 6: load(); break;    //ВИДАЛИТИ
		default:
			break;
		}
	}
}