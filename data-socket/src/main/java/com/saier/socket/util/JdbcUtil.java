package com.saier.socket.util;


import com.saier.socket.domain.Electricity;

import java.sql.*;

public class JdbcUtil {
    static {
        //加载驱动
        try {
            Class.forName("com.mysql.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

    }

    public static Connection getconn() {
        //创建一个连接对象
        Connection conn = null;
        try {
            // v6
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/hardware?useUnicode=true&characterEncoding=utf8&useSSL=false", "root", "");
            //conn = DriverManager.getConnection("jdbc:mysql://8.136.240.54:3306/hardware?useUnicode=true&characterEncoding=utf8&useSSL=false", "root", "QZskools003+");
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return conn;

    }

    public static void executing(String sql, Object[] params) {
        Connection conn = JdbcUtil.getconn();

        //准备SQL
        PreparedStatement ps = null;

        //insert into user(''''''','') value(?, ?, ?)
        try {
            //准备SQL
            ps = conn.prepareStatement(sql);
            for (int i = 0; i < params.length; i++) {
                ps.setObject(i + 1, params[i]);
            }
            ps.execute();
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } finally {
            JdbcUtil.closeall(null, ps, conn);
        }
    }

    public static void closeall(ResultSet rs, PreparedStatement ps, Connection conn) {
        try {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
            if (conn != null) {
                conn.close();
            }
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }


    public static Electricity selectFirstElectricityByEpc(String epc) {
        Electricity electricity = new Electricity();
        Connection conn = JdbcUtil.getconn();
        //准备SQL
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select * from electricity where epc = ? and temperature is null order by createtime";
        try {
            ps = conn.prepareStatement(sql);
            ps.setString(1, epc);
            rs = ps.executeQuery();
            if (rs.next()) {
				assignment(rs, electricity);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            JdbcUtil.closeall(null, ps, conn);
        }
        return electricity;
    }

	private static void assignment(ResultSet rs, Electricity electricity) throws SQLException {
        electricity.setId(rs.getInt("id"));
        electricity.setVoltage(rs.getString("voltage"));
        electricity.setCurrent(rs.getString("current"));
        electricity.setPower(rs.getString("power"));
        electricity.setEmissions(rs.getString("emissions"));
        electricity.setPowerFactor(rs.getString("powerFactor"));
        electricity.setFrequency(rs.getString("frequency"));
        electricity.setTemperature(rs.getString("temperature"));
        electricity.setTotalBattery(rs.getString("totalBattery"));
        electricity.setEpc(rs.getString("epc"));
        electricity.setCreatetime(rs.getDate("createtime"));
        electricity.setIp(rs.getString("ip"));
    }
}
