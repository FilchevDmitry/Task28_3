#include<iostream>
#include<deque>
#include<string>
#include<ctime>
#include<chrono>
#include<thread>
#include<mutex>

std::mutex inOrder, printScrean, inCourier;
	std::deque<int> kitchenOrder;
	std::deque<int> courierOrder;
class Restaurant
{
private:
	enum MENU 
	{
		PIZZA,
		SOUP, 
		STEAK, 
		SALAD,
		SUSHI
	};

public:
	int Order()
	{
		std::srand(std::time(nullptr));
		return std::rand() % (3);
	}
	std::string printOrder(int &numOrder)
	{	
		MENU menu;
		switch (numOrder)
		{
		case(0):
		{
			return "PIZZA";
			break;
		}
		case(1):
		{
			return "SOUP";
			break;
		}
		case(2):
		{
			return "STEAK";
			break;
		}
		case(3):
		{
			return "SALAD";
			break;
		}
		case(4):
		{
			return "SUSHI";
			break;
		}
		default:
			break;
		}
	}
	void Waiter()
	{
		while (true)
		{
			std::srand(time(nullptr));
			int timer = 5 + rand() % (5);
			std::this_thread::sleep_for(std::chrono::seconds(timer));
			inOrder.lock();
			int order = Order();
			kitchenOrder.push_back(order);
			printScrean.lock();
			std::cout << "Order accepted :" << printOrder(order) << std::endl;
			printScrean.unlock();
			inOrder.unlock();
		}
	}
	void Kitchen()
	{
		while (true)
		{
			if (kitchenOrder.size() != 0)
			{
				inOrder.lock();
				int dish = kitchenOrder.at(0);
				printScrean.lock();
				std::cout << "It's being prepared now :" << printOrder(dish) << std::endl;
				printScrean.unlock();
				std::srand(time(nullptr));
				int timer = 5 + rand() % (10);
				std::this_thread::sleep_for(std::chrono::seconds(timer));
				inCourier.lock();
				courierOrder.push_back(dish);
				printScrean.lock();
				std::cout << "Ready for delivery :" << printOrder(dish) << std::endl;
				printScrean.unlock();
				inCourier.unlock();
				kitchenOrder.erase(begin(kitchenOrder));
				inOrder.unlock();
			}
		}
	}
	void Courier()
	{
		const int delivery = 9;
		for (int i = 0; i < delivery; i++)
		{
			inCourier.lock();
			for (int j = 0; j < courierOrder.size(); j++)
			{
				std::cout << "The order : " << printOrder(j) << " has been delivered" << std::endl;
			}
			courierOrder.clear();
			inCourier.unlock();
			std::this_thread::sleep_for(std::chrono::seconds(30));
		}
	}
};
int main()
{
	Restaurant restaurant;
	std::thread r1(&Restaurant::Waiter);
	std::thread r2(&Restaurant::Kitchen);
	std::thread r3(&Restaurant::Courier);
	r1.detach();
	r2.detach();
	r3.join();
	return 0;
}
