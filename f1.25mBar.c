void Read_Pressure_1_25mbar() {
    unsigned int adcRaw;
    float voltage, pressure_pa, pressure_mbar;
    char txt[128];

    float Vsup = 5.0;

    adcRaw = Read_ADC_Channel(4);  // CH4
    voltage = (adcRaw / 4095.0) * Vsup;

    // Diferansiyel basınç hesaplama
    pressure_pa = voltage / 0.019;              // Pa cinsinden
    pressure_mbar = pressure_pa / 100.0;        // mBar cinsinden

    sprintf(txt,
        "ADC: %u\r\nVoltage: %.3f V\r\nPressure: %.2f Pa / %.3f mBar\r\n\r\n",
        adcRaw, voltage, pressure_pa, pressure_mbar);

    UART3_Write_Text(txt);
    Delay_ms(1000);
}
