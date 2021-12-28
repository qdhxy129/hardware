package com.saier.socket.main;

import com.saier.socket.util.SocketUtils;

/**
 * @author yixiaowei
 * @date 2021/12/12 21:23
 */
public class SocketMain {
    public static void main(String[] args) {
        SocketUtils.openServer(8080);
    }
}
