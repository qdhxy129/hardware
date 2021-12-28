package com.edu.biz;

import com.edu.dto.RealTimeDataDTO;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;

/**
 * @author yixiaowei
 * @date 2021/10/24 16:14
 */
@Service
public class GlobalBiz {
    @Resource
    private CarbonDioxideBiz carbonDioxideBiz;
    @Resource
    private CarbonMonoxideBiz carbonMonoxideBiz;
    @Resource
    private FlameBiz flameBiz;
    @Resource
    private HumanBodyBiz humanBodyBiz;
    @Resource
    private HumidityBiz humidityBiz;
    @Resource
    private LightBiz lightBiz;
    @Resource
    private MqPoisonGasBiz mqPoisonGasBiz;
    @Resource
    private OrganicMatterBiz organicMatterBiz;
    @Resource
    private PhotosensitiveBiz photosensitiveBiz;
    @Resource
    private SmokeBiz smokeBiz;
    @Resource
    private TemperatureBiz temperatureBiz;
    @Resource
    private VoiceBiz voiceBiz;
    @Resource
    private ElectricityDABiz electricityDABiz;
    @Resource
    private ElectricityDBBiz electricityDBBiz;
    @Resource
    private ElectricityDA1Biz electricityDA1Biz;
    @Resource
    private ElectricityDB1Biz electricityDB1Biz;


    public RealTimeDataDTO getRealTimeData() {
        RealTimeDataDTO realTimeDataDTO = new RealTimeDataDTO();
        realTimeDataDTO.setCarbonDioxide(carbonDioxideBiz.getNewData());
        realTimeDataDTO.setCarbonMonoxide(carbonMonoxideBiz.getNewData());
        realTimeDataDTO.setFlame(flameBiz.getNewData());
        realTimeDataDTO.setHumanBody(humanBodyBiz.getNewData());
        realTimeDataDTO.setHumidity(humidityBiz.getNewData());
        realTimeDataDTO.setElectricityDA(electricityDABiz.getNewData());
        realTimeDataDTO.setElectricityDB(electricityDBBiz.getNewData());
        realTimeDataDTO.setLight(lightBiz.getNewData());
        realTimeDataDTO.setVoice(voiceBiz.getNewData());
        realTimeDataDTO.setTemperature(temperatureBiz.getNewData());
        realTimeDataDTO.setSmoke(smokeBiz.getNewData());
        realTimeDataDTO.setMqPoisonGas(mqPoisonGasBiz.getNewData());
        realTimeDataDTO.setOrganicMatter(organicMatterBiz.getNewData());
        realTimeDataDTO.setPhotosensitive(photosensitiveBiz.getNewData());
        realTimeDataDTO.setElectricityDA1(electricityDA1Biz.getNewData());
        realTimeDataDTO.setElectricityDB1(electricityDB1Biz.getNewData());
        realTimeDataDTO.setSmokeList(smokeBiz.getSmokeList());
        realTimeDataDTO.setFlameList(flameBiz.getFLameList());
        realTimeDataDTO.setElectricityList(electricityDBBiz.getElectricityList());
        realTimeDataDTO.setTemHumDataList(temperatureBiz.getTemHumDataList());
        return realTimeDataDTO;
    }
}
