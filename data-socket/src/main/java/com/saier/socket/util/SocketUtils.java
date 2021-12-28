package com.saier.socket.util;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.timeout.ReadTimeoutHandler;
import io.netty.handler.timeout.WriteTimeoutHandler;
import lombok.extern.slf4j.Slf4j;

import java.net.SocketAddress;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;


public class SocketUtils {

    /**
     * 校验数据的标志
     */
    private static String flag;

    /**
     * 需要保存到数据库中的当前时间
     */
    private static Timestamp goodsC_date;

    /**
     * 异常标识
     */
    public static String[] abnormalFlag = new String[12];

    /**
     * 开关控制标识
     */
    public static String[] switchControlFlag = new String[]{"GD", "GM", "GFS", "GCL", "GC"};


    /**
     * 服务端开启服务
     * 所需条件：端口号
     */
    public static void openServer(int port) {

        EventLoopGroup bossGroup = new NioEventLoopGroup(3);
        EventLoopGroup workerGroup = new NioEventLoopGroup(); //8

        try {
            //创建服务器端的启动对象，配置参数
            ServerBootstrap bootstrap = new ServerBootstrap();

            //使用链式编程来进行设置
            bootstrap.group(bossGroup, workerGroup) //设置两个线程组
                    .channel(NioServerSocketChannel.class) //使用NioSocketChannel 作为服务器的通道实现
                    .option(ChannelOption.SO_BACKLOG, 128) // 设置线程队列得到连接个数
                    .childOption(ChannelOption.SO_KEEPALIVE, true) //设置保持活动连接状态
//                    .handler(null) // 该 handler对应 bossGroup , childHandler 对应 workerGroup
                    .childHandler(new ChannelInitializer<SocketChannel>() {//创建一个通道初始化对象(匿名对象)
                        //给pipeline 设置处理器
                        @Override
                        protected void initChannel(SocketChannel ch) throws Exception {
                            System.out.println("客户socketchannel hashcode=" + ch.hashCode()); //可以使用一个集合管理 SocketChannel， 再推送消息时，可以将业务加入到各个channel 对应的 NIOEventLoop 的 taskQueue 或者 scheduleTaskQueue
                            ch.pipeline().addLast(new NettyServerHandler());
                            ch.pipeline().addLast(new ReadTimeoutHandler(2));
                            ch.pipeline().addLast(new WriteTimeoutHandler(2));
                        }
                    }); // 给我们的workerGroup 的 EventLoop 对应的管道设置处理器

            System.out.println("...服务器 is ready...");

            //绑定一个端口并且同步, 生成了一个 ChannelFuture 对象
            //启动服务器(并绑定端口)
            ChannelFuture cf = bootstrap.bind(port).sync();

            //给cf 注册监听器，监控我们关心的事件

            cf.addListener(new ChannelFutureListener() {
                @Override
                public void operationComplete(ChannelFuture future) throws Exception {
                    if (cf.isSuccess()) {
                        System.out.println("监听端口 6668 成功");
                    } else {
                        System.out.println("监听端口 6668 失败");
                    }
                }
            });

            //对关闭通道进行监听
            cf.channel().closeFuture().sync();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }

    }

    /**
     * 接收数据，并存入数据库中
     */
    public static void receiveIntoData(String data, SocketAddress ip) {

        // 进行组合数据的校验
        flag = data.substring(0, 2);

        // 获得系统时间
        Date date = new Date();

        String nowTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(date);

        // 时间转换
        goodsC_date = Timestamp.valueOf(nowTime);

        switch (flag) {
            case "01":
                insertTemperatureHumidity(data, ip);
                break;
            case "03":
                insertCarbonDioxideLight(data, ip);
                break;
            case "05":
                insertBodyVoicePhotosensitive(data, ip);
                break;
            case "06":
                insertSmokeFlameCOMq(data, ip);
                break;
            case "DA":
                insertElectricityDa(data, ip);
                break;
            case "DB":
                insertElectricityDb(data, ip);
                break;
            default:
                insertDiscardedData(data, ip);
        }
    }

