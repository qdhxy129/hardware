package com.saier.socket.util;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import java.nio.charset.Charset;

/**
 * @author 26082
 */
public class NettyServerHandler extends ChannelInboundHandlerAdapter {

    // 接收数据
    String str ;


    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        // 将 msg 转成一个 ByteBuf
        // ByteBuf 是 Netty 提供的，不是 NIO 的 ByteBuffer.
        ByteBuf buf = (ByteBuf) msg;
        str = buf.toString(Charset.forName("GB2312"));
        // 接收数据后存入数据库中
        SocketUtils.receiveIntoData(str, ctx.channel().remoteAddress() );
        if ("QQ".equals(str.substring(0, 2))) {
            StringBuilder abnormalResult = SocketUtils.getAbnormalResult(SocketUtils.abnormalFlag);
            ctx.writeAndFlush(Unpooled.copiedBuffer(abnormalResult.toString(), Charset.forName("GB2312")));
        }
        if ("KZ".equals(str.substring(0, 2))) {
            StringBuilder switchResult = SocketUtils.getSwitchResult(SocketUtils.switchControlFlag);
            ctx.writeAndFlush(Unpooled.copiedBuffer(switchResult.toString(), Charset.forName("GB2312")));
        }
        // 关闭连接
        ctx.close().sync();
    }

    //数据读取完毕
    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
        //if (responseFlag) {
        //    ctx.writeAndFlush(Unpooled.copiedBuffer("DD000000000000SP", Charset.forName("GB2312")));
        //    responseFlag = false;
        //}
    }

    //处理异常, 一般是需要关闭通道

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        ctx.close();
    }
}
