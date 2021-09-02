#include<algorithm>
#include<conio.h>
#include<fstream>

namespace chili {

	
	//printf
	void print(const char* s)
	{
		for (; *s != 0;s++)
		{
			_putch(*s);
		}
	}

	void printfixed(const char* s, int width)
	{
		int n = 0;
		for (; *s != 0;s++)
		{
			n++;
			_putch(*s);
		}

		//남은 길이만큼 띄워주기
		for (;n < width;n++)
		{
			_putch(' ');
		}

	}

	//string copy
	void strcpy(const char* pSrc, char* pDst, int maxBuffSize)
	{
		int n = 0;
		for (;*pSrc != 0 && (n + 1 < maxBuffSize); pSrc++, pDst++, n++)
		{
			*pDst = *pSrc;
		}
		*pDst = 0;
	}


	//scanf
	void read(char* buf, int maxsize)
	{
		const char* const pEnd = buf + maxsize;
		for (char c = _getch();c != 13 && (buf + 1 < pEnd); c = _getch(), buf++)
		{
			_putch(c);
			*buf = c;
		}
		*buf = 0;
	}

	int str2int(const char* s)
	{
		//scan to start point
		const char* p = s;

		for (;*p >= '0' && *p <= '9';p++);//다음 주소를 가리킨다.
		p--;

		int val = 0;
		int place = 1;

		//convert place values and accumulate
		for (;p >= s;p--)
		{
			val += (*p - '0') * place;
			place *= 10;
		}

		return val;
	}

	void strev(char* pl)
	{
		//scan to start point
		char* pr = pl;

		for (;*pr != 0;pr++);//다음 주소를 가리킨다.
		pr--;

		for (;pl < pr;pl++, pr--)
		{
			const char temp = *pl;
			*pl = *pr;
			*pr = temp;
		}
	}


	void int2str(int val, char* buf, int size)
	{
		char* const pStart = buf;
		char* const pEnd = buf + size;
		for (; val > 0 && (buf + 1 < pEnd);val /= 10, buf++)
		{
			*buf = '0' + val % 10;
		}
		*buf = 0;

		strev(pStart);
	}

	class Database
	{
	private:
		class Entry
		{
		public:
			Entry() = default;
			Entry(const char* name, int value)
				:
				value(value)
			{
				strcpy(name, this->name,sizeof(this->name));
			}
			void Print() const
			{
				printfixed(name, nameBufferSize - 1);
				_putch('|');
				for (int i = 0; i < value;i++)
				{
					_putch('=');
				}
				_putch('\n');
			}
			void Serialize(std::ofstream& out)const
			{
				out.write(name, sizeof(name));
				out.write(reinterpret_cast<const char*>(&value), sizeof(value));
			}
			void Deserialize(std::ifstream& in)
			{
				in.read(name, sizeof(name));
				in.read(reinterpret_cast<char*>(&value), sizeof(value));
			}
		private:
			static constexpr int nameBufferSize = 10;
			char name[nameBufferSize];
			int value;
		};
	public:
		void Load(const char* filename)
		{
			std::ifstream in(filename, std::ios::binary);
			in.read(reinterpret_cast<char*>(&curNumberEntries), sizeof(curNumberEntries));

			for (int i = 0; i < curNumberEntries;i++)
			{
				entries[i].Deserialize(in);
			}
		}
		void Save(const char* filename) const
		{
			std::ofstream out(filename, std::ios::binary);
			out.write(reinterpret_cast<const char*>(&curNumberEntries), sizeof(curNumberEntries));

			for (int i = 0; i < curNumberEntries;i++)
			{
				entries[i].Serialize(out);
			}
		}
		void Print() const
		{
			for (int i = 0; i < curNumberEntries;i++)
			{
				entries[i].Print();
			}
		}
		void Add(const char* name, int val)
		{
			if (curNumberEntries < maxNumberEntries)
			{
				entries[curNumberEntries++] = { name, val };
			}
		}
	private:
		static constexpr int maxNumberEntries = 16;
		Entry entries[maxNumberEntries];
		int curNumberEntries = 0;
	};

	
}

int main()
{
	chili::Database db;
	char buffer[256];
	char buffer2[256];
	bool quitting = false;
	do
	{
		chili::print("(l)oad (s)ave (a)dd (q)uit or (p)rint?");
		char response = _getch();
		switch (response)
		{
		case 'l':
			chili::print("\nEnter file name: ");
			chili::read(buffer, sizeof(buffer));
			db.Load(buffer);
			_putch('\n');
			break;
		case 's':
			chili::print("\nEnter file name: ");
			chili::read(buffer, sizeof(buffer));
			db.Save(buffer);
			_putch('\n');
			break;
		case'a':
			chili::print("\nEnter  name: ");
			chili::read(buffer, sizeof(buffer));
			chili::print("\nEnter  value: ");
			chili::read(buffer2, sizeof(buffer2));
			db.Add(buffer, chili::str2int(buffer2));
			_putch('\n');
			break;
		case'p':
			chili::print("\n       Beautiful Chart Bitched!");
			chili::print("\n       ------------------------\n\n");
			db.Print();
			_putch('\n');
			break;
		case'q':
			quitting = true;
			break;
		}

	} while (!quitting);
	
	while (!_kbhit());


	return 0;
}