    /**
     * 电量监控，DB数据保存
     * epc，温度，总电量，二氧化碳排量
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertElectricityDb(String data, SocketAddress ip) {
        String sql = "insert into electricityDB (id,epc,temperature,totalBattery,emissions,createtime,ip)" +
                " values(null,?,?,?,?,?,?) ";
        Object[] params = {
                // epc
                data.substring(2, 26),
                // 温度
                data.substring(26, 31),
                // 总电量，前有五个空格
                data.substring(36, 41),
                // 二氧化碳排量，前有五个空格
                data.substring(46, 52),
                goodsC_date,
                ip.toString()
        };
        JdbcUtil.executing(sql, params);
    }

    /**
     * 电量监控，DA数据 保存
     * epc，电压，电流，有功功率，功率因数，频率
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertElectricityDa(String data, SocketAddress ip) {
        String sql = "insert into electricityDA (id,epc,voltage,current,power,powerFactor,frequency,createtime,ip)" +
                " values(null,?,?,?,?,?,?,?,?) ";
        Object[] params = {
                // epc
                data.substring(2, 26),
                // 电压
                data.substring(26, 34),
                // 电流，前有两个空格
                data.substring(36, 42),
                // 有功功率，前有两个空格
                data.substring(44, 51),
                // 功率因数
                data.substring(51, 56),
                // 频率
                data.substring(56, 61),
                goodsC_date,
                ip.toString()
        };
        JdbcUtil.executing(sql, params);
    }

    /**
     * 不满足格式的数据保存
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertDiscardedData(String data, SocketAddress ip) {
        String sql = "insert into message values(null,?,?,?)";
        Object[] params = {
                data,
                goodsC_date,
                ip.toString()
        };
        JdbcUtil.executing(sql, params);
    }


    /**
     * 烟雾 火焰 CO MQ135 组合数据保存
     *
     * @return void
     * @author yixiaowei
     * @date 9:22 2021/9/11
     */
    private static void insertSmokeFlameCOMq(String data, SocketAddress ip) {
        // 校验
        if (!checkData(data)) {
            return;
        }

        // 拆分数据
        String smoke = data.substring(2, 3);
        String flame = data.substring(5, 6);
        String co = data.substring(8, 13);
        String mq = data.substring(15, 22);
        Object[] coParams = {
                co,
                goodsC_date,
                ip.toString()
        };
        Object[] mqParams = {
                mq,
                goodsC_date,
                ip.toString()
        };
        Object[] smokeParams = {
                smoke,
                goodsC_date,
                ip.toString()
        };
        Object[] flameParams = {
                flame,
                goodsC_date,
                ip.toString()
        };
        String sqlCo = "insert into carbonMonoxide values(null,?,?,?)";
        String sqlMq = "insert into mqPoisonGas values(null,?,?,?)";
        String sqlSmoke = "insert into smoke values(null,?,?,?)";
        String sqlFlame = "insert into flame values(null,?,?,?)";
        JdbcUtil.executing(sqlMq, mqParams);
        JdbcUtil.executing(sqlCo, coParams);
        JdbcUtil.executing(sqlSmoke, smokeParams);
        JdbcUtil.executing(sqlFlame, flameParams);
        handleExceptionInfo("mq", mq);
        handleExceptionInfo("co", co);
        handleExceptionInfo("flame", flame);
        handleExceptionInfo("smoke", smoke);
    }

    /**
     * 对data进行校验
     *
     * @param data 校验的数据
     * @return boolean 校验通过返回true
     */
    private static boolean checkData(String data) {
//        todo 暂时 先不做校验
        return true;
    }

    /**
     * 二氧化碳、光照、有机物组合数据保存
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertCarbonDioxideLight(String data, SocketAddress ip) {
        // 校验
        if (!checkData(data)) {
            return;
        }
        // 对数据进行拆分
        String carbonDioxide = data.substring(2, 7);
        String light = data.substring(9, 14);
        String organicMatter = data.substring(14, 19);
        Object[] carbonDioxideParams = {
                carbonDioxide,
                goodsC_date,
                ip.toString()
        };
        Object[] lightParams = {
                light,
                goodsC_date,
                ip.toString()
        };
        Object[] organicMatterParams = {
                organicMatter,
                goodsC_date,
                ip.toString()
        };
        String sqlCarbonDioxide = "insert into carbonDioxide values(null,?,?,?)";
        String sqlLight = "insert into light values(null,?,?,?)";
        String sqlOrganicMatter = "insert into organicMatter values(null,?,?,?)";
        JdbcUtil.executing(sqlCarbonDioxide, carbonDioxideParams);
        JdbcUtil.executing(sqlLight, lightParams);
        JdbcUtil.executing(sqlOrganicMatter, organicMatterParams);
        handleExceptionInfo("organicMatter", organicMatter);
        handleExceptionInfo("light", light);
        handleExceptionInfo("carbonDioxide", carbonDioxide);
    }


    /**
     * 温湿度类型的组合数据保存
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertTemperatureHumidity(String data, SocketAddress ip) {
        // 校验
        if (!checkData(data)) {
            return;
        }
        // 对数据进行拆分
        String temperature = data.substring(2, 4);
        String humidity = data.substring(6, 8);
        Object[] temperatureParams = {
                temperature,
                goodsC_date,
                ip.toString()
        };
        Object[] humidityParams = {
                humidity,
                goodsC_date,
                ip.toString()
        };
        String sqlTemperature = "insert into temperature values(null,?,?,?)";
        String sqlHumidity = "insert into humidity values(null,?,?,?)";
        JdbcUtil.executing(sqlTemperature, temperatureParams);
        JdbcUtil.executing(sqlHumidity, humidityParams);
        handleExceptionInfo("temperature", temperature);
        handleExceptionInfo("humidity", humidity);

    }

    /**
     * 温湿度类型的组合数据保存
     *
     * @param data 数据
     * @param ip   ip
     */
    private static void insertBodyVoicePhotosensitive(String data, SocketAddress ip) {
        // 校验
        if (!checkData(data)) {
            return;
        }
        // 对数据进行拆分
        String body = data.substring(2, 3);
        String voice = data.substring(5, 6);
        String photosensitive = data.substring(8, 9);

        Object[] bodyParams = {
                body,
                goodsC_date,
                ip.toString()
        };

        Object[] voiceParams = {
                voice,
                goodsC_date,
                ip.toString()
        };

        Object[] photosensitiveParams = {
                photosensitive,
                goodsC_date,
                ip.toString()
        };
        String sqlBody = "insert into humanBody values(null,?,?,?)";
        String sqlVoice = "insert into voice values(null,?,?,?)";
        String sqlPhotosensitive = "insert into photosensitive values(null,?,?,?)";
        JdbcUtil.executing(sqlBody, bodyParams);
        JdbcUtil.executing(sqlVoice, voiceParams);
        JdbcUtil.executing(sqlPhotosensitive, photosensitiveParams);
        handleExceptionInfo("humanBody", body);
        handleExceptionInfo("voice", voice);
        handleExceptionInfo("photosensitive", photosensitive);

    }


