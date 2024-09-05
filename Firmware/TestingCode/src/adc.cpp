#include "adc.h"

ADC_HandleTypeDef hadc1;


uint16_t readADC(adc_channels channel, uint8_t timeout)
{
    ADC_ChannelConfTypeDef config = {0};
    uint16_t adc_val = 0;
    switch(channel)
    {
    case RF_RECEIVER:
        config.Channel = ADC_CHANNEL_9;
        break;

    case DL_RECEIVER:
        config.Channel = ADC_CHANNEL_8;
        break;

    case LF_RECEIVER:
        config.Channel = ADC_CHANNEL_7;
        break;

    case DR_RECEIVER:
        config.Channel = ADC_CHANNEL_6;
        break;
    } 

    config.Rank = 1;
    //config.SamplingTime = ;
    if(HAL_ADC_ConfigChannel(&hadc1, &config)!=HAL_OK)
    {
    //error handler
    }

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,timeout);
    adc_val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return adc_val;

}