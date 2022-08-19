#pragma once
#include <string>
#include <fstream>
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
	int getId() const { return id; }
	string getNumTZ() const { return numTZ; }
	string getData() const { return data; }
	string getText() const { return text; }
	int getSum() const { return sum; }
	bool getPay() const { return pay; }
	//Сетери данних
	void setNumTZ(string num) { numTZ = num; }
	void setData(string d) { data = d; }
	void setText(string t) { text = t; }
	void setSum(int s) { sum = s; }
	void setPay(bool p) { pay = p; }
	//Метод зберігання
	void save();
	//Метод зміни оплати
	void changePay(int p)
	{
		if (p == 1)
			pay = true;
		else
			pay = false;
	}

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
	in.ignore();
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
	getline(cin, numberTZ);

	List<Protocol*>* list = base.getValue(numberTZ);
	
	if (list != nullptr)
		list->print();
	else
		cout << "\nЗа введеним номером правопорушень не знайдено\n";

	system("pause");
	
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
	string numberTZ;
	int id = -1;
	int chengeId{};
	int pay = -1;

	system("cls");
	cout << "Змінити статус оплати правопорушення" << '\n';
	cout << "---------------------" << '\n';
	cout << "Введіть номер транспортного засобу" << '\n';
	getline(cin, numberTZ);

	List<Protocol*>* list = base.getValue(numberTZ);

	if (list != nullptr)
	{
		cout << "\n\nВ базі знайдено наступні правопорушення за введеним номером: \n";
		list->print();
		
		int count = list->length();

		int* whiteID = new int[count];

		for (size_t i{}; i < count; i++)
		{
			whiteID[i] = list->at(i)->getId();
		}

		cout << "Доступні ID: ";

		for (size_t i{}; i < count; i++)
			cout << whiteID[i] << ' ';

		//Перевірка на допустимий діапазон вводимих айді
		while (!isRange(id, whiteID, count))
		{
			cout << "\nВведіть id правопорушення для зміни його статусу\n";
			cin >> id;

			if(!isRange(id, whiteID, count))
				cout << "Невірний id\n";

		}

		for (size_t i{}; i < count; i++)
		{
			if (id == list->at(i)->getId())
				chengeId = i;
		}

		//Записуємо у вказівник адресу нашого порушення зі списку
		Protocol* prot = list->at(chengeId);
	
		cout << prot;
		
		//Отримання від користувача коректних данних стосовно оплати протоколу
		while (pay != 0 && pay != 1)
		{
			cout << "Для зміни статусу проплати протокола введиіть:\n";
			cout << "1 - сплачено\n";
			cout << "0 - не сплачено\n";
			cin >> pay;
		}
		
		prot->changePay(pay);

		cout << "Протокол після змін:\n";
		cout << prot;

	}
	else
		cout << "\nЗа введеним номером правопорушень не знайдено\n";

	system("pause");

	
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