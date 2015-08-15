package com.tauros;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class IUserObj implements IUser {
	private String name;
	private String id;
	private String address;
	
	
	
	private String user = "e1848324";
	private String password = "19F=3Y1D";
	private String host = "144.122.71.133";
	private String database = "phw1_e1848324";
	private int port = 3306;
	
	private Connection con;

	
	public IUserObj (String newName, String newId, String newAddress) {
		name = newName;
		id = newId;
		address = newAddress;
		
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
	
	public String getFullName() {
		return name;
	}
	
	public String getId() {
		return id;
	}
	
	public String getAddress() {
		return address;
	}
	
	public void updateAddress (String newAddress) {
		String query = "update users set address = '" + newAddress + "' where name = '" + name + "' and id = '" + id + "' and address = '" + address + "';";
         
		try {
			Statement st = this.con.createStatement();
			st.executeQuery(query);
		}
			
		catch (SQLException e) {
			
		}
	}
	
	public void shopped (IStore store, String date, double amount) {
		SimpleDateFormat oldFormat = new SimpleDateFormat("dd.MM.yyyy"); 
		Date tempDate;
		try {
			tempDate = oldFormat.parse (date);
			SimpleDateFormat newFormat = new SimpleDateFormat("yyyy-MM-dd");
			String newDate = newFormat.format(tempDate);
			String query = "insert into shoppings values ('" + newDate + "','" + amount + "','" + id + "','" + store.getStoreName() + "','" + (store.getStoreCategory()).name() + "');";
			
			try {
				Statement st = this.con.createStatement();
				st.executeUpdate (query);
			}
			catch (SQLException e) {
				
			}
		}
		catch (ParseException e1) {
			
		}
	}
	
	
	public IShopping[] getShoppings (IStore store, String startDate, String endDate) {
		SimpleDateFormat oldFormat = new SimpleDateFormat("dd.MM.yyyy"); 
		Date tempDate;
		try {
			tempDate = oldFormat.parse (startDate);
			SimpleDateFormat newFormat = new SimpleDateFormat("yyyy-MM-dd");
			String newStartDate = newFormat.format(tempDate);
			try {
				tempDate = oldFormat.parse (endDate);
				String newEndDate = newFormat.format (tempDate);
				
				IShopping [] resultArray = new IShopping[0];
				ResultSet rs;
				String query = "select * from shoppings where id = '" + id + "' and name = '" + store.getStoreName() + "' and category = '" + (store.getStoreCategory()).name() + "' and shop >= '" + newStartDate + "' and shop <= '" + newEndDate + "' order by shop;";
				try {
					Statement st = this.con.createStatement();
					rs = st.executeQuery (query);
					while (rs.next()) {
						IShopping [] temp = new IShopping[resultArray.length + 1];
						String s_shop = rs.getString ("shop");
						double s_amount = rs.getDouble ("amount");
						
						
						IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, id, store.getStoreName(), store.getStoreCategory());
						for (int i = 0; i < resultArray.length; ++i) {
							temp[i] = resultArray[i];
						}
						temp[resultArray.length] = newShopping;
						resultArray = temp;
					}
					return resultArray;
				}
				catch (SQLException e) {
					
				}
			}
			catch (ParseException e1) {
				
			}
		}
		catch (ParseException e2) {

		}
		return null;
	}
	
	public IShopping[] getShoppings (IStore store) {
		IShopping [] resultArray = new IShopping[0];
		ResultSet rs;
		String query = "select * from shoppings where id = '" + id + "' and name = '" + store.getStoreName() + "' and category = '" + (store.getStoreCategory()).name() + "' order by shop;";
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IShopping [] temp = new IShopping[resultArray.length + 1];
				String s_shop = rs.getString ("shop");
				double s_amount = rs.getDouble ("amount");
				
				
				IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, id, store.getStoreName(), store.getStoreCategory());
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newShopping;
				resultArray = temp;
			}
			return resultArray;
		}
		catch (SQLException e) {
			return null;
		}
	}
	public IShopping[] getShoppings() {
		IShopping [] resultArray = new IShopping[0];
		ResultSet rs;
		String query = "select * from shoppings where id = '" + id + "' order by shop;";
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IShopping [] temp = new IShopping[resultArray.length + 1];
				String s_shop = rs.getString ("shop");
				double s_amount = rs.getDouble ("amount");
				String s_name = rs.getString ("name");
				String s_category = rs.getString ("category");
				
				IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, id, s_name, Category.valueOf (s_category));
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newShopping;
				resultArray = temp;
				return resultArray;
			}
			return resultArray;
		}
		catch (SQLException e) {
			return null;
		}
	}
	
	
	public boolean checkPrimeMinisterOffer() {
		ResultSet rs;
		String query = "select distinct name from shoppings where id = '" + id + "' and shop <= curdate() and shop >= subdate(curdate(), 30) and amount >= 100;";
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			int count = 0;
			while (rs.next()) {
				++count;
			}
			if (count >= 5) {
				return true;
			}
		}
		catch (SQLException e) {
			return false;
		}
		return false;
	}
	
	
	public boolean checkGovernorOffer() {
		ResultSet rs;
		String query = "select distinct name, count(distinct shop) from shoppings where id = '" + id + "' and shop <= curdate() and shop >= subdate(curdate(), 30) and amount >= 50 group by name having count(distinct shop) > 4;";
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			int count = 0;
			while (rs.next()) {
				++count;
			}
			if (count > 0) {
				return true;
			}
		}
		catch (SQLException e) {
			return false;
		}
		return false;
	}
	
	
	public boolean checkMayorOffer() {
		ResultSet rs;
		String query = "select distinct name, count(distinct shop) from shoppings where id = '" + id + "' and shop <= curdate() and shop >= subdate(curdate(), 30) group by name having count(distinct shop) > 10;";
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			int count = 0;
			while (rs.next()) {
				++count;
			}
			if (count > 0) {
				return true;
			}
		}
		catch (SQLException e) {
			return false;
		}
		return false;		
	}
	
	
}