    /**
     * 处理异常数据
     */
    public static void handleExceptionInfo(String dataType, String data) {
        // 类型转换
        Double receiveData = Double.valueOf(data);

        // 定义当前时间毫秒数
        long currentTimeMillis = System.currentTimeMillis();
        switch (dataType) {
            case "temperature":
                abnormalFlag[0] = receiveData > 30 ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "humidity":
                abnormalFlag[1] = receiveData > 70 ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "smoke":
                abnormalFlag[5] = data.equals("1") ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "flame":
                abnormalFlag[6] = data.equals("1") ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "mq":
                abnormalFlag[8] = receiveData > 500 ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "light":
                abnormalFlag[3] = receiveData > 4000 ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "co":
                abnormalFlag[7] = receiveData > 70 ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "humanBody":
                abnormalFlag[4] = data.equals("1") ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "voice":
                abnormalFlag[9] = data.equals("1") ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;
            case "photosensitive":
                abnormalFlag[10] = data.equals("1") ? "1" + currentTimeMillis : "0" + currentTimeMillis;
                break;

        }
    }


    /**
     * 获取异常数据
     */
    public static StringBuilder getAbnormalResult(String[] abnormalFlag) {
        // 获取当前时间毫秒数
        Long currentTimeMillis = System.currentTimeMillis();

        // 创建StringBuilder对象
        StringBuilder sb = new StringBuilder("DD");

        for (int i = 0; i < abnormalFlag.length; i++) {
            if (abnormalFlag[i] == null || abnormalFlag[i].length() < 1) {
                sb.append("0");
                continue;
            }
            // 获取发送数据时的时间毫秒数
            Long oldTimeMillis = Long.valueOf(abnormalFlag[i].substring(1));
            // 获取五分钟内最新数据 超时则归零
            sb.append(currentTimeMillis - oldTimeMillis > 60000 * 5 ? "0" : abnormalFlag[i].substring(0, 1));
        }
        sb.append("SP");
        return sb;
    }


    /**
     * 处理开关数据
     */
    public static void handleSwitchInfo(String swichData) {
        if ("KD".equals(swichData) || "GD".equals(swichData)) {
            switchControlFlag[0] = swichData;
            return;
        }
        if ("KM".equals(swichData) || "GM".equals(swichData)) {
            switchControlFlag[1] = swichData;
            return;
        }
        if ("KFS".equals(swichData) || "GFS".equals(swichData)) {
            switchControlFlag[2] = swichData;
            return;
        }
        if ("KCL".equals(swichData) || "GCL".equals(swichData)) {
            switchControlFlag[3] = swichData;
            return;
        }
        if ("KC".equals(swichData) || "GC".equals(swichData)) {
            switchControlFlag[4] = swichData;
            return;
        }
    }

    /**
     * 获取开关数据
     */
    public static StringBuilder getSwitchResult(String[] switchControlFlag) {
        // 创建StringBuilder对象
        StringBuilder sb = new StringBuilder("KZ");

        for (int i = 0; i < switchControlFlag.length; i++) {
            // 获取五分钟内最新数据 超时则归零
            sb.append(switchControlFlag[i]);
        }
        sb.append("SP");
        return sb;
    }

}
