package com.edu.unit;

import com.edu.handler.NettyClientHandler;
import io.netty.bootstrap.Bootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import lombok.extern.slf4j.Slf4j;

/**
 * 给指定ip、端口发送数据
 * @author yixiaowei
 * @date 2021/8/22 13:32
 */
@Slf4j
public class SocketSendUtil {


    /**
     * 给指定的ip地址 + 端口 发送消息
     * @author yixiaowei
     * @date 13:36 2021/8/22
     * @param ipAddress  ip地址
     * @param port 端口号
     * @param msg 发送消息
     * @return void
     */
    public static Message sendInfo(String ipAddress, Integer port, String msg){


        Message message = new Message();
        //客户端需要一个事件循环组
        EventLoopGroup group = new NioEventLoopGroup();


        try {
            //创建客户端启动对象
            //注意客户端使用的不是 ServerBootstrap 而是 Bootstrap
            Bootstrap bootstrap = new Bootstrap();

            //设置相关参数
            bootstrap.group(group) //设置线程组
                    .channel(NioSocketChannel.class) // 设置客户端通道的实现类(反射)
                    .handler(new ChannelInitializer<SocketChannel>() {
                        @Override
                        protected void initChannel(SocketChannel ch) throws Exception {
                            ch.pipeline().addLast(new NettyClientHandler(msg)); //加入自己的处理器
                        }
                    });

            log.info("客户端 ok..");

            //启动客户端去连接服务器端
            //关于 ChannelFuture 要分析，涉及到netty的异步模型
            ChannelFuture channelFuture = bootstrap.connect(ipAddress, port).sync();
            if (channelFuture.isSuccess()) {
                group.shutdownGracefully();
                // 设置返回成功的消息
                message.success();
            }
            //给关闭通道进行监听
            channelFuture.channel().closeFuture().sync();
        }catch (Exception e){
            log.info("服务端未开启端口监听 或 可能某些原因导致连接失败");
        }finally {
            group.shutdownGracefully();
        }

        return message;
    }
}
