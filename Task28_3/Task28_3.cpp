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
		return std::rand() % (3);
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

			if (inKitchenOrder.size() != 0)
			{
				inOrder.lock();
				int dish = inKitchenOrder.at(0);
				inKitchenOrder.erase(begin(inKitchenOrder));
				inOrder.unlock();
				printScrean.lock();
				std::cout << "It's being prepared now :" << printOrder(dish) << std::endl;
				printScrean.unlock();
				std::srand(time(nullptr));
				int timer = 5 + rand() % (10);
				std::this_thread::sleep_for(std::chrono::seconds(timer));
				inCourier.lock();
				inCourierOrder.push_back(dish);
				inCourier.unlock();
				printScrean.lock();
				std::cout << "Ready for delivery :" << printOrder(dish) << std::endl;
				printScrean.unlock();
			}
		}
	}
	void Courier(std::deque <int>& inCourierOrder)
	{
	
			inCourier.lock();
			for (int j = 0; j < inCourierOrder.size(); j++)
			{
				printScrean.lock();
				std::cout << "The order : " << printOrder(j) << " has been delivered" << std::endl;
				printScrean.unlock();
			}
			inCourierOrder.clear();
			inCourier.unlock();
			std::this_thread::sleep_for(std::chrono::seconds(30));
	}
};
int main()
{
	std::deque<int> kitchenOrder;
	std::deque<int> courierOrder;
	Restaurant restaurant;
	const int DELIVERY = 9;
	std::thread r1([&]()
		{
			restaurant.Waiter(kitchenOrder);
		});
	std::thread r2([&]()
		{
			restaurant.Kitchen(kitchenOrder, courierOrder);
		});
	
	std::thread r3([&]()
		{
			
			for (int i = 0; i < DELIVERY; i++) 
			{
				restaurant.Courier(courierOrder);
			}
		});
	r1.detach();
	r2.detach();
	r3.join();
	return 0;
}
