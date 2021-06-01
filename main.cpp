// dict15.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "dictpara.h"
#include "array.h"
#include <time.h>
#include <vector>
typedef Array<DictPara> Container;

Container readDictionary(const char* fname,const char* bwname, const char* runame) {
	Container Dictionary;
	DictPara X;
	std::ifstream inFile(fname);
	std::ofstream bwFile(bwname);
	std::ofstream ruFile(runame);
	if (!inFile) throw stroka("Can not open dictionary ") + fname;
	int counter = 0;
	while (inFile.peek() != EOF) {
		inFile >> X;
		DictPara Z = X;
		char* p;
		p = X.get_translate().swc();

		//----------------------------------------------------------------
		
		std::vector<char*>vec;
		vec.push_back(p);
		int k = 0;
		while (*p != '\0')
		{
			if (*p == '(') k = 1;
			if (*p == ')') k = 0;
			if (*(p - 1) == ',' && k == 0)
			{
				*(p - 1) = '\0';
				if (*p == ' ' && *(p + 1) != '\0') ++p;
				vec.push_back(p);
			}
			++p;
		}
		for (int i = 0; i < vec.size(); ++i)
		{
			p = vec[i];
			while (*p != '\0')
			{
				if (*p == '(')
				{
					while (*p != ')' && *p != '\0')
					{
						*p = ' ';
						++p;
					}
					if (*p == ')') *p = ' ';
				}
				++p;
			}
			p = vec[i];
			while (*p == ' ')
			{
				++p;
			}
			char* t = vec[i];
			while (*t != '\0')
			{
				if (*t == ' ')
				{
					if (*(t - 1) == ' ' || *(t + 1) == '\0') *t = '\0';
				}
				++t;
			}
			vec[i] = p;
		}

		//------------------------------------------------------------------

		if (vec.size() > 0)
		{
			for (int i = 0; i < vec.size(); ++i)
			{
				char* t = vec[i]; int l = 0;
				while (*t!='\0')
				{
					if (*t == ' ' && *(t+1)!=' ' && *(t+1)!='\0')
					{
						++l;
						break;
					}
					++t;
				}
				if (strlen(vec[i]) > 0 && l == 0)
				{
					ruFile << DictPara(vec[i], X.get_word().str());
					Dictionary.push_back(DictPara(vec[i], X.get_word().str()));
					++counter;
				}
				else
				{
					bwFile << Z;
				}
			}
		}
		else bwFile << Z;
	}
	inFile.close();
	return Dictionary;
}

int main()
{
	char fname[] = "en_ru.txt";
	char bwname[] = "badwords.txt";
	char runame[] = "ru-en.txt";
	try {
		Container Dictionary;
		long tic = static_cast<long>(time(NULL));

		Dictionary = readDictionary(fname,bwname,runame);

		std::cout << "Reading from file: " << static_cast<long>(time(NULL)) - tic << " sec" << std::endl;
		std::cout << "Total: " << Dictionary.size() << std::endl;
		std::cout << "Capacity: " << Dictionary.capacity() << std::endl;
		/*
		for (auto pos = Dictionary.begin(); pos != Dictionary.end(); ++pos) // Array<DictPara>::iterator
			std::cout << *pos;

		for (auto pos = Dictionary.rbegin(); pos != Dictionary.rend(); ++pos)
				std::cout << *pos;
		*/

		for (unsigned int i = 0; i < Dictionary.size(); ++i)
			std::cout << Dictionary[i];

//		Dictionary.sort();
		/*
			Поиск слов в словаре Dictionary
		*/
		char what[128];
		Container::iterator pos;
		do {
			std::cout << "> ";
			std::cin.getline(what, 128);
			if (strlen(what) < 1) break;
			tic = static_cast<long>(time(NULL));
			for (int i = 0; i < 1000; ++i)
				pos = Dictionary.find(DictPara(what, ""));
			std::cout << "Finded at: " << static_cast<long>(time(NULL)) - tic << " sec" << std::endl;
			if (pos != Dictionary.end())
				std::cout << *pos;
			else
				std::cout << "There are no word '" << what << "' in dictionary" << std::endl;

			Dictionary.erase(pos);

			/*
			for (unsigned int i = 0; i < Dictionary.size(); ++i) {
				if (Dictionary[i] == DictPara(what, "")) {
					std::cout << Dictionary[i];
				}
			}
			*/
		} while (strlen(what) > 0);

	}
	catch (const stroka error) {
		std::cout << "Error: " << error << std::endl;
	}
	catch (const char* error) {
		std::cout << "Error: " << error << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error" << std::endl;
	}

	return 0;
}

/*
Написать программу "наивный переводчик".
Текст на английском языке из файла input.txt
Перевести дословно на русский язык и на китайский язык.
Сохранить переводы в файлах input_ru.txt и inputh_zh.txt
Если слова нет в словаре, то оставлять его написанным на английском языке.
Учесть запятые, точки и пр. .,:!?"'(){}[]<>

*/