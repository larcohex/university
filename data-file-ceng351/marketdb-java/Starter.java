package com.tauros;

public class Starter {
	public static IShoppingMall getShoppingMallHandler() {
		IShoppingMall shopping_mall = null;
		
		/******************************************************************
		 * Create new instance of the class that implements IShoppingMall interface
		 * and assign it to shopping_mall variable.
		 ******************************************************************/ 
		shopping_mall = (IShoppingMall)new IShoppingMallObj();
		
		return shopping_mall;
	}
}
