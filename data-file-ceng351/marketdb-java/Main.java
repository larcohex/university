package com.tauros;


public class Main {

	public static void main(String[] args) {
		IShoppingMall mall = Starter.getShoppingMallHandler();
		// Initializations
		mall.onStart();
		// register users
		try {
			mall.registerUser("Emre Isikligil", "12345678910", "Computer Eng. Dep. A-402 METU Ankara +3122105592");
			mall.registerUser("Izzet Altinmese", "23456789101", "Somewhere in the planet");
			mall.addStore("Armani Jeans", Category.WEAR);
		} catch (UserAlreadyExists e) {
			System.out.println(UserAlreadyExists.message);
		} catch (StoreAlreadyExists e) {
			System.out.println(StoreAlreadyExists.message);
		}

		// Get an user object
		IUser user = mall.getUser("12345678910");
		System.out.println (user.getFullName());
		// Get an store object ans check it
		IStore[] stores = mall.searchStore("Armani", Category.WEAR);
		if(stores.length != 1)
		{
			System.out.println("Wrong number of store returned");
			return;
		}
		else if(!stores[0].getStoreName().equals("Armani Jeans") || stores[0].getStoreCategory() != Category.WEAR)
		{
			System.out.println("Wrong store returned");
			return;
		}
		user.shopped(stores[0], "10.11.2013", 275.0);
		
		// Get shoppings of the user and check it
		IShopping[] shoppings = user.getShoppings();
		if(shoppings.length != 1)
		{
			System.out.println("Wrong number of shoppings returned");
			return;
		}
		else if(!shoppings[0].getStore().getStoreName().equals("Armani Jeans") || !shoppings[0].getUser().getId().equals("12345678910") ||
				shoppings[0].getAmount() != 275.0)
		{
			System.out.println("Wrong shopping info returned!");
		}
		
		
		user.shopped(stores[0],"11.11.2013", 153.0);
		user.shopped(stores[0],"12.11.2013", 59.0);
		user.shopped(stores[0],"13.11.2013", 80.0);
		user.shopped(stores[0],"14.11.2013", 80.0);
		user.shopped(stores[0],"14.12.2013", 80.0);
		System.out.println (user.checkGovernorOffer());
	}
}
