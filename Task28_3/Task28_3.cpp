#include<iostream>
#include<deque>
#include<string>
#include<ctime>
#include<chrono>
#include<thread>
#include<mutex>

std::mutex inOrder, printScrean, inCourier;

class Restaurant
{
private:

public:
	int Order()
	{
		std::srand(std::time(nullptr));
		int num= rand() % (4);
		return num;
	}
	std::string printOrder(int &numOrder)
	{	
		
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
	void Waiter(std::deque <int> &inKitchenOrder)
	{
		while (true)
		{
			std::srand(time(nullptr));
			int order = Order();
			inOrder.lock();
			inKitchenOrder.push_back(order);
			inOrder.unlock();

			printScrean.lock();
			std::cout << "Order accepted :" << printOrder(order) << std::endl;
			printScrean.unlock();
			
			int timer = 5 + rand() % (5);
			std::this_thread::sleep_for(std::chrono::seconds(timer));
		}
	}
	void Kitchen(std::deque <int>& inKitchenOrder, std::deque <int>& inCourierOrder)
	{
		while (true)
		{		
				std::srand(time(nullptr));
				int timer = 5 + rand() % (10);
				std::this_thread::sleep_for(std::chrono::seconds(timer));
				inOrder.lock();
				int dishO = inKitchenOrder[0];
				inKitchenOrder.erase(begin(inKitchenOrder));
				inOrder.unlock();
				printScrean.lock();
				std::cout << "It's being prepared now :" << printOrder(dishO) << std::endl;
				printScrean.unlock();
				inCourier.lock();
				inCourierOrder.push_back(dishO);
				printScrean.lock();
				std::cout << "Ready for delivery :" << printOrder(dishO) << std::endl;
				printScrean.unlock();
				inCourier.unlock();
		}
	}
	void Courier(std::deque <int>& inCourierOrder)
	{		
		
			if (inCourierOrder.size() != 0)
			{
				inCourier.lock();
				for (int j = 0; j < inCourierOrder.size(); ++j)
				{
				printScrean.lock();
				std::cout << "The order : " << printOrder(inCourierOrder[j]) << " has been delivered" << std::endl;
				printScrean.unlock();
				}
				inCourierOrder.clear();
				inCourier.unlock();
			}	
	}
};
int main()
{
	std::deque<int> kitchenOrder;
	std::deque<int> courierOrder;
	Restaurant restaurant;
	const int DELIVERY = 10;
	std::thread order([&]()
		{
			restaurant.Waiter(kitchenOrder);
		});
	std::thread kitchen([&]()
		{
			restaurant.Kitchen(kitchenOrder, courierOrder);
		});

	for (int i = 0; i < DELIVERY; i++) 
	{
		std::this_thread::sleep_for(std::chrono::seconds(30));
		restaurant.Courier(courierOrder);
	}
		
	order.detach();
	kitchen.detach();
	
	return 0;
}
