package com.edu.rest;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

/**
 * 跳转页面的controller
 * @author 姜振炜
 */
@Controller
public class TestController {

    /**
     * 废数据
     * @return java.lang.String
     */
    @GetMapping("/goMessage")
    public String goMessage(){
        return "message";
    }

    /**
     * 湿度
     * @return java.lang.String
     */
    @GetMapping("/goHumidity")
    public String goHumidity(){
        return "humidity";
    }

    /**
     * 温度
     * @return java.lang.String
     */
    @GetMapping("/goTemperature")
    public String goTemperature(){
        return "index";
    }

    /**
     * 二氧化碳
     * @return java.lang.String
     */
    @GetMapping("/goCarbonDioxide")
    public String goCarbonDioxide(){
        return "carbonDioxide";
    }

    /**
     * 火焰
     * @return java.lang.String
     */
    @GetMapping("/foFlame")
    public String goFlame(){
        return "flame";
    }

    /**
     * 人体
     * @return java.lang.String
     */
    @GetMapping("/goHumanBody")
    public String goHumanBody(){
        return "humanBody";
    }

    /**
     * 烟雾
     * @return java.lang.String
     */
    @GetMapping("/goSmoke")
    public String goSmoke(){
        return "smoke";
    }

    /**
     * 光照
     * @return java.lang.String
     */
    @GetMapping("/goLight")
    public String goLight(){
        return "light";
    }

    /**
     * 有机物
     * @return java.lang.String
     */
    @GetMapping("/goOrganicMatter")
    public String goOrganicMatter(){
        return "organicMatter";
    }

    /**
     * CO
     * @return java.lang.String
     */
    @GetMapping("/goCarbonMonoxide")
    public String goCarbonMonoxide(){
        return "carbonMonoxide";
    }

    /**
     * MQ有毒气体
     * @return java.lang.String
     */
    @GetMapping("/goMqPoisonGas")
    public String goMqPoisonGas(){
        return "mqPoisonGas";
    }

    /**
     * ElectricityDA
     * @return java.lang.String
     */
    @GetMapping("/goElectricityDA")
    public String goElectricityDA(){
        return "electricityDA";
    }
    /**
     * ElectricityDB
     * @return java.lang.String
     */
    @GetMapping("/goElectricityDB")
    public String goElectricityDB(){
        return "electricityDB";
    }
    /**
     * ElectricityDA
     * @return java.lang.String
     */
    @GetMapping("/goElectricityDA1")
    public String goElectricityDA1(){
        return "electricityDA1";
    }
    /**
     * ElectricityDB
     * @return java.lang.String
     */
    @GetMapping("/goElectricityDB1")
    public String goElectricityDB1(){
        return "electricityDB1";
    }

    /**
     * voice
     * @return java.lang.String
     */
    @GetMapping("/goVoice")
    public String goVoice(){
        return "voice";
    }

    /**
     * Photosensitive
     * @return java.lang.String
     */
    @GetMapping("/goPhotosensitive")
    public String goPhotosensitive(){
        return "photosensitive";
    }
}
