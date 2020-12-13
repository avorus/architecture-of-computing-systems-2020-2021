#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

using namespace std;

/// <summary>
/// Осуществляет корректный ввод целого положительного числа в диапазоне [min, max].
/// </summary>
/// <param name="message">Введите {message}:</param>
/// <param name="min">левая граница диапазона</param>
/// <param name="max">правая граница диапазона</param>
/// <returns>корректное значение</returns>
int correctInput(string message, int min, int max)
{
	cout << "Введите " << message << "(от " << min << " до " << max << "): ";
	int input;
	while (!(cin >> input && min <= input && input <= max))
	{
		cout << "Что-то не то... Попейте кофе и посмотрите внимательнее: ";
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(32767, '\n');
		}
	}
	return input;
}

//Массив данных с женскими русскими именами
string women[] = {"Анастасия", "Мария", "Анна", "Виктория", "Екатерина", "Наталья", "Марина", "Вероника", "Жанна", "Светлана", "Татьяна", 
"Кристина", "Наталья", "Полина", "София", "Дарья", "Алиса", "Ксения", "Александра", "Елена", "Алина", "Виктория"};
//Массив данных с мужскими русскими именами
string men[] = {"Владислав", "Игорь", "Владимир", "Павел", "Руслан", "Марк", "Константин", "Тимур", "Олег", "Ярослав", "Антон", "Николай", "Даниил",
"Александр", "Дмитрий", "Максим", "Сергей", "Андрей", "Алексей", "Артём", "Илья", "Кирилл", "Михаил", "Никита", "Денис", "Евгений", "Тимофей"};

/// <summary>
/// Посетитель парикмахеской.
/// </summary>
class Visitor
{
	bool sex; //пол посетителя: false - мужчина, true - женщина
	string name; //имя посетителя
public:
	/// <summary>
	/// Создаёт нового посетителя, генерируя пол и имя.
	/// </summary>
	explicit Visitor()
	{
		sex = rand() % 2;
		name = sex ? women[rand() % women->size()] : men[rand() % men->size()];
	}
	/// <summary>
	/// Возвращает логическое значение, является ли данный посетитель женщиной.
	/// </summary>
	/// <returns>пол посетителя</returns>
	bool isWoman()
	{
		return sex;
	}
	/// <summary>
	/// Возвращает строковое представление посетителя.
	/// </summary>
	/// <returns>имя посетителя</returns>
	string toString()
	{
		return name;
	}
};

/// <summary>
/// Парикмахерская.
/// </summary>
class Barbershop
{
	queue<Visitor> visitors; //очередь посетителей

	mutex barber; //замок в кабинет парикмахера
	condition_variable newVisitor;

	mutex output;

	bool isAllVisitorsCut = false; //подстрижены ли все посетители
	bool isVisitorCame = false; //пришёл ли посетитель

	/// <summary>
	/// Имитирует процесс стрижки для данного посетителя.
	/// </summary>
	/// <param name="visitor">посетитель</param>
	void cut_hair(Visitor &visitor)
	{
		//Обычно стрижки женщин занимают больше времени, чем стрижка мужчин.
		this_thread::sleep_for(chrono::seconds(visitor.isWoman() ? visitor.toString().length() % 3 + 5: visitor.toString().length() % 3 + 1));
	}
public:
	/// <summary>
	/// Имитирует приход посетителя в парикмахерскую.
	/// </summary>
	/// <param name="numberVisitors"></param>
	void visitorCome(int numberVisitors) {
		int count = 1; //номер посетителя
		while (count <= numberVisitors) {
			//Создание посетителя
			Visitor visitor = Visitor();
			//Посетитель идёт в парикмахерскую
			if(rand() % 2) this_thread::sleep_for(chrono::milliseconds(100));
			//Посетитель пришёл в парикмахерскую
			output.lock();
			cout << "Посетитель " << visitor.toString() << (visitor.isWoman() ? " пришла" : " пришёл") << " в парикмахерскую.\n";
			output.unlock();
			//Посетитель встаёт в очередь
			unique_lock<mutex> lock(barber);
			visitors.push(visitor);
			count++;
			isVisitorCame = true;
			newVisitor.notify_one();
		}
		isAllVisitorsCut = true;
		newVisitor.notify_one();
	}

	void barberWork() {
		unique_lock<mutex> lock(barber);
		while (!isAllVisitorsCut) {
			while (!isVisitorCame) {
				newVisitor.wait(lock);
			}
			//Парикмахера разбудили: пришёл посетитель или все подстрижены
			while (!visitors.empty()) { //если есть посетители, ожидающие свою очередь
				//Парикмахер берёт нового посетителя
				Visitor visitor = visitors.front(); //первый посетитель в очереди
				output.lock();
				cout << "Посетитель " << visitor.toString() << (visitor.isWoman() ? " села" : " сел") << " в кресло к парикмахеру.\n";
				output.unlock();
				//Парикмахер начинает стрижку
				cut_hair(visitor);
				//Парикмахер закончил стрижку и посетитель ушёл
				output.lock();
				cout << "Посетитель " << visitor.toString() << (visitor.isWoman() ? " сделала стрижку и ушла" : " сделал стрижку и ушёл") << ".\n";
				output.unlock();
				visitors.pop();
			}
			isVisitorCame = false;
		}
	}
};

int main() {
	srand(static_cast<unsigned int>(time(0)));

	setlocale(LC_ALL, "Russian");
	cout << "Вы - администратор в парикмахерской!\n";
	cout << "Вы пришли на работу, открыли парикмахерскую, включили везде свет, сделали себе кофе и сели на рабочее место.\n";
	cout << "\"Интересно, сколько на сегодня записей?\", - думаете вы и решаете посмотреть в журнал.\n";

	int numberVisitors = correctInput("количество посетителей на сегодня", 1, 1000);

	Barbershop barbershop;
	cout << "Парикмахер пришёл на работу. Вы его обрадовали новостью, что у него сегодня " << numberVisitors << " клиентов.\n";
	thread barber(&Barbershop::barberWork, ref(barbershop));
	thread visitors(&Barbershop::visitorCome, ref(barbershop), numberVisitors);
	visitors.join();
	barber.join();
	cout << "Парикмахер закончил работу и ушёл домой уставший.\n";
	cout << "Вы собираете вещи, выключаете свет, закрываете парикмахерскую. Сегодня был трудный день.\n";

	return 0;
}

