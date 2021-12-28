package com.edu.dto;

import com.edu.entity.*;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;
import java.util.Map;

/**
 * @author yixiaowei
 * @date 2021/10/24 16:03
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class RealTimeDataDTO {

    private CarbonDioxide carbonDioxide;

    private CarbonMonoxide carbonMonoxide;

    private Flame flame;

    private List<Flame> flameList;

    private HumanBody humanBody;

    private Humidity humidity;

    private Light light;

    private MqPoisonGas mqPoisonGas;

    private OrganicMatter organicMatter;

    private Photosensitive photosensitive;

    private Smoke smoke;

    private List<Smoke> smokeList;

    private Temperature temperature;

    private Voice voice;

    private ElectricityDA electricityDA;

    private ElectricityDB electricityDB;

    private ElectricityDA electricityDA1;

    private ElectricityDB electricityDB1;

    private Map electricityList;

    private Map temHumDataList;


}
