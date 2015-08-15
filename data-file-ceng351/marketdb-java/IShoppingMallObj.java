package com.tauros;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.tauros.IStore;
import com.tauros.IUser;
import com.tauros.Category;
import com.tauros.DatabaseAlreadyCreated;
import com.tauros.StoreAlreadyExists;
import com.tauros.StoreNotExist;
import com.tauros.UserAlreadyExists;
import com.tauros.UserNotExist;

public class IShoppingMallObj implements IShoppingMall {
	private String user = "e1848324";
	private String password = "19F=3Y1D";
	private String host = "144.122.71.133";
	private String database = "phw1_e1848324";
	private int port = 3306;
	
	private Connection con;
	
	public IShoppingMallObj() {
		this.onStart();
	}
	
	public void onStart() {
		String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database;
		
		try {
			Class.forName ("com.mysql.jdbc.Driver");
			this.con =  DriverManager.getConnection (url, this.user, this.password);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		try {
			createDatabase();
		}
		catch (DatabaseAlreadyCreated e) {
			
		}
	}
	
	public boolean createDatabase() throws DatabaseAlreadyCreated {
		boolean result;
		String query = "create table stores (" +
						"name varchar (100)," +
						"category varchar (13)," +
						"primary key (name, category));";
		try {
			Statement st = this.con.createStatement();
			result = (st.executeUpdate(query)) != 0;	
		}
		catch (SQLException e) {
				throw new DatabaseAlreadyCreated();
		}
		query = "create table users (" +
				"name varchar (100)," +
				"id varchar (11) not null," +
				"address varchar (500)," +
				"primary key (id));";
		try {
			Statement st = this.con.createStatement();
			result = result || ((st.executeUpdate(query)) != 0);
		}
		catch (SQLException e) {
				throw new DatabaseAlreadyCreated();
		}
		query = "create table shoppings (" +
				"shop date," +
				"amount real," +
				"id varchar (11) not null," +
				"name varchar (100)," +
				"category varchar (13)," +
				"primary key (name, category, id, shop)," +
				"foreign key (name, category) references stores (name, category)," +
				"foreign key (id) references users (id));";
		try {
			Statement st = this.con.createStatement();
			result = result || ((st.executeUpdate(query)) != 0);
		}
		catch (SQLException e) {
				throw new DatabaseAlreadyCreated();
		}
		return result;
	}
	
	public void addStore (String storeName, Category category) throws StoreAlreadyExists {
		String query = "insert into stores values ('" + storeName + "','" + category + "');";
		
		try {
			Statement st = this.con.createStatement();
			st.executeUpdate(query);
		}
		catch (SQLException e) {
			throw new StoreAlreadyExists();
		}
	}
	
	public void removeStore (IStore store) throws StoreNotExist {
		String name = store.getStoreName();
		String query = "delete from stores where name = '" + name +"';";
		
		try {
			Statement st = this.con.createStatement();
			st.executeUpdate (query);
		}
		catch (SQLException e) {
			throw new StoreNotExist();
		}
	}
	
	
	public void registerUser (String fullname, String id, String address) throws UserAlreadyExists {
		String query = "insert into users values ('" + fullname + "','" + id + "','" + address + "');";
		
		try {
			Statement st = this.con.createStatement();
			st.executeUpdate (query);
		}
		catch (SQLException e) {
			throw new UserAlreadyExists();
		}
	}

	
	public void unregisterUser (IUser user) throws UserNotExist {
		String name = user.getFullName();
		String query = "delete from users where name = '" + name +"';";
		
		try {
			Statement st = this.con.createStatement();
			st.executeUpdate (query);
			
		}
		catch (SQLException e) {
			throw new UserNotExist();
		}
	}
	
	
	public IStore[] searchStore (String nameIncludes) {
		ResultSet rs;
		IStore [] resultArray = new IStore[0];
		String query = "select * from stores where name like '%" + nameIncludes + "%' order by name;";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IStore [] temp = new IStore[resultArray.length + 1];
				String s_name = rs.getString ("name");
				String s_category = rs.getString ("category");
				
				IStore newStore = (IStore) new IStoreObj (s_name, Category.valueOf(s_category));
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newStore;
				resultArray = temp;
			}
			return resultArray;
			
		}
		catch (SQLException e) {
			return null;
		}
	}

	public IStore[] searchStore (String nameIncudes, Category category) {
		ResultSet rs;
		IStore [] resultArray = new IStore[0];
		String query = "select * from stores where name like '%" + nameIncudes + "%' and category = '" + category + "' order by name;";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IStore [] temp = new IStore[resultArray.length + 1];
				String s_name = rs.getString ("name");
				String s_category = rs.getString ("category");
				
				IStore newStore = (IStore) new IStoreObj (s_name, Category.valueOf(s_category));
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newStore;
				resultArray = temp;
			}
			return resultArray;
			
		}
		catch (SQLException e) {
			return null;
		}
	}


	public IStore[] searchStore (Category category) {
		ResultSet rs;
		IStore [] resultArray = new IStore[0];
		String query = "select * from stores where category = '" + category + "' order by name;";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IStore [] temp = new IStore[resultArray.length + 1];
				String s_name = rs.getString ("name");
				String s_category = rs.getString ("category");
				
				IStore newStore = (IStore) new IStoreObj (s_name, Category.valueOf(s_category));
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newStore;
				resultArray = temp;
			}
			return resultArray;
			
		}
		catch (SQLException e) {
			return null;
		}
	}

	
	public IUser[] searchUser (String nameIncludes) {
		ResultSet rs;
		IUser [] resultArray = new IUser[1];
		String query = "select * from users where name like '%" + nameIncludes + "%' order by name;";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				IUser [] temp = new IUser[resultArray.length + 1];
				String s_name = rs.getString ("name");
				String s_id = rs.getString ("id");
				String s_address = rs.getString ("address");
				
				IUser newUser = (IUser) new IUserObj (s_name, s_id, s_address);
				for (int i = 0; i < resultArray.length; ++i) {
					temp[i] = resultArray[i];
				}
				temp[resultArray.length] = newUser;
				resultArray = temp;
			}
			return resultArray;
			
		}
		catch (SQLException e) {
			return null;
		}	
	}
	
	
	public IUser getUser (String id) {
		ResultSet rs;
		String query = "select * from users where id = '" + id + "';";
		
		try {
			Statement st = this.con.createStatement();
			rs = st.executeQuery (query);
			while (rs.next()) {
				String s_name = rs.getString ("name");
				String s_id = rs.getString ("id");
				String s_address = rs.getString ("address");
				
				return (IUser) new IUserObj (s_name, s_id, s_address);
				
			}
		}
		catch (SQLException e) {
			
		}
		return null;
	}
}
