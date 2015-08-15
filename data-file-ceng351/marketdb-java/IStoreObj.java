package com.tauros;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;


public class IStoreObj implements IStore {
	private String name;
	private Category category;
	
	
	private String user = "e1848324";
	private String password = "19F=3Y1D";
	private String host = "144.122.71.133";
	private String database = "phw1_e1848324";
	private int port = 3306;
	
	private Connection con;
	
	
	public IStoreObj (String newName, Category newCategory) {
		name = newName;
		category = newCategory;
		
		
		
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
	
	public String getStoreName() {
		return name;
	}
	
	public Category getStoreCategory() {
		return category;
	}
	
	public void setStoreName (String newName) throws StoreAlreadyExists {
		ResultSet rs;
		String tempCategory = (this.getStoreCategory()).name();
		String tempName = this.getStoreName();
		String query = "select * from stores where name = '" + newName + "' and  category = '" + tempCategory + "';";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			if (rs.next()) {
				throw new StoreAlreadyExists();
			}
			query = "update stores set name = '" + newName + "' where name = '" + tempName + "' and category = '" + tempCategory + "';";
			st.executeUpdate (query);
			
		}
		catch (SQLException e) {
		}
	}
	
	public void setStoreCategory (Category newCategory) throws StoreAlreadyExists {
		ResultSet rs;
		String tempCategory = (this.getStoreCategory()).name();
		String tempName = this.getStoreName();
		String query = "select * from stores where name = '" + tempName + "' and  category = '" + newCategory + "';";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			if(rs.next())
			{
				throw new StoreAlreadyExists();
			}
			query = "update stores set category = '" + newCategory + "' where name = '" + tempName + "' and category = '" + tempCategory + "';";
			st.executeUpdate (query);
		}
		catch (SQLException e) {
			
		}
	}
	
	public IShopping[] getShoppings (String startDate, String endDate) {
		SimpleDateFormat oldFormat = new SimpleDateFormat("dd.MM.yyyy"); 
		Date tempDate;
		try {
			tempDate = oldFormat.parse (startDate);
			SimpleDateFormat newFormat = new SimpleDateFormat("yyyy-MM-dd");
			String newStartDate = newFormat.format(tempDate);
			try {
				tempDate = oldFormat.parse (endDate);
				String newEndDate = newFormat.format (tempDate);
				
				
				ResultSet rs;
				IShopping [] resultArray = new IShopping[0];
				String query = "select * from shoppings where name = '" + name + "' and shop <= '" + newEndDate + "' and shop >= '" + newStartDate + "' order by shop;";
				
				try {
					Statement st = this.con.createStatement();
					rs = st.executeQuery (query);
					while (rs.next()) {
						IShopping [] temp = new IShopping[resultArray.length + 1];
						String s_shop = rs.getString ("shop");
						double s_amount = rs.getDouble ("amount");
						String s_id = rs.getString ("id");
						String s_name = rs.getString ("name");
						String s_category = rs.getString ("category");
						
						IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, s_id, s_name, Category.valueOf(s_category));
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
	
	public IShopping[] getShoopings (String date) {
		SimpleDateFormat oldFormat = new SimpleDateFormat("dd.MM.yyyy"); 
		Date tempDate;
		try {
			tempDate = oldFormat.parse (date);
			SimpleDateFormat newFormat = new SimpleDateFormat("yyyy-MM-dd");
			String newDate = newFormat.format(tempDate);
			
			
			ResultSet rs;
			IShopping [] resultArray = new IShopping[0];
			String query = "select * from shoppings where name = '" + name + "' and shop = '" + newDate + "' order by shop;";
			
			try {
				Statement st = this.con.createStatement();
				rs = st.executeQuery (query);
				while (rs.next()) {
					IShopping [] temp = new IShopping[resultArray.length + 1];
					String s_shop = rs.getString ("shop");
					double s_amount = rs.getDouble ("amount");
					String s_id = rs.getString ("id");
					String s_name = rs.getString ("name");
					String s_category = rs.getString ("category");
					
					IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, s_id, s_name, Category.valueOf(s_category));
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
		return null;
	}
	
	
	public IShopping[] getShoppings() {
		ResultSet rs;
		IShopping [] resultArray = new IShopping[0];
		String query = "select * from shoppings where name = '" + name + "' order by shop;";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IShopping [] temp = new IShopping[resultArray.length + 1];
				String s_shop = rs.getString ("shop");
				double s_amount = rs.getDouble ("amount");
				String s_id = rs.getString ("id");
				String s_name = rs.getString ("name");
				String s_category = rs.getString ("category");
				
				IShopping newShopping = (IShopping) new IShoppingObj (s_shop, s_amount, s_id, s_name, Category.valueOf(s_category));
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
}
