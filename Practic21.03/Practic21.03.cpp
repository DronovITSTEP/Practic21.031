#include <iostream>
#include <Windows.h>
#include <iomanip>

using namespace std;

struct User {
	char login[25];
	char password[25];
	int height;
	int weight;
};
struct Product {
	char name[50];
	int kkal;
};

// меню приложения
void StartMenu();
void MainMenu();

// вход и регистрация
bool SignIn();
User CreateUser();

// работа с файлами
bool ReadUser(const char*, const char*);
void SaveUser(User);
Product* SearchProduct(const char*);
bool IsEqualsUser(const char*);

// логика программы
void ShowProducts();
void AmountKkal();
void AddProduct(Product*);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	StartMenu();
}

void StartMenu() {
	char c;
	do {
		cout << "Добро пожаловать в программу \"Подсчет калорий\"\n"
			<< "1. Вход\n"
			<< "2. Зарегистрироваться\n"
			<< "3. Выход\n";
		cout << "ваш выбор -> "; cin >> c;

		switch (c)
		{
		case '1':
			if (SignIn()) MainMenu();
			break;
		case '2':
			SaveUser(CreateUser());
			MainMenu();
			break;
		}
		if (c < '1' || c > '3') {
			cout << "Ошибка выбора! Повторите попытку\n\n";
			cin.ignore();
			Sleep(500);
			system("cls");

		}
	} while (c != '3');

}
void MainMenu() {

	char c;
	do {
		system("cls");
		cout << "\"Подсчет калорий\"\n\n";

		cout << "1. Список продуктов\n"
			<< "2. Подсчитать полученные калории\n"
			<< "3. Выйти из программы\n";
		cout << "ваш выбор -> "; cin >> c;
		switch (c)
		{
		case '1':
			ShowProducts();
			break;
		case '2':
			AmountKkal();
			break;
		}
	} while (c != '3');

}

bool SignIn() {
	
	char login[25];
	char password[25];
	int counter = 0;
	do {
		system("cls");
		cout << "Введите логин -> "; cin >> login;
		cout << "Введите пароль -> "; cin >> password;

		if (ReadUser(login, password)) {
			cout << "Добро пожаловать!\n";
			return true;
		}
		cout << "Такого пользователя нет в системе\n"
			<< "Повторите попытку\n";
		Sleep(2000);
		counter++;
	} while (counter < 3);
	system("cls");
	int t = 10;
	while (t)
	{		
		cout << "Превшено кол-во попыток неправльного ввода. Пропробуйте заново через " << t << " секунд";
		Sleep(1000);
		t--;
		system("cls");		
	}
	return false;
}
bool ReadUser(const char* l, const char* p) {
	FILE* in;
	User user;

	if (!fopen_s(&in, "users.txt", "r")) {
		while (!feof(in)) {
			fscanf_s(in, "%s", user.login, 25);
			fscanf_s(in, "%s", user.password, 25);
			fscanf_s(in, "%d", &user.height);
			fscanf_s(in, "%d", &user.weight);

			if (strcmp(user.login, l) == 0 &&
				strcmp(user.password, p) == 0) {
				if (in != 0) fclose(in);
				return true;
			}
		}
	}
	if (in != 0) fclose(in);
	return false;

}

User CreateUser() {
	User user;

	do {
		cout << "Введите имя -> "; cin >> user.login;
	} while (IsEqualsUser(user.login));
	cout << "Введите пароль -> "; cin >> user.password;
	cout << "Введите рост -> "; cin >> user.height;
	cout << "Введите вес -> "; cin >> user.weight;

	return user;
}
void SaveUser(User user) {
	FILE* out;

	if (!fopen_s(&out, "users.txt", "a")) {
		fprintf(out, "\n");
		fprintf(out, "%s ", user.login);
		fprintf(out, "%s ", user.password);
		fprintf(out, "%d ", user.height);
		fprintf(out, "%d", user.weight);
	}
	if (out != 0) fclose(out);
}
bool IsEqualsUser(const char* c) {
	FILE* in;
	char* temp = new char[256];
	if (!fopen_s(&in, "users.txt", "r")) {
		while (!feof(in)) {
			fgets(temp, 256, in);
			if (strstr(temp, c)) {
				cout << "Такой пользователь уже зарегистрирован в сети\n";
				return true;
			}
		}
	}
	if (in != 0) fclose(in);
	return false;
}

void ShowProducts() {
	FILE* in;
	Product prod;
	int i = 1;
	cout << "Список продуктов\n";

	if (!fopen_s(&in, "products.txt", "r")) {
		while (!feof(in)) {
			fscanf_s(in, "%s", prod.name, 50);
			fscanf_s(in, "%d", &prod.kkal);

			cout << left << i << ". " << setw(20) << prod.name
				<< right << "kkal = " << prod.kkal << endl;
			i++;
		}
	}
	system("pause");
}
void AmountKkal() {
	Product* product;
	char prod[50];
	int weightProduct;
	double amount = 0;
	int i;
	do {
		cout << "Введите название продукта -> ";
		cin >> prod;
		if (!(product = SearchProduct(prod))) {
			product = new Product;
			strcpy_s(product->name, prod);
			cout << "Такого продукта нет в списке. Процесс добавления\n"
				<< "Укажите кол-во калорий на 100 грамм веса в этом продукте -> "; cin >> product->kkal;
			AddProduct(product);
		}
		cout << "Укажите вес продукта (грамм)-> ";
		cin >> weightProduct;
		amount += ((double)weightProduct / 100) * product->kkal;

		cout << "Добавить еще один продукт для подсчета? (1 - Да/0 - Нет) -> "; cin >> i;
	} while (i);
	cout << "Общее кол-во полученных калорий составила " << amount << endl;
	system("pause");
}
Product* SearchProduct(const char* p) {
	FILE* in;
	Product* prod = new Product;
	if (!fopen_s(&in, "products.txt", "r")) {
		while (!feof(in)) {
			fscanf_s(in, "%s", prod->name, 50);
			fscanf_s(in, "%d", &prod->kkal);

			if (!strcmp(prod->name, p)) {
				if (in != 0) fclose(in);
				return prod;
			}
		}
	}
	if (in != 0) fclose(in);
	return nullptr;
}
void AddProduct(Product* product) {
	FILE* out;

	if (!fopen_s(&out, "products.txt", "a")) {
		fprintf(out, "\n");
		fprintf(out, "%s ", product->name);
		fprintf(out, "%d", product->kkal);
	}
	if (out != 0) fclose(out);
}