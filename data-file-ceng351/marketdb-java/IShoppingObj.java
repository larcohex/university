package com.tauros;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;


public class IShoppingObj implements IShopping {
	private String date;
	private double amount;
	private String userID;
	private String storeName;
	private Category storeCategory;
	
	
	
	
	
	private String user = "e1848324";
	private String password = "19F=3Y1D";
	private String host = "144.122.71.133";
	private String database = "phw1_e1848324";
	private int port = 3306;
	
	private Connection con;
	
	public IShoppingObj (String newDate, double newAmount, String newUserID, String newStoreName, Category newStoreCategory) {
		date = newDate;
		amount = newAmount;
		userID = newUserID;
		storeName = newStoreName;
		storeCategory = newStoreCategory;
		
		
		String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database;
		
		try {
			Class.forName ("com.mysql.jdbc.Driver");
			this.con =  DriverManager.getConnection (url, this.user, this.password);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	public IUser getUser() {
		ResultSet rs;
		String query = "select * from users where id = '" + userID + "';";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				String s_name = rs.getString ("name");
				String s_address = rs.getString ("address");
				
				return (IUser) new IUserObj (s_name, userID, s_address);
				
			}
		}
		catch (SQLException e) {
		}
		return null;
	}
	
	
	public IStore getStore() {
		return (IStore) new IStoreObj (storeName, storeCategory);
	}
	
	
	public String getDate() {
		return date;
	}
	
	
	public double getAmount() {
		return amount;
	}
}
