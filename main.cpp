#include <iostream>
#include <Wt/dbo/dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>
#include <memory>
#include <ctime>

class Book;
class Stock;
class Sale;

class Publisher
{
public:
	std::string name;
	Wt::Dbo::collection < Wt::Dbo::ptr<Book>> publisher;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, publisher, Wt::Dbo::ManyToOne, "publisher");
	}
};

class Shop
{
public:
	std::string name;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> shop;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, shop, Wt::Dbo::ManyToOne, "shop");
	}
};
class Book
{
public:
	std::string title;
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> book;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher, "publisher");
		Wt::Dbo::hasMany(a, book, Wt::Dbo::ManyToOne, "book");
	}
};
class Stock
{
public:
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	int count = 0;
	Wt::Dbo::collection < Wt::Dbo::ptr<Sale> > stock;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::belongsTo(a, book, "book");
		Wt::Dbo::belongsTo(a, shop, "shop");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "stock");
	}
};

class Sale
{
public:
	int price = 0;
	std::string date_sale;
	Wt::Dbo::ptr<Stock> stock;
	int count = 0;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, stock, "stock");
		Wt::Dbo::field(a, count, "count");
	}
};



int main()
{
	std::string connection_string =
		"host=localhost "
		"port=5432 "
		"dbname=test "
		"user=postgres "
		"password=U12h12b12";
	try
	{
		std::unique_ptr<Wt::Dbo::backend::Postgres> con{ new Wt::Dbo::backend::Postgres(connection_string) };
		Wt::Dbo::Session s;
		s.setConnection(move(con));
		s.mapClass<Publisher>("publisher");
		s.mapClass<Shop>("shop");
		s.mapClass<Book>("book");
		s.mapClass<Stock>("stock");
		s.mapClass<Sale>("sale");
		
		Wt::Dbo::Transaction t{ s };
		s.dropTables();
		s.createTables();
		// --------------------------------------------------------------------------------------------------------------------
		// Create note in Publisher
		std::unique_ptr<Publisher> publisher{ new Publisher{"Erich Maria Remarque"} };
		std::unique_ptr<Publisher> publisher1{ new Publisher{"Jack London"} };

		Wt::Dbo::ptr<Publisher> publisher_ptr = s.add(move(publisher));
		Wt::Dbo::ptr<Publisher> publisher1_ptr = s.add(move(publisher1));

		// --------------------------------------------------------------------------------------------------------------------
		// Create note in Shop

		std::unique_ptr<Shop> shop1{ new Shop {"Library"} };
		std::unique_ptr<Shop> shop2{ new Shop{"Book of City"} };

		Wt::Dbo::ptr<Shop> shop1_ptr = s.add(move(shop1));
		Wt::Dbo::ptr<Shop> shop2_ptr = s.add(move(shop2));

		// --------------------------------------------------------------------------------------------------------------------
		// Create note in Book

		std::unique_ptr<Book> book_Three_Comrades{ new Book{"Three Comrades"} };
		book_Three_Comrades->publisher = publisher_ptr;
		std::unique_ptr<Book> book_a_time_to_love_and_time_to_die{ new Book{"A Time to Love and a Time to Die", publisher_ptr} };
		std::unique_ptr < Book > book_martin_eden{ new Book{"Martin Eden", publisher1_ptr} };
		std::unique_ptr<Book> book_smoke_bellow{ new Book{"Smoke Bellow", publisher1_ptr} };

		Wt::Dbo::ptr<Book> book_Three_Comrades_ptr = s.add(move(book_Three_Comrades));
		Wt::Dbo::ptr<Book> book_a_time_to_love_and_time_to_die_ptr = s.add(move(book_a_time_to_love_and_time_to_die));
		Wt::Dbo::ptr<Book> book_martin_eden_ptr = s.add(move(book_martin_eden));
		Wt::Dbo::ptr<Book> book_smoke_bellow_ptr = s.add(move(book_smoke_bellow));

		// --------------------------------------------------------------------------------------------------------------------
		// Create note in Stock

		std::unique_ptr<Stock> stock1{ new Stock{book_a_time_to_love_and_time_to_die_ptr, shop1_ptr, 20} };
		std::unique_ptr<Stock> stock2{ new Stock{book_Three_Comrades_ptr, shop1_ptr, 10} };
		std::unique_ptr<Stock> stock3{ new Stock{book_martin_eden_ptr, shop1_ptr, 14} };
		std::unique_ptr<Stock> stock4{ new Stock{book_smoke_bellow_ptr, shop1_ptr, 17} };
		std::unique_ptr<Stock> stock5{ new Stock{book_a_time_to_love_and_time_to_die_ptr, shop2_ptr, 4} };
		std::unique_ptr<Stock> stock6{ new Stock{book_Three_Comrades_ptr, shop2_ptr, 3} };
		std::unique_ptr<Stock> stock7{ new Stock{book_martin_eden_ptr, shop2_ptr, 28} };
		std::unique_ptr<Stock> stock8{ new Stock{book_smoke_bellow_ptr, shop2_ptr, 37} };
		Wt::Dbo::ptr<Stock> stock1_ptr = s.add(move(stock1));
		Wt::Dbo::ptr<Stock> stock2_ptr = s.add(move(stock2));
		Wt::Dbo::ptr<Stock>  stock3_ptr = s.add(move(stock3));
		Wt::Dbo::ptr<Stock>  stock4_ptr = s.add(move(stock4));
		Wt::Dbo::ptr<Stock>  stock5_ptr = s.add(move(stock5));
		Wt::Dbo::ptr<Stock>  stock6_ptr = s.add(move(stock6));
		Wt::Dbo::ptr<Stock>  stock7_ptr = s.add(move(stock7));
		Wt::Dbo::ptr<Stock>  stock8_ptr = s.add(move(stock8));

		// --------------------------------------------------------------------------------------------------------------------
		// Create note in Sale

		std::unique_ptr<Sale> sale1{ new Sale{13, "2024-09-30", stock1_ptr, 1} };
		std::unique_ptr<Sale> sale2{ new Sale{9, "2024-09-24", stock2_ptr, 3} };
		std::unique_ptr<Sale> sale3{ new Sale{10, "2024-09-29", stock3_ptr, 4} };
		std::unique_ptr<Sale> sale4{ new Sale{13, "2024-09-10", stock4_ptr, 1} };
		std::unique_ptr<Sale> sale5{ new Sale{4, "2024-09-14", stock5_ptr, 3} };
		std::unique_ptr<Sale> sale6{ new Sale{54, "2024-09-26", stock6_ptr, 7} };
		std::unique_ptr<Sale> sale7{ new Sale{11, "2024-09-26", stock7_ptr, 1} };
		std::unique_ptr<Sale> sale8{ new Sale{23, "2024-09-24", stock8_ptr, 3} };
		s.add(move(sale1));
		s.add(move(sale2));
		s.add(move(sale3));
		s.add(move(sale4));
		s.add(move(sale5));
		s.add(move(sale6));
		s.add(move(sale7));
		s.add(move(sale8));
		
		//------------------------------------------------------------------------------
		//Query into DB

		std::string query_publisher = "";
		std::cout << "Specify the publisher which you want to find: ";
		std::getline(std::cin, query_publisher);
		Wt::Dbo::ptr<Publisher> publisher_find = s.find<Publisher>().where("name=?").bind(query_publisher);
		std::cout << "Publisher: " << publisher_find->name << std::endl;
		std::cout << "His books are sold in: ";
		Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books = s.find<Book>().where("publisher_id=?").bind(publisher_find);
		for (Wt::Dbo::ptr<Book> book : books)
		{
			Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks = s.find<Stock>().where("book_id=?").bind(book);
			for (Wt::Dbo::ptr<Stock> stock : stocks)
			{
				Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> shops = s.find<Shop>().where("id=?").bind(stock->shop);
				for (Wt::Dbo::ptr<Shop> shop : shops)
				{
					std::cout << shop->name << ", ";
				}

			}
		}
		std::cout << std::endl;
		

		t.commit();


	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception happened: " << ex.what() << std::endl;
	}

}