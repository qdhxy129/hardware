package com.edu.unit;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

import java.util.List;
import java.util.stream.Collectors;

/**
 * 数据处理工具类
 *
 * @author 姜振炜
 * @date 2021/8/12 10:44
 */
public class DataProcessing {

    /**
     * 转换bean
     *
     * @param data  要转换的数据
     * @param clazz 转换到的类型
     * @return T
     */
    public static <T> T beanCopy(Object data, Class<T> clazz) {
        if (data == null) {
            throw new RuntimeException("复制bean中，bean参数没有值");
        }
        if (clazz == null) {
            throw new RuntimeException("bean转换时请显式指定目标bean的类型");
        }
        JSONObject object = JSON.parseObject(JSON.toJSONString(data));
        return JSON.parseObject(JSON.toJSONString(object), clazz);
    }

    /**
     * 转换list
     *
     * @param data  要转换的list
     * @param clazz 转换到的类型
     * @return java.util.List<T>
     */
    public static <T> List<T> listCopy(List<Object> data, Class<T> clazz) {
        return data.stream()
                .map(item -> beanCopy(item, clazz))
                .collect(Collectors.toList());
    }
}
